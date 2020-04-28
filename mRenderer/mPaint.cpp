#include "mPaint.h"
#include "mDevice.h"
#include <Windows.h>

static Mat viewport( int x, int y, int w, int h )
{
    auto vp = Mat::identity();
    vp[0][3] = x + w / 2.f;
    vp[1][3] = y + h / 2.f;
    vp[2][3] = 255.f / 2.f;
    vp[0][0] = w / 2.f;
    vp[1][1] = h / 2.f;
    vp[2][2] = 255.f / 2.f;
    return vp;
}

static Mat vp = viewport( 0, 0, mDevice::width, mDevice::height );

// true -> discard
inline bool mClip( Vec4f v )
{
    if ( v.x < -v.w || v.x > v.w || v.y < -v.w || v.y > v.w || v.z < -v.w || v.z > v.w )
        return true;
    return false;
}

static inline Vec2i NDC2Screen( Vec3f p )
{
    return Vec2i( ( p.x ) * mDevice::width, ( p.y ) * mDevice::height );
}

void line( Vec2i p0, Vec2i p1, mColor mc )
{
    int x0 = p0.x, x1 = p1.x;
    int y0 = p0.y, y1 = p1.y;
    int dx = abs( x1 - x0 ), sx = x0 < x1 ? 1 : -1;
    int dy = abs( y1 - y0 ), sy = y0 < y1 ? 1 : -1;
    int err = ( dx > dy ? dx : -dy ) / 2;

    while ( x0 != x1 || y0 != y1 ) {
        mDevice::setPixel( x0, y0, mc );
        int e2 = err;
        if ( e2 > -dx ) { err -= dy; x0 += sx; }
        if ( e2 < dy ) { err += dx; y0 += sy; }
    }
}

// 插值版本
void mLine( mPoint2D & p0, mPoint2D & p1 )
{
    int x0 = p0.p.x, x1 = p1.p.x;
    int y0 = p0.p.y, y1 = p1.p.y;
    int dx = abs( x1 - x0 ), sx = x0 < x1 ? 1 : -1;
    int dy = abs( y1 - y0 ), sy = y0 < y1 ? 1 : -1;
    int err = ( dx > dy ? dx : -dy ) / 2;

    while ( x0 != x1 || y0 != y1 ) {
        float dx_px1 = ( x0 - p1.p.x );
        float dy_py1 = ( y0 - p1.p.y );
        float t = sqrtf( dx_px1 * dx_px1 + dy_py1 * dy_py1 ) / sqrtf( dx * dx + dy * dy );
        auto mc = p0.mc * t + p1.mc * ( 1 - t );

        mDevice::setPixel( x0, y0, mc );
        int e2 = err;
        if ( e2 > -dx ) { err -= dy; x0 += sx; }
        if ( e2 < dy ) { err += dx; y0 += sy; }
    }
}

static inline bool isBackFace( std::array<Vec4f, 3> vertices )
{
    auto a = proj<3>( vertices[0] );
    auto b = proj<3>( vertices[1] );
    auto c = proj<3>( vertices[2] );
    auto ab = b - a;
    auto ac = c - a;
    return (cross(ab, ac).z >= 0);
}

// alis
#define PNTS    shader->vertices
#define TEXS    shader->texcoords
#define F(A, B, X, Y) ((A##.y-B##.y)*X + (B##.x-A##.x)*Y + A##.x*B##.y - B##.x*A##.y)
void mRasterize( mShader * shader, int faceIndex )
{
    shader->VertexShader( faceIndex );
    //if ( mClip( PNTS[0] ) || mClip( PNTS[1] ) || mClip( PNTS[2] ) )   continue;
    //if ( PNTS[0].y == PNTS[1].y && PNTS[0].y == PNTS[2].y )   continue;     // 退化三角形
    //if ( isBackFace( PNTS ) )     continue;
    for ( auto & pnt : PNTS ) {
        pnt = vp * pnt;
        float inW = 1.0f / pnt.w;
        pnt = { pnt.x * inW, pnt.y * inW, pnt.z * inW, inW };
    }
    // 包围盒 max, min 
    float xmin = mMax( 0.0f, mMin3( PNTS[0].x, PNTS[1].x, PNTS[2].x ) );
    float ymin = mMax( 0.0f, mMin3( PNTS[0].y, PNTS[1].y, PNTS[2].y ) );
    float xmax = mMin( (float)mDevice::width, mMax3( PNTS[0].x, PNTS[1].x, PNTS[2].x ) );
    float ymax = mMin( (float)mDevice::height, mMax3( PNTS[0].y, PNTS[1].y, PNTS[2].y ) );

    float Falpha = F( PNTS[1], PNTS[2], PNTS[0].x, PNTS[0].y );
    float Fbeta = F( PNTS[2], PNTS[0], PNTS[1].x, PNTS[1].y );
    float Fgamma = F( PNTS[0], PNTS[1], PNTS[2].x, PNTS[2].y );
    float alpha, beta, gamma, z, w;
    mColor mc = White;
    for ( int y = ymin; y < ymax; y++ ) {
        for ( int x = xmin; x < xmax; x++ ) {
            alpha = F( PNTS[1], PNTS[2], x, y ) / Falpha;
            if ( alpha < 0 )                continue;   // 提前减枝
            beta = F( PNTS[2], PNTS[0], x, y ) / Fbeta;
            //gamma = F( PNTS[0], PNTS[1], x, y ) / Fgamma;
            gamma = 1 - alpha - beta; // 存在误差 // 貌似效率提高不大
            if ( beta < 0 || gamma < 0 )    continue;
            if ( ( alpha > 0 || Falpha * F( PNTS[1], PNTS[2], -1, -1 ) )
                 && ( beta > 0 || Fbeta * F( PNTS[2], PNTS[0], -1, -1 ) )
                 && ( gamma > 0 || Fgamma * F( PNTS[0], PNTS[1], -1, -1 ) ) ) {
                z = PNTS[0].z * alpha + PNTS[1].z * beta + PNTS[2].z * gamma;
                //z = 1.0f / ( alpha / PNTS[0].z + beta / PNTS[1].z + gamma / PNTS[2].z );
                //z = ( 1.0f / z - invZNear ) / ( invDZFN );
                w = PNTS[0].w * alpha + PNTS[1].w * beta + PNTS[2].w * gamma;
                z *= w;
                if ( mDevice::mZTest( x, y, z ) ) {
                    shader->uv = ( TEXS[0] * alpha * PNTS[0].w + TEXS[1] * beta * PNTS[1].w + TEXS[2] * gamma * PNTS[2].w ) / w;
                    if ( !shader->FrameShader( { alpha, beta, gamma }, mc ) ) {
                        //mc = Vec3f(mDevice::zbuffer[y][x], mDevice::zbuffer[y][x] , mDevice::zbuffer[y][x] );
                        mDevice::setPixel( x, y, mc );
                    }
                }
            }
        }
    }
}


// 设计目的
// 1. 需要增加阴影， mRasterize 实现困难。问题有关数据通信(shadowbuffer和shadowshader之间)
// 2. 需要兼容多种渲染模式，比如线框，这需要不同的 mRasterize
//      那么，如何获得不同版本的 mRasterize 呢？
//          想法一，使用宏，将有区别的定义放到宏里面，编译器生成唯一的一个 mRasterize
//          想法二，用函数指针，指向不同定义的 mRasterize
//          想法三，用工厂类
//          想法四，偏特化

template <RenderMode M>
void render( mModel * model, mShader * shader ) {}

template <>
void render<RenderMode::NORMAL>( mModel * model, mShader * shader )
{
    for ( int i = 0; i != model->facesSize(); i++ ) {
        shader->VertexShader( i );
        shader->GeometryShader();
        if ( mClip( PNTS[0] ) || mClip( PNTS[1] ) || mClip( PNTS[2] ) )   continue;
        if ( isBackFace(PNTS) )     continue;

        for ( auto & pnt : PNTS ) {
            pnt = vp * pnt;
            float inW = 1.0f / pnt.w;
            pnt = { pnt.x * inW, pnt.y * inW, pnt.z * inW, inW };
        }
        // 包围盒 max, min 
        float xmin = mMax( 0.0f, mMin3( PNTS[0].x, PNTS[1].x, PNTS[2].x ) );
        float ymin = mMax( 0.0f, mMin3( PNTS[0].y, PNTS[1].y, PNTS[2].y ) );
        float xmax = mMin( (float)mDevice::width, mMax3( PNTS[0].x, PNTS[1].x, PNTS[2].x ) );
        float ymax = mMin( (float)mDevice::height, mMax3( PNTS[0].y, PNTS[1].y, PNTS[2].y ) );

        float Falpha = F( PNTS[1], PNTS[2], PNTS[0].x, PNTS[0].y );
        float Fbeta = F( PNTS[2], PNTS[0], PNTS[1].x, PNTS[1].y );
        float Fgamma = F( PNTS[0], PNTS[1], PNTS[2].x, PNTS[2].y );
        float alpha, beta, gamma, z, w;
        mColor mc = White;
        for ( int y = ymin; y < ymax; y++ ) {
            for ( int x = xmin; x < xmax; x++ ) {
                alpha = F( PNTS[1], PNTS[2], x, y ) / Falpha;
                if ( alpha < 0 )                continue;   // 提前减枝
                beta = F( PNTS[2], PNTS[0], x, y ) / Fbeta;
                //gamma = F( PNTS[0], PNTS[1], x, y ) / Fgamma;
                gamma = 1 - alpha - beta; // 存在误差
                if ( beta < 0 || gamma < 0 )    continue;
                if ( ( alpha > 0 || Falpha * F( PNTS[1], PNTS[2], -1, -1 ) )
                     && ( beta > 0 || Fbeta * F( PNTS[2], PNTS[0], -1, -1 ) )
                     && ( gamma > 0 || Fgamma * F( PNTS[0], PNTS[1], -1, -1 ) ) ) {
                    z = PNTS[0].z * alpha + PNTS[1].z * beta + PNTS[2].z * gamma;
                    w = PNTS[0].w * alpha + PNTS[1].w * beta + PNTS[2].w * gamma;
                    z *= w;
                    if ( mDevice::mZTest( x, y, z ) ) {
                        // 矫正纹理（用的invW）
                        shader->uv = ( TEXS[0] * alpha * PNTS[0].w + TEXS[1] * beta * PNTS[1].w + TEXS[2] * gamma * PNTS[2].w ) / w;
                        if ( !shader->FrameShader( { alpha, beta, gamma }, mc ) ) {
                            mDevice::setPixel( x, y, mc );
                        }
                    }
                }
            }
        }
    }
}

template <>
void render<RenderMode::POINTS>( mModel * model, mShader * shader )
{
    mColor mc = White;
    for ( int i = 0; i != model->facesSize(); i++ ) {
        shader->VertexShader( i );
        for ( int j = 0; j < 3; j++ ) {
            if ( mClip( PNTS[j] ) )   continue;
            PNTS[j] = vp * PNTS[j];
            float inW = 1.0f / PNTS[j].w;
            PNTS[j] = { PNTS[j].x * inW, PNTS[j].y * inW, PNTS[j].z * inW, inW };
            mDevice::setPixel( PNTS[j].x, PNTS[j].y, mc );
        }
    }
}

// TODO, 插值版本
template <>
void render<RenderMode::WIRE_FRAME>( mModel * model, mShader * shader )
{
    mColor mc = White;
    for ( int i = 0; i != model->facesSize(); i++ ) {
        shader->VertexShader( i );
        // TODO 裁剪，重新写，判断与边框的交点
        if ( mClip( PNTS[0] ) || mClip( PNTS[1] ) || mClip( PNTS[2] ) )   continue;
        for ( auto & pnt : PNTS ) {
            pnt = vp * pnt;
            float inW = 1.0f / pnt.w;
            pnt = { pnt.x * inW, pnt.y * inW, pnt.z * inW, inW };
        }
        line( proj<2>( PNTS[0] ), proj<2>( PNTS[1] ) );
        line( proj<2>( PNTS[0] ), proj<2>( PNTS[2] ) );
        line( proj<2>( PNTS[1] ), proj<2>( PNTS[2] ) );
    }
}

template <>
void render<RenderMode::DEPTH>( mModel * model, mShader * shader )
{
    for ( int i = 0; i != model->facesSize(); i++ ) {
        shader->VertexShader( i );
        if ( mClip( PNTS[0] ) || mClip( PNTS[1] ) || mClip( PNTS[2] ) )   return;
        //if ( PNTS[0].y == PNTS[1].y && PNTS[0].y == PNTS[2].y )   return;     // 退化三角形
        if ( isBackFace( PNTS ) )     continue;
        for ( auto & pnt : PNTS ) {
            pnt = vp * pnt;
            float inW = 1.0f / pnt.w;
            pnt = { pnt.x * inW, pnt.y * inW, pnt.z * inW, inW };
        }
        float xmin = mMax( 0.0f, mMin3( PNTS[0].x, PNTS[1].x, PNTS[2].x ) );
        float ymin = mMax( 0.0f, mMin3( PNTS[0].y, PNTS[1].y, PNTS[2].y ) );
        float xmax = mMin( (float)mDevice::width, mMax3( PNTS[0].x, PNTS[1].x, PNTS[2].x ) );
        float ymax = mMin( (float)mDevice::height, mMax3( PNTS[0].y, PNTS[1].y, PNTS[2].y ) );

        float Falpha = F( PNTS[1], PNTS[2], PNTS[0].x, PNTS[0].y );
        float Fbeta = F( PNTS[2], PNTS[0], PNTS[1].x, PNTS[1].y );
        float Fgamma = F( PNTS[0], PNTS[1], PNTS[2].x, PNTS[2].y );
        float alpha, beta, gamma, z, w;
        mColor mc = White;
        for ( int y = ymin; y < ymax; y++ ) {
            for ( int x = xmin; x < xmax; x++ ) {
                alpha = F( PNTS[1], PNTS[2], x, y ) / Falpha;
                if ( alpha < 0 )                continue;
                beta = F( PNTS[2], PNTS[0], x, y ) / Fbeta;
                gamma = 1 - alpha - beta;
                if ( beta < 0 || gamma < 0 )    continue;
                if ( ( alpha > 0 || Falpha * F( PNTS[1], PNTS[2], -1, -1 ) )
                     && ( beta > 0 || Fbeta * F( PNTS[2], PNTS[0], -1, -1 ) )
                     && ( gamma > 0 || Fgamma * F( PNTS[0], PNTS[1], -1, -1 ) ) ) {
                    z = PNTS[0].z * alpha + PNTS[1].z * beta + PNTS[2].z * gamma;
                    w = PNTS[0].w * alpha + PNTS[1].w * beta + PNTS[2].w * gamma;
                    z *= w;
                    //z = ( 1.0f / z - invZNear ) / ( invDZFN );
                    if ( mDevice::mZTest( x, y, z ) ) {
                        if ( !shader->FrameShader( { alpha, beta, gamma }, mc ) ) {
                            mc = Vec3f( mDevice::zbuffer[y][x], mDevice::zbuffer[y][x], mDevice::zbuffer[y][x] );
                            mDevice::setPixel( x, y, mc );
                        }
                    }
                }
            }
        }
    }
}

#undef PNTS
#undef TEXS
#undef F(A, B, X, Y)