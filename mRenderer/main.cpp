#include <unordered_map>
#include <string>
#include <functional>

#include "testNormal.h"
#include "testVS.h"

using std::unordered_map;
using std::string;
using std::function;

unordered_map<string, function<int(int, char*[])>> testCase = {
    {"Normal", testNormal},
    {"VertexShader", testVS}
};

int main(int argc, char *argv[]) 
{
    auto testFunc = testCase.at("VertexShader");
    testFunc(argc, argv);

    return 0;
}