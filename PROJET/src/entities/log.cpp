#include "log.hpp"
#include "error.hpp"
#include <iostream>
#include <time.h>
#include <string>

using namespace std;

//Log object, used as a service to log events

ofstream Log::logFile("/nostal/nostal.log", ios::app);
int Log::currentLevel = 1;

Log::Log(int level)
{
    this->setLevel(level);

    if(Log::logFile)
    {
        this->addEntry(1, "-----------------------------------------------------");
        this->addEntry(1, "Nostalfront started and log was opened successfully !");

      /* try
        {
            // déclencher une exception par pointeur
            throw new Error(ErrorType::EXECUTION,"execution error!");

        }
        catch ( Error * error )
        {

            Error::addError(error);
        }*/
    }
    else
    {
        cerr << "Error : could not open log File !" << endl;
    }
}

Log::~Log()
{
    if(Log::logFile)
        Log::logFile.close();
}

int Log::getLevel()
{
    return Log::currentLevel;
}

Log* Log::setLevel(int level)
{
    Log::currentLevel = level;
    return this;
}

void Log::addEntry(int level, std::string entry)
{
    if(level <= Log::currentLevel)
    {
        time_t t;
        time (&t);
        string record = ctime(&t);
        record = record.substr(0,record.length()-1);
        record += " : (" + to_string(level) + ") " + entry;;
        Log::logFile << record << endl;
    }

    return;
}
