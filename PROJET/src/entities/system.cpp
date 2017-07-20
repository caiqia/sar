#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>
#include <sstream>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <sys/stat.h>


#include "system.hpp"
#include "log.hpp"
#include "game.hpp"
#include "error.hpp"
#include "../controllers/osController.hpp"
#include "../controllers/diskController.hpp"

//System object that represents a console available on the system

using namespace std;
using namespace libconfig;
namespace fs = boost::filesystem;

int System::idCount = 0; //Instance counter used to give id
std::map<int, System*> System::instances; //Vector of pointers to set publicly available all instances
std::vector<System*>  System::availableinstances;
std::vector<System*>  System::notavailableinstances;

//Init
System::System(const string& name, const string& displayName, const string& logoFile, const string& greyedLogoFile, const string& backgroundImage43, const string& backgroundImage169, const string& romPath, const string& savePath, const string& biosPath, const vector<string>& requiredBiosMD5, const vector<string>& atLeastOneBiosMD5, const vector<string>& optionalBiosMD5, const vector<string>& extensions, const string& launchCmd)
{
    this->id = System::idCount;

    if(requiredBiosMD5.size() > 0)
    {
        this->setRequiresBios(REQUIRES_BIOS);
    }
    else if(atLeastOneBiosMD5.size() > 0)
    {
        this->setRequiresBios(AT_LEAST_ONE_BIOS_REQUIRED);
    }
    else if(optionalBiosMD5.size() > 0)
    {
        this->setRequiresBios(BIOS_IS_OPTIONAL);
    }

    //System::idCount ++;
    this->setName(name);
    this->setDisplayName(displayName);
    this->setLogoFile(logoFile);
    this->setGreyedLogoFile(greyedLogoFile);
    this->setBackGround43(backgroundImage43);
    this->setBackGround169(backgroundImage169);
    this->setRomPath(romPath);
    this->setSavePath(savePath);
    this->setBiosPath(biosPath);
    this->setRequiredBiosMD5(requiredBiosMD5);
    this->setAtLeastOneBiosMD5(atLeastOneBiosMD5);
    this->setOptionalBiosMD5(optionalBiosMD5);
    this->setLaunchCmd(launchCmd);
    this->setExtensions(extensions);

    this->loadGames();

    this->checkAvailability();

    if(this->isAvailable())
    {
        System::availableinstances.push_back(this);
    }
    else
    {
        System::notavailableinstances.push_back(this);
    }

    //System::instances.insert(pair<int, System*>(this->getId(), this));



    Log::addEntry(2, "System " + this->getName() + " created successfully, found " + to_string(this->getNumberGamesAvailable()) + " game(s)");

    return;
}


//Remove all games pointed by the system
System::~System()
{
    for(auto& ent : this->getGames())
    {
        if(ent.second != nullptr)
        {
            delete(ent.second);
        }
    }
}

//Function used to create all system instances based on emul.cfg file
// Return 1 on success or 0 on failure
bool System::populate()
{
    // We delete all previously created instances of System
    if(System::instances.size() > 0)
    {
        for(auto& ent : System::instances)
        {
            if(ent.second != nullptr)
            {
                delete ent.second;
            }
        }
        System::instances.clear();
    }

    Config cfg;

    //We use libconfig to read our config file and parse it
    try
    {
        cfg.readFile("/nostal/emul.cfg");
    }
    catch(const FileIOException &fieoex)
    {
        Error::addError(Error(ErrorType::EXECUTION, "Nous n'avons pas pu lire le fichier de configuration", "I-0001"));
        Log::addEntry(1, "/!\\ Could not read system config file : /nostal/emul.cfg !");
        return 0;
    }
    catch(const ParseException &pex)
    {
        std::string file, line, error;
        file = pex.getFile();
        line = pex.getLine();
        error = pex.getError();
        Error::addError(Error(ErrorType::MAJOR, "Le fichier de configuration est corrompu", "I-0002"));
        Log::addEntry(1, "/!\\ Could not parse " + file + " : " + line + " - " + error);
        return 0;
     }

     const Setting& root = cfg.getRoot();

     bool firstLaunch;

     try
     {
         root.lookupValue("firstLaunch", firstLaunch);
     }
     catch(const SettingNotFoundException &nfex)
     {
        Error::addError(Error(ErrorType::MAJOR, "Nous n'avons pas pu déterminer si le system avait déjà été initialisé", "I-0003"));
        return 0;
     }

     if(!firstLaunch)
     {

         string rootRomPath, rootSavePath, rootBiosPath;

          try
          {
              root.lookupValue("romPath", rootRomPath);
          }
          catch(const SettingNotFoundException &nfex)
          {
              Error::addError(Error(ErrorType::MAJOR, "Nous n'avons pas pu déterminer l'emplacement des fichiers de jeux", "I-0004"));
              return 0;
          }

          try
          {
              root.lookupValue("savePath", rootSavePath);
          }
          catch(const SettingNotFoundException &nfex)
          {
              Error::addError(Error(ErrorType::MAJOR, "Nous n'avons pas pu déterminer l'emplacement des fichiers de sauvegarde", "I-0005"));
              return 0;
          }

          try
          {
              root.lookupValue("biosPath", rootBiosPath);
          }
          catch(const SettingNotFoundException &nfex)
          {
              Error::addError(Error(ErrorType::MAJOR, "Nous n'avons pas pu déterminer l'emplacement des fichiers de BIOS", "I-0006"));
              return 0;
          }

         struct stat sa;
         //if the folder doesn't exist we create it
         //TODO remove this logic to a controller
         if(stat(rootRomPath.c_str(), &sa) != 0 && !S_ISDIR(sa.st_mode))
         {
             string cmd = "mkdir " + rootRomPath;
             system(cmd.c_str());
         }

         if(stat(rootSavePath.c_str(), &sa) != 0 && !S_ISDIR(sa.st_mode))
         {
             string cmd = "mkdir " + rootSavePath;
             system(cmd.c_str());
         }

         if(stat(rootBiosPath.c_str(), &sa) != 0 && !S_ISDIR(sa.st_mode))
         {
             string cmd = "mkdir " + rootBiosPath;
             system(cmd.c_str());
         }
         if(root.exists("config") && root["config"].exists("systems"))
         {
             const Setting &systems = root["config"]["systems"];
             int count = systems.getLength();

             //We go through all the systems in the config file
             for(int i = 0; i < count; ++i)
             {
                 const Setting& system = systems[i];

                 bool hasAllParam = true;
                 string name, displayName, launchCmd;
                 vector<string> requiredBIOSMD5, atLeastOneBIOSMD5, optionalBIOSMD5, extensions;

                 if(system.exists("name"))
                 {
                     system.lookupValue("name", name);
                 }
                 else
                 {
                     hasAllParam = false;
                     Error::addError(Error(ErrorType::MAJOR, "Nous ne pouvons pas accéder au paramètre \"name\" d'un système dans le fichier de configuration", "I-0008"));
                     return 0;
                 }

                 if(system.exists("displayName"))
                 {
                     system.lookupValue("displayName", displayName);
                 }
                 else
                 {
                     hasAllParam = false;
                     Error::addError(Error(ErrorType::MAJOR, "Nous ne pouvons pas accéder au paramètre \"displayName\" de \"" + name + "\" dans le fichier de configuration", "I-0009"));
                     return 0;
                 }

                 if(system.exists("launchCmd"))
                 {
                     system.lookupValue("launchCmd", launchCmd);
                 }
                 else
                 {
                     hasAllParam = false;
                     Error::addError(Error(ErrorType::MAJOR, "Nous ne pouvons pas accéder au paramètre \"launchCmd\" de \"" + displayName + "\" dans le fichier de configuration", "I-0010"));
                     return 0;
                 }

                 if(system.exists("requiredBIOSMD5"))
                 {
                     const Setting& requiredBiosMD5Setting = system["requiredBIOSMD5"];
                     for(int j = 0; j < requiredBiosMD5Setting.getLength(); ++j)
                     {
                         requiredBIOSMD5.push_back(requiredBiosMD5Setting[j]);
                     }
                 }
                 else
                 {
                     hasAllParam = false;
                     Error::addError(Error(ErrorType::MAJOR, "Nous ne pouvons pas accéder au paramètre \"requiredBIOSMD5\" de \"" + displayName + "\" dans le fichier de configuration", "I-0011"));
                     return 0;
                 }

                 if(system.exists("atLeastOneBIOSMD5"))
                 {
                     const Setting& atLeastOneBIOSMD5Setting = system["atLeastOneBIOSMD5"];
                     for(int j = 0; j < atLeastOneBIOSMD5Setting.getLength(); ++j)
                     {
                         atLeastOneBIOSMD5.push_back(atLeastOneBIOSMD5Setting[j]);
                     }
                 }
                 else
                 {
                     hasAllParam = false;
                     Error::addError(Error(ErrorType::MAJOR, "Nous ne pouvons pas accéder au paramètre \"atLeastOneBIOSMD5\" de \"" + displayName + "\" dans le fichier de configuration", "I-0012"));
                     return 0;
                 }

                 if(system.exists("optionalBIOSMD5"))
                 {
                     const Setting& optionalBIOSMD5Setting = system["optionalBIOSMD5"];
                     for(int j = 0; j < optionalBIOSMD5Setting.getLength(); ++j)
                     {
                         optionalBIOSMD5.push_back(optionalBIOSMD5Setting[j]);
                     }
                 }
                 else
                 {
                     hasAllParam = false;
                     Error::addError(Error(ErrorType::MAJOR, "Nous ne pouvons pas accéder au paramètre \"optionalBIOSMD5\" de \"" + displayName + "\" dans le fichier de configuration", "I-0013"));
                     return 0;
                 }

                 if(system.exists("extensions"))
                 {
                     const Setting& extensionsSetting = system["extensions"];
                     for(int j = 0; j < extensionsSetting.getLength(); ++j)
                     {
                         extensions.push_back(extensionsSetting[j]);
                     }
                 }
                 else
                 {
                     hasAllParam = false;
                     Error::addError(Error(ErrorType::MAJOR, "Nous ne pouvons pas accéder au paramètre \"extensions\" de \"" + displayName + "\" dans le fichier de configuration", "I-0014"));
                     return 0;
                 }

                string logoFile, greyedLogoFile, bg43, bg169, romPath, savePath, biosPath;

                logoFile = "/nostal/IMG/" + name + "/" + name + "_LOGO.png";
                greyedLogoFile = "/nostal/IMG/" + name + "/" + name + "_LOGO_G.png";
                bg43 = "/nostal/IMG/" + name + "/" + name + "_BG_4-3.png";
                bg169 = "/nostal/IMG/" + name + "/" + name + "_BG_16-9.png";
                romPath = rootRomPath + "/" + name;
                savePath = rootSavePath + "/" + name;
                biosPath = rootBiosPath + "/" + name;

                if(hasAllParam)
                {
                    System* sys = new System(name, displayName, logoFile, greyedLogoFile, bg43, bg169, romPath, savePath, biosPath, requiredBIOSMD5, atLeastOneBIOSMD5, optionalBIOSMD5, extensions, launchCmd);
                }

             }
             for(auto &it : System::availableinstances)
             {
                  if(it != nullptr)
                  {
                      System::instances.insert(pair<int, System*>(System::idCount, it));

                  }
                  System::idCount ++;

             }

             for(auto &itt : System::notavailableinstances)
             {

                  if(itt != nullptr)
                  {
                       System::instances.insert(pair<int, System*>(System::idCount, itt));

                  }
                  System::idCount ++;

             }

         }
         else
         {
             Error::addError(Error(ErrorType::MAJOR, "Nous ne pouvons pas accéder à la configuration des systèmes", "I-0007"));
         }


    }
return 1;
}

//Function used to load all games available for current system
int System::loadGames()
{
    //We remove all previously stored games
    this->clearGames();

    int gameCount = 0;

    if(!fs::exists(this->getSavePath()))
    {
        DiskController::createDirectory(this->getSavePath());
    }

    if(!fs::exists(this->getRomPath()))
    {
        DiskController::createDirectory(this->getRomPath());
    }

    if(this->getReqiresBios() > ACCEPTS_NO_BIOS  && !fs::exists(this->getBiosPath()))
    {
        DiskController::createDirectory(this->getBiosPath());
    }

    else
    {
        for(auto it : recursive_directory_range(this->getRomPath()))
        {
            if(fs::is_regular_file(it.path()))
            {
                bool found = false;

                for(const auto& ent : this->getExtensions())
                {
                    if(it.path().extension().string().find(ent) != string::npos)
                    {
                        found = true;
                    }
                }

                if(found)
                {
                    this->addGame(new Game(this, it.path().string()));

                    ++gameCount;
                }
            }
        }
    }

    this->setNumberGamesAvailable(gameCount);
    this->sortGames();

    return gameCount;
}

int System::getId() const
{
    return this->id;
}

string System::getName() const
{
    return this->m_name;
}

void System::setName(const string& name)
{
    this->m_name = name;
    return;
}

string System::getDisplayName() const
{
    return this->m_displayName;
}

void System::setDisplayName(const string& displayName)
{
    this->m_displayName = displayName;
    return;
}

string System::getLogoFile() const
{
    return this->m_logoFile;
}

void System::setLogoFile(const string& file)
{
    this->m_logoFile = file;
    return;
}

string System::getGreyedLogoFile() const
{
    return this->m_greyedLogoFile;
}

System* System::setGreyedLogoFile(const string& file)
{
    this->m_greyedLogoFile = file;
    return this;
}



string System::getBackground43() const
{
    return this->m_background43;
}

void System::setBackGround43(const string& file)
{
    this->m_background43 = file;
    return;
}

string System::getBackground169() const
{
    return this->m_background169;
}

void System::setBackGround169(const string& file)
{
    this->m_background169 = file;
    return;
}

string System::getRomPath() const
{
    return this->m_romPath;
}

System* System::setRomPath(const string& path)
{
    this->m_romPath = path;
    return this;
}

string System::getSavePath() const
{
    return this->m_savePath;
}

System* System::setSavePath(const string& path)
{
    this->m_savePath = path;
    return this;
}

string System::getBiosPath() const
{
    return this->m_biosPath;
}

System* System::setBiosPath(const string& path)
{
    this->m_biosPath = path;
    return this;
}

std::vector<std::string> System::getRequiredBiosMD5() const
{
    return this->m_requiredBiosMD5;
}

System*System::setRequiredBiosMD5(const std::vector<std::string>& requiredBiosMD5)
{
    this->m_requiredBiosMD5 = requiredBiosMD5;
    return this;
}

std::vector<std::string> System::getAtLeastOneBiosMD5() const
{
    return this->m_atLeastOneBiosMD5;
}

System* System::setAtLeastOneBiosMD5(const std::vector<std::string>& atLeastOneBiosMD5)
{
    this->m_atLeastOneBiosMD5 = atLeastOneBiosMD5;
    return this;
}

std::vector<std::string> System::getOptionalBiosMD5() const
{
    return this->m_optionalBiosMD5;
}

System* System::setOptionalBiosMD5(const std::vector<std::string>& optionalBiosMD5)
{
    this->m_optionalBiosMD5 = optionalBiosMD5;
    return this;
}

RequiresBios System::getReqiresBios() const
{
    return this->m_requiresBios;
}

System* System::setRequiresBios(const RequiresBios& requiresBios)
{
    if(requiresBios > this->m_requiresBios)
    {
        this->m_requiresBios = requiresBios;
    }

    return this;
}

vector<string> System::getExtensions() const
{
    return this->m_extensions;
}

System* System::setExtensions(const vector<string>& extensions)
{
    this->m_extensions = extensions;
    return this;
}

string System::getLaunchCmd() const
{
    return this->m_launchCmd;
}

System* System::setLaunchCmd(const string& cmd)
{
    this->m_launchCmd = cmd;
    return this;
}

map <int, Game*> System::getGames() const
{
    return this->m_availableGames;
}

Game* System::getGame(const int& id) const
{
    if(this->m_availableGames.count(id) > 0)
    {
        return this->m_availableGames.at(id);
    }
    else
    {
        return nullptr;
    }
}

System* System::clearGames()
{
    this->m_availableGames.clear();
    return this;
}

System* System::addGame(Game* game)
{
    this->m_availableGames.insert(pair<int, Game*> (this->getNumberGamesAvailable(), game));
    this->setNumberGamesAvailable(this->getNumberGamesAvailable() + 1);
    return this;
}

System* System::sortGames()
{
    //sort(this->m_availableGames.begin(), this->m_availableGames.end(), pointerCompare);

    for(int i=0; i< this->m_availableGames.size(); i++)
    {
        for(int j=i+1; j<this->m_availableGames.size(); j++)
        {
            if(this->pointerCompare(this->m_availableGames[j],this->m_availableGames[i]))
            {
                auto tmp = this->m_availableGames[i];
                this->m_availableGames[i] = this->m_availableGames[j];
                this->m_availableGames[j] = tmp;
            }
        }
    }

    return this;
}

bool System::pointerCompare(const Game* a, const Game* b)
{
    return (*a < *b);
}

int System::getNumberGamesAvailable() const
{
    return this->m_numberGamesAvailable;
}

System* System::setNumberGamesAvailable(const int& number)
{
    this->m_numberGamesAvailable = number;
    return this;
}

bool System::hasGames() const
{
    return this->m_hasGames;
}

bool System::hasCorrectBIOS() const
{
    return this->m_hasCorrectBIOS;
}

bool System::isAvailable() const
{
    return this->m_available;
}

System* System::checkAvailability()
{
    if(this->getNumberGamesAvailable() > 0)
    {
        this->m_hasGames = true;
    }
    else
    {
        this->m_hasGames = false;
    }

    if(this->getRequiredBiosMD5().size() > 0 || this->getAtLeastOneBiosMD5().size() > 0)
    {
        if(fs::exists(this->getBiosPath()))
        {
            int foundRequired = 0;
            int foundAtLeast = 0;
            for(auto it : recursive_directory_range(this->getBiosPath()))
            {
                if(fs::is_regular_file(it.path()))
                {
                    string md5 = DiskController::getFileMD5(this->getBiosPath() + "/" + it.path().filename().string());

                    for(const auto& ent : this->getRequiredBiosMD5())
                    {
                        if(md5 == ent)
                        {
                            ++foundRequired;
                        }
                    }

                    for(const auto& ent : this->getAtLeastOneBiosMD5())
                    {
                        if(md5 == ent)
                        {
                            ++foundAtLeast;
                        }
                    }
                }
            }

            if(foundAtLeast == 0 && this->getAtLeastOneBiosMD5().size() > 0)
            {
                this->m_hasCorrectBIOS = false;
            }
            else if(foundRequired != this->getRequiredBiosMD5().size())
            {
                this->m_hasCorrectBIOS = false;
            }
            else
            {
                this->m_hasCorrectBIOS = true;
            }

        }
        else
        {
            DiskController::createDirectory(this->getBiosPath());
            this->m_hasCorrectBIOS = false;
        }
    }
    else
    {
        this->m_hasCorrectBIOS = true;
    }

    if(this->hasGames() && this->hasCorrectBIOS())
    {
        this->m_available = true;
    }
    else
    {
        this->m_available = false;
    }

    return this;
}

System* System::setSelectedGame(const int& selectedGame)
{
    this->m_selectedGame = selectedGame;
    return this;
}

int System::getSelectedGame() const
{
    return this->m_selectedGame;
}

Game* System::getSelectedGameEntity() const
{
    return this->getGame(this->getSelectedGame());
}
