#pragma once
#include <map>
#include <tuple>

namespace CAPA {

struct BenchmarkSet
{
    BenchmarkSet(std::string benchmarkLocation);
    bool Exists(std::string operation);
    double Speedup(std::string operation, std::size_t dimension);
    double Speedup(std::string operation);
    std::tuple<double, double> GetResult(std::string operation, std::size_t dimension);
    std::tuple<double, double> LowerUpper(std::string operation, std::size_t dimension);

    std::map<std::string, std::map<std::size_t, std::tuple<double, double>>> benchmarks;;
    static std::map<std::string, std::size_t> VectorFixtures;
    static std::map<std::string, std::size_t> MatrixFixtures;
};

} // End Namespace CAPA
