#ifndef CAPA_GENERICEXCEPTION_H
#define CAPA_GENERICEXCEPTION_H

#include <exception>
#include <string>

namespace CAPA
{

class GenericException : public std::exception
{
public:
    explicit GenericException(std::string desc);
    virtual ~GenericException() throw() {}
    virtual const char* what() const throw() override;

private:
    std::string description;
};

} // end namespace CAPA

#endif
