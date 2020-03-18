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

void line( Vec2i & p0, Vec2i & p1, mColor mc )
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

void mTriangle( mPoint2D & A, mPoint2D & B, mPoint2D & C )
{
    if ( A.p.y == B.p.y && A.p.y == C.p.y )   return; // 退化三角形
    // 包围盒 max, min 
    int xmin = mMax( 0, mMin3( A.p.x, B.p.x, C.p.x ) );
    int ymin = mMax( 0, mMin3( A.p.y, B.p.y, C.p.y ) );
    int xmax = mMin( mDevice::width, mMax3( A.p.x, B.p.x, C.p.x ) );
    int ymax = mMin( mDevice::height, mMax3( A.p.y, B.p.y, C.p.y ) );

    // 注意带参宏的展开方式，若前有参数 x，则每个 x 都会被识别为参数
    #define F(A, B, X, Y) ((A##.y-B##.y)*X + (B##.x-A##.x)*Y + A##.x*B##.y - B##.x*A##.y)
    float Falpha = F( B.p, C.p, A.p.x, A.p.y );
    float Fbeta = F( C.p, A.p, B.p.x, B.p.y );
    float Fgamma = F( A.p, B.p, C.p.x, C.p.y );
    float alpha, beta, gamma;
    mColor mc = White;
    for ( int y = ymin; y < ymax; y++ )
        for ( int x = xmin; x < xmax; x++ ) {
            alpha = F( B.p, C.p, x, y ) / Falpha;
            if ( alpha < 0 )  continue;
            beta = F( C.p, A.p, x, y ) / Fbeta;
            //gamma = 1 - alpha - beta; // 存在误差
            gamma = F( A.p, B.p, x, y ) / Fgamma;
            // 改为 < 和 || continue 更好
            if ( alpha >= 0 && beta >= 0 && gamma >= 0 ) {
                if ( ( alpha > 0 || Falpha * F( B.p, C.p, -1, -1 ) ) &&
                    ( beta > 0 || Fbeta * F( C.p, A.p, -1, -1 ) ) &&
                     ( gamma > 0 || Fgamma * F( A.p, B.p, -1, -1 ) ) ) {
                    mc = A.mc * alpha + B.mc * beta + C.mc * gamma;
                    mDevice::setPixel( x, y, mc );
                }
            }
        }
    #undef F(A, B, X, Y)
}


// alis
#define PNTS    shader->vertices
#define F(A, B, X, Y) ((A##.y-B##.y)*X + (B##.x-A##.x)*Y + A##.x*B##.y - B##.x*A##.y)
void mRasterize( mShader * shader, int faceIndex )
{
    shader->VertexShader( faceIndex );

    if ( mClip( PNTS[0] ) || mClip( PNTS[1] ) || mClip( PNTS[2] ) )   return;

    if ( PNTS[0].y == PNTS[1].y && PNTS[0].y == PNTS[2].y )   return;     // 退化三角形

    for ( auto & v : PNTS ) {
        v = vp * v;
        float inW = 1.0f / v.w;
        v = { v.x * inW, v.y * inW, v.z * inW, inW };
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
                    //shader->_uv = ( shader->texcoords[0] * alpha / PNTS[0].z +
                    //                   shader->texcoords[1] * beta / PNTS[1].z +
                    //                   shader->texcoords[2] * gamma / PNTS[2].z ) * z;
                    shader->_uv = ( shader->texcoords[0] * alpha * PNTS[0].w +
                                    shader->texcoords[1] * beta * PNTS[1].w +
                                    shader->texcoords[2] * gamma * PNTS[2].w ) / w;

                    if ( !shader->FrameShader( { alpha, beta, gamma }, mc ) ) {
                        mDevice::setPixel( x, y, mc );
                    }
                }
            }
        }
    }
}
#undef PNTS
#undef F(A, B, X, Y)