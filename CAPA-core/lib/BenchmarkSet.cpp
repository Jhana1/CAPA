#include <iostream>
#include <map>
#include "nlohmann/json.hpp"

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

BenchmarkSet::BenchmarkSet(json bench)
{
    for (auto &it : j["benchmarks"])
    {
        std::string name = it["name"];
        std::string fixture = it["fixture"];
        std::vector<double> runs;
        for (auto &run : it)
        {
            runs.push_back((double) run["duration"]);
        }
        benchmarks[name][fixture] = runs;
    }
}



} // Namespace Capa


