#ifndef DEF_ERRORCONTROLLER
#define DEF_ERRORCONTROLLER

#include <vector>
#include <string>

#include "abstractController.hpp"
#include "../entities/error.hpp"
#include "../entities/log.hpp"

class Controller;

class ErrorController : public AbstractController
{
public:
    ErrorController(Controller* controller);
    ~ErrorController();

    virtual ErrorController* control();
    virtual ErrorController* inputControl();

    Error* getError();
    ErrorController* setError(Error* error);
    ErrorController* removeFirstError();

protected:
    Error* m_error;

};

#endif
