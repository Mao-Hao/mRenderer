#include <unordered_map>
#include <string>
#include <functional>

#include "testOCH.h"

#include "test_1_phong.h"
#include "test_2_geometry.h"

using std::unordered_map;
using std::string;
using std::function;

unordered_map<string, function<int( int, char * [] )>> testCase = {
    //{"Orbital", testOCH},    
    {"phong", test_1_phong},
    {"geometry", test_2_geometry}
};

int main( int argc, char * argv[] )
{
    //auto testFunc = testCase.at("phong");
    auto testFunc = testCase.at( "geometry" );
    testFunc( argc, argv );
    return 0;
}