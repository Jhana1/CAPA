#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "nlohmann/json.hpp"
#include "CAPA/BenchmarkSet.h"

namespace CAPA {

BenchmarkSet::BenchmarkSet(std::string benchmarkLocation)
{
    std::ifstream file(benchmarkLocation);

    if (!file.good())
    {
        // TODO: Fill with generic data
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
            if (test.find("Matrix") != std::string::npos)
            {
                auto &tup = benchmarks[test][MatrixFixtures[fixture]];
                std::get<0>(tup) = median;
            }
            else
            {
                auto &tup = benchmarks[test][VectorFixtures[fixture]];
                std::get<0>(tup) = median;
            }
        }
        else if (name.compare(0,6, "Device") == 0)
        {
            std::string test = name.substr(6);
            // Check if still using old Onload/Offload specifier
            // and remove if so
            auto pos =  test.find("NoOnload");
            if (pos != std::string::npos) { test.erase(pos); }
            pos = test.find("Onload");
            if (pos != std::string::npos) { test.erase(pos); }

            // Check if Matrix or Vector Operation
            if (test.find("Matrix") != std::string::npos) {
                auto &tup = benchmarks[test][MatrixFixtures[fixture]];
                std::get<1>(tup) = median;
            }
            else
            {
                auto &tup = benchmarks[test][VectorFixtures[fixture]];
                std::get<1>(tup) = median;
            }
        }
    }

    // Printing for debugging purposes
    /*
    for (auto &it : benchmarks)
    {
        std::cout << it.first << ": " << std::endl;
        for (auto &ip : it.second)
        {
            std::cout << ip.first << ": " << std::get<0>(ip.second) << " ### "
                                          << std::get<1>(ip.second) << std::endl;
        }
    }
    */
}

BenchmarkSet::BenchmarkSet(const BenchmarkSet &rhs) 
{
    benchmarks = rhs.benchmarks;
}

BenchmarkSet BenchmarkSet::operator= (const BenchmarkSet &rhs)
{
    if (this != &rhs)
    {
        benchmarks = rhs.benchmarks;
    }
    return *this;
}
BenchmarkSet::~BenchmarkSet() {
    benchmarks.clear();
    VectorFixtures.clear();
    MatrixFixtures.clear();
}

double BenchmarkSet::Speedup(std::string operation)
{
    double host = 0;
    double device = 0;
    // No Size given, average out performance over all test cases
    // not exactly fair, but who cares.
    for (auto &itr : benchmarks[operation])
    {
        std::cout << std::get<0>(itr.second) << " " << std::get<1>(itr.second) << std::endl;
        host += std::get<0>(itr.second);
        device += std::get<1>(itr.second);
    }
    return host/device;
}

double BenchmarkSet::Speedup(std::string operation, std::size_t dimension)
{
    if (!dimension) { return Speedup(operation); }
    auto result = benchmarks[operation].lower_bound(dimension);
    double host   = std::get<0>(result->second);
    double device = std::get<1>(result->second);
    return host/device;
}

std::tuple<double, double> BenchmarkSet::LowerUpper(std::string operation, std::size_t dimension)
{
    if (!dimension) 
    {
        auto lower = benchmarks[operation].begin();
        auto upper = --benchmarks[operation].end();

        double host_lower = std::get<0>(lower->second);
        double device_lower = std::get<1>(lower->second);
        double host_upper = std::get<0>(upper->second);
        double device_upper = std::get<1>(upper->second);

        return std::make_tuple(host_lower/device_lower, host_upper/device_upper);
    }
    
    auto lower = benchmarks[operation].lower_bound(dimension);
    if (lower == benchmarks[operation].end()) {lower--; lower--;}
    if (lower != benchmarks[operation].begin()) {lower--;}
    double host_lower = std::get<0>(lower->second);
    double device_lower = std::get<1>(lower->second);
    double host_upper = std::get<0>((++lower)->second);
    double device_upper = std::get<1>((lower)->second);
    return std::make_tuple(host_lower/device_lower, host_upper/device_upper);
}

bool BenchmarkSet::Exists(std::string operation)
{
    return benchmarks.find(operation) != benchmarks.end();
}

std::tuple<double, double> BenchmarkSet::GetResult(std::string operation, std::size_t dimension)
{
    auto result = benchmarks[operation].lower_bound(dimension);
    return result->second;
}

std::map<std::string, std::size_t> BenchmarkSet::VectorFixtures = {
    {"F0", 1},
    {"F1", 4},
    {"F2", 16},
    {"F3", 64},
    {"F4", 256},
    {"F5", 1024},
    {"F6", 4096},
    {"F7", 16384},
    {"F8", 65536},
    {"F9", 262144},
    {"F10", 1048576},
    {"F11", 4194304},
    {"F12", 16777216}
};

std::map<std::string, std::size_t> BenchmarkSet::MatrixFixtures = {
    {"F0", 2},
    {"F1", 4},
    {"F2", 8},
    {"F3", 16},
    {"F4", 32},
    {"F5", 64},
    {"F6", 128},
    {"F7", 256},
    {"F8", 512},
    {"F9", 1024},
    {"F10", 2048},
    {"F11", 4096},
    {"F12", 8192}
};


} // Namespace Capa


