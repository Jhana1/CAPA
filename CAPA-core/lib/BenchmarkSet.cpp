#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "nlohmann/json.hpp"
#include "CAPA/BenchmarkSet.h"

namespace CAPA {

Dimensions::Dimensions(std::size_t v, std::size_t m)
{
    Vector = v;
    Matrix = std::make_tuple(m, m, m);
}

Dimensions::Dimensions(std::size_t v, std::size_t a, std::size_t b, std::size_t c)
{
    Vector = v;
    Matrix = std::make_tuple(a, b, c);
}

std::map<std::string, Dimensions> Fixtures = {
    {"F0", Dimensions(1, 2)},
    {"F1", Dimensions(4, 4)},
    {"F2", Dimensions(16, 8)},
    {"F3", Dimensions(64, 16)},
    {"F4", Dimensions(256, 32)},
    {"F5", Dimensions(1024, 64)},
    {"F6", Dimensions(4096, 128)},
    {"F7", Dimensions(16384, 256)},
    {"F8", Dimensions(65536, 512)},
    {"F9", Dimensions(262144, 1024)},
    {"F10", Dimensions(1048576, 2048)},
    {"F11", Dimensions(4194304, 4096)},
    {"F12", Dimensions(16777216, 8192)}
};
 
BenchmarkSet::BenchmarkSet(std::string benchmarkLocation)
{
    std::ifstream file(benchmarkLocation);

    if (!file.good())
    {

        return;
    }

    nlohmann::json j;
    file >> j;

    std::map<std::string, std::map<std::string, std::vector<double>>> tmp;
    for (auto &it : j["benchmarks"])
    {
        std::string name = it["name"];
        std::string fixture = it["fixture"];
        std::vector<double> runs;
        for (auto &run : it["runs"])
        {
            runs.push_back((double) run["duration"]);
        }
        std::sort(runs.begin(), runs.end());
        double median = runs[runs.size()/2];
        
        if (name.compare(0,4,"Host") == 0)
        {
            std::string test = name.substr(4);
            auto &tup = benchmarks[test][fixture];
            std::get<0>(tup) = median;
        }
        else
        {
            std::string test = name.substr(6);
            // Check if still using old Onload/Offload specifier
            // and remove if so
            
            auto pos =  test.find("NoOnload");
            if (pos != std::string::npos) { test.erase(pos); }
            pos = test.find("Onload");
            if (pos != std::string::npos) { test.erase(pos); }

            auto &tup = benchmarks[test][fixture];
            std::get<1>(tup) = median;
        }
    }

    for (auto &it : benchmarks)
    {
        std::cout << it.first << ": " << std::endl;
        for (auto &ip : it.second)
        {
            std::cout << ip.first << ": " << std::get<0>(ip.second) << " "
                                          << std::get<1>(ip.second) << std::endl;
        }
    }
}



} // Namespace Capa


