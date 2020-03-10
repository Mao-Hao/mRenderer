#include <unordered_map>
#include <string>
#include <functional>
#include "testOCH.h"
#include "testNormal.h"
#include "testVS.h"

using std::unordered_map;
using std::string;
using std::function;

unordered_map<string, function<int( int, char * [] )>> testCase = {
    {"Orbital", testOCH},
    {"Normal", testNormal},
    {"VertexShader", testVS}
};

int main( int argc, char * argv[] )
{
    auto testFunc = testCase.at("VertexShader");
    //auto testFunc = testCase.at( "Orbital" );
    testFunc( argc, argv );

    return 0;
}