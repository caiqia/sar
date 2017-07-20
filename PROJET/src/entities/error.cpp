#include <string>
#include <vector>
#include "error.hpp"

using namespace std;

//Error Object, stores information about errors

std::vector<Error> Error::errorqueue;

//Init
Error::Error(ErrorType type, string errormessage, string errorcode )
{
this->setErrorType(type);
this->setErrorMessage(errormessage);
this->setErrorCode(errorcode);

    return;

}

Error::~Error()
{
  return;
}

void Error::addError(Error error)
{
    Error::errorqueue.push_back(error);

return;
}

Error* Error::setErrorType(ErrorType type)
{

    this->m_ErrorType=type;
    return this;

}

ErrorType Error::getErrorType()
{
  return this->m_ErrorType;
}

Error* Error::setErrorMessage(string errorMessage)
{
    this->m_ErrorMessage = errorMessage;
    return this;
}

string Error::getErrorMessage()
{
    return this->m_ErrorMessage;
}

Error* Error::setErrorCode(string errorCode)
{
    this->m_ErrorCode = errorCode;
    return this;
}

string Error::getErrorCode()
{
    return this->m_ErrorCode;
}
