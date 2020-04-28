#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include <array>
#include "mBase.h"

// 基础库，包含各类Vec/Mat
// Cpp Style

const float Epsilon = 1e-6f;
const float Pi = 3.1415926535f;
const float HalfPi = 1.5707963267f;
//--------------------------------------------------------------------------------------
// 以下全是Vec
//--------------------------------------------------------------------------------------
// Vec
template <size_t DIM, typename T> class vec
{
public:
    vec() { for ( size_t i = DIM; i--; data_[i] = T() ); }
    T & operator[]( const size_t i ) { mAssert( i < DIM ); return data_[i]; }
    const T & operator[]( const size_t i ) const { mAssert( i < DIM ); return data_[i]; }
private:
    T data_[DIM];
};

//--------------------------------------------------------------------------------------
// vec偏特化 Vec2i/f
template <typename T> class vec<2, T>
{
public:
    vec() : x( T() ), y( T() ) {}
    vec( T X, T Y ) : x( X ), y( Y ) {}
    // 类型转换
    template <class U> vec<2, T>( const vec<2, U> & v );
    T & operator[]( const size_t i ) { mAssert( i < 2 ); return i <= 0 ? x : y; }
    const T & operator[]( const size_t i ) const { mAssert( i < 2 ); return i <= 0 ? x : y; }

    float norm() { return std::sqrtf( x * x + y * y ); }
    vec<2, T> & normalize( T l = 1 )
    {
        float n = norm();
        if ( n == 0 )     return *this;
        *this = ( *this ) * ( l / n );
        return *this;
    }

    T x, y;
};

template <> template <> inline vec<2, int>  ::vec( const vec<2, float> & v )
    : x( int( v.x + .5f ) ), y( int( v.y + .5f ) )
{
}
template <> template <> inline vec<2, float>::vec( const vec<2, int> & v )
    : x( v.x ), y( v.y )
{
}


//--------------------------------------------------------------------------------------
// vec偏特化 Vec3i/f
template <typename T> class vec<3, T>
{
public:
    vec() : x( T() ), y( T() ), z( T() ) {}
    vec( T X, T Y, T Z ) : x( X ), y( Y ), z( Z ) {}
    // 类型转换
    template <class U> vec<3, T>( const vec<3, U> & v );
    T & operator[]( const size_t i ) { mAssert( i < 3 ); return i <= 0 ? x : ( 1 == i ? y : z ); }
    const T & operator[]( const size_t i ) const { mAssert( i < 3 ); return i <= 0 ? x : ( 1 == i ? y : z ); }

    float norm() { return std::sqrtf( x * x + y * y + z * z ); }
    vec<3, T> & normalize( T l = 1 )
    {
        float n = norm();
        if ( n == 0 )     return *this;
        *this = ( *this ) * ( l / n );
        return *this;
    }

    T x, y, z;
};

template <> template <> inline vec<3, int>  ::vec( const vec<3, float> & v )
    : x( int( v.x + .5f ) ), y( int( v.y + .5f ) ), z( int( v.z + .5f ) )
{
}
template <> template <> inline vec<3, float>::vec( const vec<3, int> & v )
    : x( v.x ), y( v.y ), z( v.z )
{
}

//--------------------------------------------------------------------------------------
// vec偏特化 Vec4i/f
template <typename T> class vec<4, T>
{
public:
    vec() : x( T() ), y( T() ), z( T() ), w( T() ) {}
    vec( T X, T Y, T Z, T W ) : x( X ), y( Y ), z( Z ), w( W ) {}
    // 类型转换
    template <class U> vec<4, T>( const vec<4, U> & v );
    T & operator[]( const size_t i ) { mAssert( i < 4 ); return i <= 0 ? x : ( 1 == i ? y : ( 2 == i ? z : w ) ); }
    const T & operator[]( const size_t i ) const { mAssert( i < 4 ); return i <= 0 ? x : ( 1 == i ? y : ( 2 == i ? z : w ) ); }

    float norm() { return std::sqrtf( x * x + y * y + z * z + w * w ); }
    vec<4, T> & normalize( T l = 1 )
    {
        float n = norm();
        if ( n == 0 )     return *this;
        *this = ( *this ) * ( l / n );
        return *this;
    }

    T x, y, z, w;
};

template <> template <> inline vec<4, int>  ::vec( const vec<4, float> & v )
    : x( int( v.x + .5f ) ), y( int( v.y + .5f ) ), z( int( v.z + .5f ) ), w( int( v.w + .5f ) )
{
}
template <> template <> inline vec<4, float>::vec( const vec<4, int> & v )
    : x( v.x ), y( v.y ), z( v.z ), w( v.w )
{
}

//--------------------------------------------------------------------------------------
// Vec 运算符
// l & r对应项积的和
template<size_t DIM, typename T> T operator*( const vec<DIM, T> & lhs, const vec<DIM, T> & rhs )
{
    T ret = T();
    for ( size_t i = DIM; i--; ret += lhs[i] * rhs[i] );
    return ret;
}

// 负
template<size_t DIM, typename T>vec<DIM, T> operator-( vec<DIM, T> v )
{
    for ( size_t i = DIM; i--; v[i] = -v[i] );
    return v;
}

// 加
template<size_t DIM, typename T>vec<DIM, T> operator+( vec<DIM, T> lhs, const vec<DIM, T> & rhs )
{
    for ( size_t i = DIM; i--; lhs[i] += rhs[i] );
    return lhs;
}
// 减
template<size_t DIM, typename T>vec<DIM, T> operator-( vec<DIM, T> lhs, const vec<DIM, T> & rhs )
{
    for ( size_t i = DIM; i--; lhs[i] -= rhs[i] );
    return lhs;
}
// 乘
template<size_t DIM, typename T, typename U> vec<DIM, T> operator*( vec<DIM, T> lhs, const U & rhs )
{
    for ( size_t i = DIM; i--; lhs[i] *= rhs );
    return lhs;
}

// 除
template<size_t DIM, typename T, typename U> vec<DIM, T> operator/( vec<DIM, T> lhs, const U & rhs )
{
    auto invRhs = 1 / rhs;
    for ( size_t i = DIM; i--; lhs[i] *= invRhs );
    return lhs;
}

// +
template<size_t LEN, size_t DIM, typename T> vec<LEN, T> embed( const vec<DIM, T> & v, T fill = 1 )
{
    vec<LEN, T> ret;
    for ( size_t i = LEN; i--; ret[i] = ( i < DIM ? v[i] : fill ) );
    return ret;
}

// -
template<size_t LEN, size_t DIM, typename T> vec<LEN, T> proj( const vec<DIM, T> & v )
{
    vec<LEN, T> ret;
    for ( size_t i = LEN; i--; ret[i] = v[i] );
    return ret;
}

template <typename T> vec<3, T> cross( vec<3, T> v1, vec<3, T> v2 )
{
    return vec<3, T>( v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x );
}

template <size_t DIM, typename T> std::ostream & operator<<( std::ostream & out, vec<DIM, T> & v )
{
    for ( size_t i = 0; i < DIM; i++ )
        out << v[i] << ' ';
    return out;
}


using Vec2i = vec<2, int>;
using Vec2f = vec<2, float>;
using Vec3i = vec<3, int>;
using Vec3f = vec<3, float>;
using Vec4i = vec<4, int>;
using Vec4f = vec<4, float>;

//--------------------------------------------------------------------------------------
// matrix
template<size_t DimCols, size_t DimRows, typename T> class mat;
//
template<size_t DIM, typename T> struct dt
{
    static T det( const mat<DIM, DIM, T> & src )
    {
        T ret = 0;
        for ( size_t i = DIM; i--; ret += src[0][i] * src.cofactor( 0, i ) );
        return ret;
    }
};

template<typename T> struct dt<1, T>
{
    static T det( const mat<1, 1, T> & src )
    {
        return src[0][0];
    }
};

template<size_t DimRows, size_t DimCols, typename T> class mat
{

    vec<DimCols, T> rows[DimRows];

public:
    mat() {}

    vec<DimCols, T> & operator[] ( const size_t idx )
    {
        mAssert( idx < DimRows );
        return rows[idx];
    }

    const vec<DimCols, T> & operator[] ( const size_t idx ) const
    {
        mAssert( idx < DimRows );
        return rows[idx];
    }

    vec<DimRows, T> col( const size_t idx ) const
    {
        mAssert( idx < DimCols );
        vec<DimRows, T> ret;
        for ( size_t i = DimRows; i--; ret[i] = rows[i][idx] )
            ;
        return ret;
    }

    void setCol( size_t idx, vec<DimRows, T> v )
    {
        mAssert( idx < DimCols );
        for ( size_t i = DimRows; i--; rows[i][idx] = v[i] );
    }

    //---------------------------------------
    //
    static mat<DimRows, DimCols, T> identity();

    static mat<DimRows, DimCols, T> scale( T sx, T sy, T sz );

    static mat<DimRows, DimCols, T> translate( T tx, T ty, T tz );

    static mat<DimRows, DimCols, T> rotate( float angle, T vx, T vy, T vz );
    static mat<DimRows, DimCols, T> rotate_x( float angle );
    static mat<DimRows, DimCols, T> rotate_y( float angle );
    static mat<DimRows, DimCols, T> rotate_z( float angle );

    static mat<DimRows, DimCols, T> lookAt( Vec3f eye, Vec3f gaze, Vec3f up );

    static mat<DimRows, DimCols, T> perspective( float fov, float asp, float n, float f );
    static mat<DimRows, DimCols, T> frustum( float left, float right, float bottom, float top, float zNear, float zFar );
    //
    //---------------------------------------

    T det() const
    {
        return dt<DimCols, T>::det( *this );
    }

    // 求子式
    mat<DimRows - 1, DimCols - 1, T> getMinor( size_t row, size_t col ) const
    {
        mat<DimRows - 1, DimCols - 1, T> ret;
        for ( size_t i = DimRows - 1; i--; )
            for ( size_t j = DimCols - 1; j--; ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1] );
        return ret;
    }

    // 
    T cofactor( size_t row, size_t col ) const
    {
        return getMinor( row, col ).det() * ( ( row + col ) % 2 ? -1 : 1 );
    }

    mat<DimRows, DimCols, T> adjugate() const
    {
        mat<DimRows, DimCols, T> ret;
        for ( size_t i = DimRows; i--; )
            for ( size_t j = DimCols; j--; ret[i][j] = cofactor( i, j ) );
        return ret;
    }

    mat<DimRows, DimCols, T> invertTranspose()
    {
        mat<DimRows, DimCols, T> ret = adjugate();
        T tmp = ret[0] * rows[0];
        return ret / tmp;
    }

    mat<DimRows, DimCols, T> invert()
    {
        return invertTranspose().transpose();
    }

    mat<DimCols, DimRows, T> transpose()
    {
        mat<DimCols, DimRows, T> ret;
        for ( size_t i = DimCols; i--; ret[i] = this->col( i ) );
        return ret;
    }
};

// 返回单位矩阵
// 1  0  0  0
// 0  1  0  0
// 0  0  1  0
// 0  0  0  1
template<size_t DimRows, size_t DimCols, typename T>
inline mat<DimRows, DimCols, T> mat<DimRows, DimCols, T>::identity()
{
    mat<DimRows, DimCols, T> m;
    for ( size_t i = DimRows; i--; )
        for ( size_t j = DimCols; j--; m[i][j] = ( i == j ) )
            ;
    return m;
}

// 返回缩放矩阵
//sx  0  0  0
// 0 sy  0  0
// 0  0 sz  0
// 0  0  0  1
template<size_t DimRows, size_t DimCols, typename T>
inline mat<DimRows, DimCols, T> mat<DimRows, DimCols, T>::scale( T sx, T sy, T sz )
{
    mat<DimRows, DimCols, T> m;
    mAssert( sx != 0 && sy != 0 && sz != 0 );
    m[0][0] = sx;
    m[1][1] = sy;
    m[2][2] = sz;
    m[3][3] = 1;
}

// 返回平移变换矩阵
// 1  0  0 tx
// 0  1  0 ty
// 0  0  1 tz
// 0  0  0  1
template<size_t DimRows, size_t DimCols, typename T>
inline mat<DimRows, DimCols, T> mat<DimRows, DimCols, T>::translate( T tx, T ty, T tz )
{
    auto m = identity();
    m[0][3] = tx;
    m[1][3] = ty;
    m[2][3] = tz;
    return m;
}

//  返回旋转矩阵
//  nx*nx*(1-c)+c     ny*nx*(1-c)-s*nz  nz*nx*(1-c)+s*ny  0
//  nx*ny*(1-c)+s*nz  ny*ny*(1-c)+c     nz*ny*(1-c)-s*nx  0
//  nx*nz*(1-c)-s*ny  ny*nz*(1-c)+s*nx  nz*nz*(1-c)+c     0
//  0                 0                 0                 1

//  nx*nx*(1-c)+c     ny*nx*(1-c)-s*nz  nz*nx*(1-c)+s*ny  0
//  nx*ny*(1-c)+s*nz  ny*ny*(1-c)+c     nz*ny*(1-c)-s*nx  0
//  nx*nz*(1-c)-s*ny  ny*nz*(1-c)+s*nx  nz*nz*(1-c)+c     0
//  0                 0                 0                 1
template<size_t DimRows, size_t DimCols, typename T>
inline mat<DimRows, DimCols, T> mat<DimRows, DimCols, T>::rotate( float angle, T vx, T vy, T vz )
{
    Vec3f n = Vec3f( vx, vy, vz ).normalize();
    float cosa = (float)cos( angle );
    float sina = (float)sin( angle );
    auto m = identity();
    m[0][0] = n.x * n.x * ( 1 - cosa ) + cosa;
    m[0][1] = n.y * n.x * ( 1 - cosa ) - sina * n.z;
    m[0][2] = n.z * n.x * ( 1 - cosa ) + sina * n.y;

    m[1][0] = n.x * n.y * ( 1 - cosa ) + sina * n.z;
    m[1][1] = n.y * n.y * ( 1 - cosa ) + cosa;
    m[1][2] = n.z * n.y * ( 1 - cosa ) - sina * n.x;

    m[2][0] = n.x * n.z * ( 1 - cosa ) - sina * n.y;
    m[2][1] = n.y * n.z * ( 1 - cosa ) + sina * n.x;
    m[2][2] = n.z * n.z * ( 1 - cosa ) + cosa;

    return m;
}

//  返回绕x轴的旋转矩阵
//  1  0  0  0
//  0  c -s  0
//  0  s  c  0
//  0  0  0  1
template<size_t DimRows, size_t DimCols, typename T>
inline mat<DimRows, DimCols, T> mat<DimRows, DimCols, T>::rotate_x( float angle )
{
    float cosa = (float)cos( angle );
    float sina = (float)sin( angle );
    auto m = identity();
    m[1][1] = cosa;
    m[1][2] = -sina;
    m[2][1] = sina;
    m[2][2] = cosa;
    return m;
}

//  返回绕y轴的旋转矩阵
//  c  0  s  0
//  0  1  0  0
// -s  0  c  0
//  0  0  0  1
template<size_t DimRows, size_t DimCols, typename T>
inline mat<DimRows, DimCols, T> mat<DimRows, DimCols, T>::rotate_y( float angle )
{
    float cosa = (float)cos( angle );
    float sina = (float)sin( angle );
    auto m = identity();
    m[0][0] = cosa;
    m[0][2] = sina;
    m[2][0] = -sina;
    m[2][2] = cosa;
    return m;
}

//  返回绕z轴的旋转矩阵
//  c -s  0  0
//  s  c  0  0
//  0  0  1  0
//  0  0  0  1
template<size_t DimRows, size_t DimCols, typename T>
inline mat<DimRows, DimCols, T> mat<DimRows, DimCols, T>::rotate_z( float angle )
{
    float cosa = (float)cos( angle );
    float sina = (float)sin( angle );
    auto m = identity();
    m[0][0] = cosa;
    m[0][1] = -sina;
    m[1][0] = sina;
    m[1][1] = cosa;
    return m;
}


// eye:  眼睛位置
// gaze: 凝视的方向
// up:   向上向量
// x.x  x.y  x.z  -dot(x_axis,eye)
// y.x  y.y  y.z  -dot(y_axis,eye)
// z.x  z.y  z.z  -dot(z_axis,eye)
//   0    0    0                 1
//
// z: normalize(eye-gaze), the backward vector
// x: normalize(cross(up, z)), the right vector
// y: cross(z, x), the up vector
template<size_t DimRows, size_t DimCols, typename T>
inline mat<DimRows, DimCols, T> mat<DimRows, DimCols, T>::lookAt( Vec3f eye, Vec3f gaze, Vec3f up )
{
    Vec3f z = ( eye - gaze ).normalize();
    Vec3f x = cross( up, z ).normalize();
    Vec3f y = cross( z, x ).normalize();
    auto m = identity();
    for ( int i = 0; i < 3; i++ ) {
        m[0][i] = x[i];
        m[1][i] = y[i];
        m[2][i] = z[i];
    }
    m[0][3] = -x * eye;
    m[1][3] = -y * eye;
    m[2][3] = -z * eye;
    return m;
}


// fov: the field of view angle in the y direction, in radians
// asp: the aspect ratio, defined as width divided by height
// n,f: the distances to the near and far depth clipping planes
// 1/(asp*tan(fov/2))             0             0           0
//                  0  1/tan(fov/2)             0           0
//                  0             0  -(f+n)/(f-n) -2f*n/(f-n)
//                  0             0            -1           0
template<size_t DimRows, size_t DimCols, typename T>
inline mat<DimRows, DimCols, T> mat<DimRows, DimCols, T>::perspective( float fov, float asp, float n, float f )
{
    float zRange = f - n;
    auto m = identity();
    mAssert( fov > 0 && asp > 0 );
    mAssert( n > 0 && f > 0 && zRange > 0 );
    m[1][1] = 1.0f / tanf( fov / 2 );
    m[0][0] = m[1][1] / asp;
    m[2][2] = -( n + f ) / zRange;
    m[2][3] = -2 * n * f / zRange;
    m[3][2] = -1;
    m[3][3] = 0;
    return m;
}


//  near, far: the distances to the near and far depth clipping planes
// 
//  2n/(r-l)         0   (r+l)/(r-l)           0
//         0  2n/(t-b)   (t+b)/(t-b)           0
//         0         0  -(f+n)/(f-n)  -2fn/(f-n)
//         0         0            -1           0
// 
//  see http://docs.gl/gl2/glFrustum
template<size_t DimRows, size_t DimCols, typename T>
inline mat<DimRows, DimCols, T> mat<DimRows, DimCols, T>::
frustum( float left, float right, float bottom, float top, float zNear, float zFar )
{
    float xRange = right - left;
    float yRange = top - bottom;
    float zRange = zFar - zNear;
    auto m = identity();
    mAssert( zNear > 0 && zFar > 0 );
    mAssert( xRange > 0 && yRange > 0 && zRange > 0 );
    m[0][0] = 2 * zNear / xRange;
    m[1][1] = 2 * zNear / yRange;
    m[0][2] = ( left + right ) / xRange;
    m[1][2] = ( bottom + top ) / yRange;
    m[2][2] = -( zNear + zFar ) / zRange;
    m[2][3] = -2 * zNear * zFar / zRange;
    m[3][2] = -1;
    m[3][3] = 0;
    return m;
}






using Mat = mat<4, 4, float>;

//------------------------------------------------------------------

template<size_t DimRows, size_t DimCols, typename T>
mat<DimCols, DimRows, T> operator/( mat<DimRows, DimCols, T> lhs, const T & rhs )
{
    T invRhs = (T)1 / rhs;
    for ( size_t i = DimRows; i--; lhs[i] = lhs[i] * invRhs );
    return lhs;
}

template<size_t DimRows, size_t DimCols, typename T>
vec<DimRows, T> operator*( const mat<DimRows, DimCols, T> & lhs, const vec<DimCols, T> & rhs )
{
    vec<DimRows, T> ret;
    for ( size_t i = DimRows; i--; ret[i] = lhs[i] * rhs );
    return ret;
}

template<size_t R1, size_t C1, size_t C2, typename T>
inline mat<R1, C2, T> operator*( const mat<R1, C1, T> & lhs, const mat<C1, C2, T> & rhs )
{
    mat<R1, C2, T> result;
    for ( size_t i = R1; i--; )
        for ( size_t j = C2; j--; result[i][j] = lhs[i] * rhs.col( j ) );
    return result;
}



//--------------------------------------------------------------------------------------
// 类型判断(编译期)
template <typename T> struct is_vec2_imp : std::false_type {};
template <typename T> struct is_vec2_imp<vec<2, T>> : std::true_type {};
template <typename T> constexpr bool is_vec2() { return is_vec2_imp<T>(); }

template <typename T> struct is_vec3_imp : std::false_type {};
template <typename T> struct is_vec3_imp<vec<3, T>> : std::true_type {};
template <typename T> constexpr bool is_vec3() { return is_vec3_imp<T>(); }

template <typename T> constexpr bool is_vec() { return is_vec2<T>() || is_vec3<T>(); }

// 只有 mat<4, 4, float>能过
template <typename T> constexpr bool is_Mat() { return is_same<std::decay<T>::type, Mat>(); }

// ??? 为什么Mat过不了这个if？
//template <typename T> struct is_mat_4x4_imp : std::false_type {};
//template <typename T> struct is_mat_4x4_imp<mat<4, 4, T>> : std::true_type {};
//template <typename T> constexpr bool is_mat() { 
//    if (is_float<T>())  return is_mat_4x4_imp<T>(); 
//    return  false;
//}

//--------------------------------------------------------------------------------------
// 一些数学函数

inline float mRadiansf( float angle ) { return ( angle * Pi ) / 180.0f; }

template <typename T> inline T mMin3( T a, T b, T c )
{
    if ( b < a )  a = b;
    if ( c < a )  return c;
    return a;
}

template <typename T> inline T mMax3( T a, T b, T c )
{
    if ( b > a )  a = b;
    if ( c > a )  return c;
    return a;
}



// 之所以自己写，是因为 std::max 等在包含了 windows.h 的情况下不能使用(宏展开出错？）
template <typename T> inline T mMax( T a, T b ) { return a > b ? a : b; }
template <typename T> inline T mMin( T a, T b ) { return a < b ? a : b; }

// clamp family
template <typename T,
    If_Type( !is_vec<T>() )>
    T mClamp( const T & v, const T & lo, const T & hi )
{
    return v < lo ? lo : hi < v ? hi : v;
}

template <typename T,
    If_Type( is_vec2<T>() )>
    T mClamp( const T & v, const T & lo, const T & hi )
{
    return T( v.x < lo.x ? lo.x : hi.x < v.x ? hi.x : v.x,
              v.y < lo.y ? lo.y : hi.y < v.y ? hi.y : v.y );
}

template <typename T,
    If_Type( is_vec3<T>() )>
    T mClamp( const T & v, const T & lo, const T & hi )
{
    return T( v.x < lo.x ? lo.x : hi.x < v.x ? hi.x : v.x,
              v.y < lo.y ? lo.y : hi.y < v.y ? hi.y : v.y,
              v.z < lo.z ? lo.z : hi.z < v.z ? hi.z : v.z );
}

// 重心插值
template <typename T>
__forceinline T interpolate( std::array<T, 3> & v, Vec3f t )
{
    return v[0] * t.x + v[1] * t.y + v[2] * t.z;
}

// reflect
__forceinline Vec3f reflect( Vec3f i, Vec3f n)
{
    return i - n * 2 * (i * n);
}