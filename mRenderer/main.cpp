#include <unordered_map>
#include <string>
#include <functional>

#include "testOCH.h"
#include "testFPSH.h"

using std::unordered_map;
using std::string;
using std::function;

// function<int(int, char**)> testFunc;
unordered_map<string, function<int(int, char*[])>> testCase = {
    {"OrbitalCamera", testOCH},
    {"FPSCamera", testFPSH}
};

int main(int argc, char *argv[]) 
{
    auto testFunc = testCase.at("FPSCamera");

    testFunc(argc, argv);

    return 0;
}