#pragma once
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namepsace CAPA {
struct Dimensions
{
    Dimensions(std::size_t v, std::size_t m);
    Dimensions(std::size_t v, std::size_t a, std::size_t b, std::size_t c);
    std::size_t Vector;
    std::tuple<std::size_t, std::size_t, std::size_t> Matrix;
};

struct BenchmarkSet
{
    BenchmarkSet(json bench);
    std::map<std::string, std::map<std::string, std::vector<double>>> benchmarks;
};
}
