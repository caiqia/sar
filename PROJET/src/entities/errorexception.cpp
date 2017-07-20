#include <string>
#include <exception>
#include <iostream>
#include "errorexception.hpp"
#include "error.hpp"

using namespace std;

Errorexception::Errorexception(ErrorType type, string errormessage, string errorcode ) throw()
{
  this->setErrorType(type);
  this->setErrorMessage(errormessage);
  this->setErrorCode(errorcode);

  return;
}

Errorexception::~Errorexception() throw()
{
  return;
}


ErrorType Errorexception::getErrorType()const throw()
{
  return this->m_ErrorType;
}

Errorexception* Errorexception::setErrorType(ErrorType type) throw()
{
  this->m_ErrorType = type;
  return this;
}

string Errorexception::getErrorMessage()const throw()
{
  return this->m_ErrorMessage;
}

Errorexception* Errorexception::setErrorMessage(string errormessage) throw()
{
  this->m_ErrorMessage = errormessage;
  return this;
}

string Errorexception::getErrorCode()const throw()
{
  return this->m_ErrorCode;
}

Errorexception* Errorexception::setErrorCode(string errorcode) throw()
{
  this->m_ErrorCode = errorcode;
}
