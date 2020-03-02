#include <unordered_map>
#include <string>
#include <functional>

#include "testOCH.h"
#include "testFPSH.h"
#include "testTex.h"
#include "testNormal.h"
#include "testVS.h"

using std::unordered_map;
using std::string;
using std::function;

// function<int(int, char**)> testFunc;
unordered_map<string, function<int(int, char*[])>> testCase = {
    {"OrbitalCamera", testOCH},
    {"FPSCamera", testFPSH},
    {"Texture", testTex},
    {"Normal", testNormal},
    {"VertexShader", testVS}
};

int main(int argc, char *argv[]) 
{
    auto testFunc = testCase.at("Texture");
    //auto testFunc = testCase.at("FPSCamera");
    testFunc(argc, argv);

    return 0;
}