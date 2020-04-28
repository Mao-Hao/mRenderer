
1. 添加几何着色器
2. 整理RS函数

``` cpp
template <class Info>
class mShader : public Info
{
public:
    #pragma region uniforms
    Mat m, v, p;
    #pragma endregion uniforms

    #pragma region varyings
    std::array<Vec4f, 3> vertices;
    std::array<Vec2f, 3> texcoords;
    Vec2f uv;
    #pragma endregion varyings

    #pragma region shaders
    virtual std::array<Vec4f, 3> VertexShader( int faceIndex ) = 0;
    virtual std::array<Vec4f, 3> GeometryShader() = 0;
    // true -> discard
    virtual bool FrameShader( Vec3f bc, _Out_ Vec3f & color ) = 0;
    #pragma endregion shaders
};
```