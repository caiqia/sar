#ifndef DEF_ERROR
#define DEF_ERROR

#include <string>
#include <vector>
#include "log.hpp"

//enum for possible errors
enum ErrorType
{
    CRITIQUE,
    MAJOR,
    EXECUTION,
    MINOR
};

class Error
{
public:
    Error(ErrorType type, std::string errorMessage, std::string errorCode);
    ~Error();
    static void addError(Error error);
    ErrorType getErrorType();
    Error* setErrorType(ErrorType type);
    std::string getErrorMessage();
    Error* setErrorMessage(std::string errorMessage);
    std::string getErrorCode();
    Error* setErrorCode(std::string errorCode);

  static std::vector<Error> errorqueue;

protected:
  ErrorType m_ErrorType;
  std::string m_ErrorMessage;
  std::string m_ErrorCode;

};

#endif
