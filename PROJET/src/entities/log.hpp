#ifndef DEF_LOG
#define DEF_LOG

#include <fstream>
#include <iostream>



class Log
{
public:
    Log(int level);
    ~Log();

    int getLevel();
    Log* setLevel(int level);

    static void addEntry(int level, std::string entry);

protected:
    static std::ofstream logFile;
    static int currentLevel;
};

#endif
