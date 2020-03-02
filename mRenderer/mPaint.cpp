#include "mPaint.h"
#include "mDevice.h"

static inline Vec2i NDC2Screen(Vec3f & p) {
    return Vec2i((p.x) * mDevice::width, (p.y) * mDevice::height);
}

void line(Vec2i & p0, Vec2i & p1, mColor mc) {
    int x0 = p0.x, x1 = p1.x;
    int y0 = p0.y, y1 = p1.y;
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (x0 != x1 || y0 != y1) {
        mDevice::setPixel(x0, y0, mc);
        int e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 <  dy) { err += dx; y0 += sy; }
    }
}

// 插值版本
void mLine(mPoint2D & p0, mPoint2D & p1) {
    int x0 = p0.p.x, x1 = p1.p.x;
    int y0 = p0.p.y, y1 = p1.p.y;
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (x0 != x1 || y0 != y1) {  
        float dx_px1 = (x0 - p1.p.x);
        float dy_py1 = (y0 - p1.p.y);
        float t = sqrtf(dx_px1 * dx_px1 + dy_py1 * dy_py1) / sqrtf(dx * dx + dy * dy);
        auto mc = p0.mc * t + p1.mc * (1 - t);

        mDevice::setPixel(x0, y0, mc);
        int e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 <  dy) { err += dx; y0 += sy; }
    }
}


static inline Vec3f barycentric(Vec2i a, Vec2i b, Vec2i c, Vec2i p) {  
    auto  sx = Vec3i(b.x - a.x, c.x - a.x, a.x - p.x);
    auto  sy = Vec3i(b.y - a.y, c.y - a.y, a.y - p.y);
    Vec3f u  = cross(sx, sy);
    if (std::abs(u[2]) < 1) return Vec3f(-1, 1, 1);
    return Vec3f(1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

// bug
void triangle(mPoint2D & A, mPoint2D & B, mPoint2D & C) {
    if (A.p.y == B.p.y && A.p.y == C.p.y) return;
    // 包围盒 max, min 防止负值/过大值 
    int xmin = mMax(0, mMin3(A.p.x, B.p.x, C.p.x));   
    int ymin = mMax(0, mMin3(A.p.y, B.p.y, C.p.y));    
    int xmax = mMin(mDevice::width, mMax3(A.p.x, B.p.x, C.p.x));
    int ymax = mMin(mDevice::height, mMax3(A.p.y, B.p.y, C.p.y));

    for (int y = ymin; y <= ymax; y++)
        for (int x = xmin; x <= xmax; x++) {
            Vec3f k = barycentric(A.p, B.p, C.p, Vec2i(x, y));
            if (k.x > 0 && k.y > 0 && k.z > 0) {
                mColor mc = A.mc * k.x + B.mc * k.y + C.mc * k.z; 
                mDevice::setPixel(x, y, mc);
            }
        }
}

// 待优化 但还是比楼上快
// 1. Gamma的计算可优化（在解决误差的前提下）
// 2. 可改为增量算法
// 3. 对退化三角形，即 F* = 0 ，可能出现除数为0现象 done
// 4. 三角形边正好过(-1, -1)的情况如何处理？
// todo:
// 1. zbuffer
// 2. bool discard = 
// 3. clip
void mTriangle(mPoint2D & A, mPoint2D & B, mPoint2D & C) {
    if (A.p.y == B.p.y && A.p.y == C.p.y)   return; // 退化三角形
    // 包围盒 max, min 
    int xmin = mMax(0, mMin3(A.p.x, B.p.x, C.p.x));   
    int ymin = mMax(0, mMin3(A.p.y, B.p.y, C.p.y));    
    int xmax = mMin(mDevice::width,  mMax3(A.p.x, B.p.x, C.p.x));
    int ymax = mMin(mDevice::height, mMax3(A.p.y, B.p.y, C.p.y));

    // 注意带参宏的展开方式，若前有参数 x，则每个 x 都会被识别为参数
#define F(A, B, X, Y) ((A##.y-B##.y)*X + (B##.x-A##.x)*Y + A##.x*B##.y - B##.x*A##.y)
    float Falpha = F(B.p, C.p, A.p.x, A.p.y);
    float Fbeta  = F(C.p, A.p, B.p.x, B.p.y);
    float Fgamma = F(A.p, B.p, C.p.x, C.p.y);
    float alpha, beta, gamma;
    mColor mc = White;
    for (int y = ymin; y < ymax; y++)
        for (int x = xmin; x < xmax; x++) {
            alpha = F(B.p, C.p, x, y) / Falpha;
            if (alpha < 0)  continue;
            beta  = F(C.p, A.p, x, y) / Fbeta;
            //gamma = 1 - alpha - beta; // 存在误差
            gamma = F(A.p, B.p, x, y) / Fgamma;
            // 改为 < 和 || continue 更好
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                if ((alpha > 0 || Falpha * F(B.p, C.p, -1, -1)) && 
                    (beta  > 0 || Fbeta  * F(C.p, A.p, -1, -1)) && 
                    (gamma > 0 || Fgamma * F(A.p, B.p, -1, -1))) {     
                    mc = A.mc * alpha + B.mc * beta + C.mc * gamma;  
                    mDevice::setPixel(x, y, mc);
                }
            }
        }
#undef F(A, B, X, Y)
}

void mTriangleZ(mVertex & A, mVertex & B, mVertex & C) {
    Vec2i a = NDC2Screen(A.p);
    Vec2i b = NDC2Screen(B.p);
    Vec2i c = NDC2Screen(C.p);

    if (a.y == b.y && a.y == c.y)   return; // 退化三角形

    // 包围盒 max, min 
    int xmin = mMax(0, mMin3(a.x, b.x, c.x));   
    int ymin = mMax(0, mMin3(a.y, b.y, c.y));    
    int xmax = mMin(mDevice::width,  mMax3(a.x, b.x, c.x));
    int ymax = mMin(mDevice::height, mMax3(a.y, b.y, c.y));

    // 注意带参宏的展开方式，若前有参数 x，则每个 x 都会被识别为参数
#define F(A, B, X, Y) ((A##.y-B##.y)*X + (B##.x-A##.x)*Y + A##.x*B##.y - B##.x*A##.y)
    float Falpha = F(b, c, a.x, a.y);
    float Fbeta  = F(c, a, b.x, b.y);
    float Fgamma = F(a, b, c.x, c.y);
    float alpha, beta, gamma, z;
    mColor mc = White;
    for (int y = ymin; y < ymax; y++)
        for (int x = xmin; x < xmax; x++) {
            alpha = F(b, c, x, y) / Falpha;
            if (alpha < 0)  continue;
            beta  = F(c, a, x, y) / Fbeta;
            //gamma = 1 - alpha - beta; // 存在误差
            gamma = F(a, b, x, y) / Fgamma;
            // 改为 < 和 || continue 更好
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                if ((alpha > 0 || Falpha * F(b, c, -1, -1)) && 
                    (beta  > 0 || Fbeta  * F(c, a, -1, -1)) && 
                    (gamma > 0 || Fgamma * F(a, b, -1, -1))) { 
                    z = A.p.z * alpha + B.p.z * beta + C.p.z * gamma;
                    z = (1.0f / z - invZNear) / (invDZFN);
                    if (mDevice::mZTest(x, y, z)) {
                        mc = A.mc * alpha + B.mc * beta + C.mc * gamma; 
                        mDevice::setPixel(x, y, mc);
                    }
                }
            }
        }
#undef F(A, B, X, Y)
}