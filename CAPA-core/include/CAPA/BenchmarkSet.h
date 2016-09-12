#pragma once
#include <map>
#include <tuple>

namespace CAPA {

struct BenchmarkSet
{
    BenchmarkSet(std::string benchmarkLocation);
    double Speedup(std::string operation, std::size_t dimension);
    double Speedup(std::string operation);
    std::tuple<double, double> GetResult(std::string operation, std::size_t dimension);

    std::map<std::string, std::map<std::size_t, std::tuple<double, double>>> benchmarks;;
};

} // End Namespace CAPA
