#include "mModel.h"

#include <vector>
#include <string>

inline std::vector<mModel *> loadModels( const char * name )
{
    std::vector<mModel *> models;

    if ( name == "assassin" ) {
        models.push_back( new mModel( "obj/assassin/hair.obj", 0.01f ) );
        models.push_back( new mModel( "obj/assassin/body.obj", 0.01f ) );
        models.push_back( new mModel( "obj/assassin/weapon.obj", 0.01f ) );
        for ( auto m : models ) {
            m->bindTexture( m->diffuseMap, "_basecolor.tga" );
            m->bindTexture( m->normalMap, "_normal.tga" );
            m->bindTexture( m->specularMap, "_metalness.tga" );
        }
        models.push_back( new mModel( "obj/assassin/face.obj", 0.01f ) );
        models.back()->bindTexture( models.back()->diffuseMap, "_basecolor.tga" );
    }
    else if ( name == "diablo" ) {
        auto m = new mModel( "obj/diablo3_pose/diablo3_pose.obj" );
        m->bindTexture( m->diffuseMap, "_diffuse.tga" );
        m->bindTexture( m->normalMap, "_nm.tga" );
        m->bindTexture( m->specularMap, "_spec.tga" );
        models.push_back( m );
    }
    else if ( name == "head" ) {
        auto m = new mModel( "obj/african_head/african_head.obj" );
        m->bindTexture( m->diffuseMap, "_diffuse.tga" );
        m->bindTexture( m->normalMap, "_nm.tga" );
        m->bindTexture( m->specularMap, "_spec.tga" );
        models.push_back( m );
        m = new mModel( "obj/african_head/african_head_eye_inner.obj" );
        m->bindTexture( m->diffuseMap, "_diffuse.tga" );
        m->bindTexture( m->normalMap, "_nm.tga" );
        m->bindTexture( m->specularMap, "_spec.tga" );
        models.push_back( m );
        m = new mModel( "obj/african_head/african_head_eye_outer.obj" );
        m->bindTexture( m->diffuseMap, "_diffuse.tga" );
        m->bindTexture( m->normalMap, "_nm.tga" );
        m->bindTexture( m->specularMap, "_spec.tga" );
        models.push_back( m );
    }
    else if ( name == "marble_box" ) {
        auto m = new mModel( "obj/box/box.obj" );
        m->bindTexture( m->diffuseMap, "_diff.tga" );
        models.push_back( m );

    }
    else if ( name == "wood_box" ) {
        auto m = new mModel( "obj/wood_box/wood_box.obj" );
        m->bindTexture( m->diffuseMap, "_diff.tga" );
        models.push_back( m );

    }
    else if ( name == "brick_floor" ) {
        auto m = new mModel( "obj/brick_floor/brick_floor.obj" );
        m->bindTexture( m->diffuseMap, "_diff.tga" );
        m->bindTexture( m->normalMap, "_nm.tga" );
        models.push_back( m );

    }


    return models;
}