#include "CAPA/GenericException.h"

#include <utility>

using namespace CAPA;

GenericException::GenericException(std::string desc)
    : description(std::move(desc))
{
}

const char *GenericException::what() const throw()
{
    return description.c_str();
}
