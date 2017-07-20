#ifndef DEF_ERROREXCEPTION
#define DEF_ERROREXCEPTION

#include <exception>
#include "error.hpp"

using namespace std;

class Error;

class Errorexception: public exception
{
public:
  Errorexception(ErrorType type, std::string errorMessage, std::string errorCode) throw();

  ~Errorexception()throw();

ErrorType getErrorType()const throw();
Errorexception* setErrorType(ErrorType type)throw();
std::string getErrorMessage()const throw();
Errorexception* setErrorMessage(std::string errorMessage)throw();
std::string getErrorCode()const throw();
Errorexception* setErrorCode(std::string errorCode)throw();

private:
  ErrorType m_ErrorType;
  std::string m_ErrorMessage;
  std::string m_ErrorCode;


};

#endif
