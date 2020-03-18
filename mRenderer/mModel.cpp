#include <iostream>
#include <fstream>
#include <sstream>
#include "mModel.h"
using namespace std;

mModel::mModel( const char * path, float sz ) : modelPath( path )
{
    ifstream input;
    input.open( path, ifstream::in );
    if ( input.fail() )   return;
    string line;
    while ( !input.eof() ) {
        getline( input, line );
        istringstream iss( line.c_str() );
        char trash;
        if ( !line.compare( 0, 2, "v " ) ) {
            iss >> trash;
            Vec3f v;
            for ( int i = 0; i < 3; i++ )
                iss >> v[i];
            vertices.push_back( v * sz );
        }
        else if ( !line.compare( 0, 3, "vn " ) ) {
            iss >> trash >> trash;
            Vec3f norm;
            for ( int i = 0; i < 3; i++ ) iss >> norm[i];
            normals.push_back( norm );
        }
        else if ( !line.compare( 0, 3, "vt " ) ) {
            iss >> trash >> trash;
            Vec2f uv;
            for ( int i = 0; i < 2; i++ ) iss >> uv[i];
            texcoords.push_back( uv );
        }
        else if ( !line.compare( 0, 2, "f " ) ) {
            vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;
            while ( iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2] ) {
                for ( int i = 0; i < 3; i++ ) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back( tmp );
            }
            faces.push_back( f );
        }
    }
}

mModel::~mModel() {}

bool mModel::bindTexture( mTGATexture & tex, const char * suffix )
{
    return loadTexture( modelPath, suffix, tex );
}

bool mModel::loadTexture( const char * path, const char * suffix, mTGATexture & tex )
{
    string texfile( path );
    bool flag = false;
    size_t dot = texfile.find_last_of( "." );
    if ( dot != string::npos ) {
        texfile = texfile.substr( 0, dot ) + string( suffix );
        flag = tex.loadTGAImage( texfile.c_str() );
        //tex.flip_vertically();
    }
    cout << "loadTexture " << (flag ? "succeed.\n" : "failed\n");
    return flag;
}

int mModel::vertSize()
{
    return (int)vertices.size();
}

int mModel::facesSize()
{
    return (int)faces.size();
}

Vec4f mModel::getVertex( int index )
{
    return Vec4f( ( vertices[index].x ),
                  -( vertices[index].y ),
                  ( vertices[index].z ), 1.0f );
}

Vec4f mModel::getVertex( int faceIndex, int vertIndex )
{
    return Vec4f( ( vertices[faces[faceIndex][vertIndex][0]].x ),
                  -( vertices[faces[faceIndex][vertIndex][0]].y ),
                  ( vertices[faces[faceIndex][vertIndex][0]].z ), 1.0f );
}

std::array<int, 3> mModel::getFace( int index )
{
    array<int, 3> face = { 0 };
    for ( int i = 0; i < (int)faces[index].size(); i++ )
        face[i] = faces[index][i][0];
    return face;
}

Vec2f mModel::getTexcoord( int faceIndex, int vertIndex )
{
    return texcoords[faces[faceIndex][vertIndex][1]];
}

Vec3f mModel::getNormal( int faceIndex, int vertIndex )
{
    int index = faces[faceIndex][vertIndex][2];
    return Vec3f( normals[index].x, -normals[index].y, normals[index].z ).normalize();
    //return normals[index].normalize();
}

Vec3f mModel::getNormal( Vec2f _texcoord )
{
    // TODO
    return Vec3f();
}

mColor mModel::diffuse( Vec2f _uv )
{
    Vec2i uv( _uv[0] * diffuseMap.width, _uv[1] * diffuseMap.height );
    auto color = diffuseMap.getColor( uv[0], uv[1] );
    return { color.x, color.y, color.z };
}

float mModel::specular( Vec2f _uv )
{
    Vec2i uv( _uv[0] * specularMap.width, _uv[1] * specularMap.width );
    return specularMap.getColor( uv[0], uv[1] )[2] / 255.f;
}

mColor mModel::getSpecularColor( Vec2f _uv )
{
    Vec2i uv( _uv[0] * specularMap.width, _uv[1] * specularMap.width );
    return proj<3>(specularMap.getColor( uv[0], uv[1] ));
}