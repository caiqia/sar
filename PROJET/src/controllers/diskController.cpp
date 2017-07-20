#include "diskController.hpp"
#include "controller.hpp"
#include "osController.hpp"
#include "../entities/log.hpp"
#include "../entities/error.hpp"

#include <string>
#include <iostream>
#include <regex>
#include <fstream>
#include <libconfig.h++>

using namespace std;
using namespace libconfig;
using namespace boost::filesystem;

DiskController::DiskController(Controller* controller)
: AbstractController(controller, DISK_CONTROLLER)
{

}

DiskController::~DiskController()
{

}

DiskController* DiskController::control()
{
    AbstractController::control();
    return this;
}
DiskController* DiskController::inputControl()
{
    AbstractController::inputControl();
    return this;
}

int DiskController::createDataPartition()
{
    string cmd = "parted /dev/mmcblk0 print";
    string output, start, end;

    OSController* osController = static_cast<OSController*> (this->getController()->getController(OS_CONTROLLER));

    if(!this->getController()->getDevMode() && osController != nullptr)
    {
        output = osController->execCmd(cmd.c_str());
    }

    regex rgxStart("2[ ]+[0-9]+.[0-9]+[a-zA-Z]+[ ]+([0-9]+.[0-9]+[a-zA-Z]+)");
    regex rgxEnd("\/dev\/mmcblk0: ([0-9]+.[0-9]+[a-zA-Z]+)");
    smatch match;

    regex_search(output, match, rgxStart);
    start = match[1];

    regex_search(output, match, rgxEnd);
    end = match[1];

    cmd = "parted /dev/mmcblk0 mkpart primary ext2 " + start + " " + end;

    if(!this->getController()->getDevMode() && osController != nullptr)
    {
        output = osController->execCmd(cmd.c_str());
        //TODO check output
    }
    else
    {
        cout << "[DEV] Cmd callded : " << cmd << endl;
    }


    return 0;
}

int DiskController::createDataFileSystem()
{
    string cmd = "mkfs.ext2 /dev/mmcblk0p3";

    OSController* osController = static_cast<OSController*> (this->getController()->getController(OS_CONTROLLER));

    if(!this->getController()->getDevMode() && osController != nullptr)
    {
        string output = osController->execCmd(cmd.c_str());

        //TODO check output
    }
    else
    {
        cout << "[DEV] Cmd callded : " << cmd << endl;
    }


    return 0;
}

int DiskController::addDataPartitionToFstab()
{
    if(!this->getController()->getDevMode())
    {
        std::ofstream fstab("/etc/fstab", ios::out | ios::app);
        if(fstab)
        {
            fstab << "/dev/mmcblk0p3 /data ext2 defaults 1 2\n";
            fstab.close();
        }
        else
        {
            cout << "could not edit fstab" << endl;
            return 1;
        }
    }
    else
    {
        cout << "[DEV] writing to fstab" << endl;
    }

    return 0;
}

int DiskController::mountDataPartition()
{
    string cmd = "mount /dev/mmcblk0p3";

    OSController* osController = static_cast<OSController*> (this->getController()->getController(OS_CONTROLLER));

    if(!this->getController()->getDevMode() && osController != nullptr)
    {
        string output = osController->execCmd(cmd.c_str());
        //TODO check output
    }
    else
    {
        cout << "[DEV] Cmd called : " << cmd << endl;
    }

    return 0;
}

int DiskController::createPublicRessourceFolders()
{
    Config cfg;

    try
    {
        cfg.readFile("/nostal/emul.cfg");
    }
    catch(const FileIOException &fieoex)
    {
        Log::addEntry(1, "/!\\ Could not read system config file : /nostal/emul.cfg !");
        return 1;
    }
    catch(const ParseException &pex)
    {
        std::string file, line, error;
        file = pex.getFile();
        line = pex.getLine();
        error = pex.getError();
        Log::addEntry(1, "/!\\ Could not parse " + file + " : " + line + " - " + error);
        return 1;
     }

     const Setting& root = cfg.getRoot();

     string romPath, biosPath, savePath;

     bool found = true;

     try
     {
         root.lookupValue("romPath", romPath);
     }
     catch(const SettingNotFoundException &nfex)
     {
         cout << "Could not find romPath param in emul.cfg" << endl;
         found = false;
     }

     try
     {
         root.lookupValue("savePath", savePath);
     }
     catch(const SettingNotFoundException &nfex)
     {
         cout << "Could not find savePath param in emul.cfg" << endl;
         found = false;
     }

     try
     {
         root.lookupValue("biosPath", biosPath);
     }
     catch(const SettingNotFoundException &nfex)
     {
         cout << "Could not find biosPath param in emul.cfg" << endl;
         found = false;
     }

     OSController* osController = static_cast<OSController*> (this->getController()->getController(OS_CONTROLLER));

     if(found && osController != nullptr)
     {
         string cmd = "mkdir \"" + romPath + "\"";
         string output;


         if(!this->getController()->getDevMode())
         {
             output = osController->execCmd(cmd.c_str());
             //TODO check output
         }
         else
         {
             cout << "[DEV] Cmd called : " << cmd << endl;
         }


         cmd = "mkdir \"" + savePath + "\"";

         if(!this->getController()->getDevMode())
         {
             output = osController->execCmd(cmd.c_str());
         }
         else
         {
             cout << "[DEV] Cmd called : " << cmd << endl;
         }


         cmd = "mkdir \"" + biosPath + "\"";

         if(!this->getController()->getDevMode())
         {
             output = osController->execCmd(cmd.c_str());
         }
         else
         {
             cout << "[DEV] Cmd called : " << cmd << endl;
         }

         return 0;
     }
     else
     {
         //Major error Or perhaps first an execution one with a fallback to default param...
         return 1;
     }
}

bool DiskController::createDirectory(const string& directoryPath)
{
    const char* dirPath = directoryPath.c_str();
    path dir(dirPath);

    bool created = true;

    try
    {
        created = create_directories(dir);
    }
    catch(const filesystem_error&)
    {
        Error::addError(Error(ErrorType::MINOR, "Erreur lors de la création du dossier : " + directoryPath, "FS-0002"));
    }

    if(!created)
    {
        Error::addError(Error(ErrorType::MINOR, "Impossible de créer le dossier : " + directoryPath, "FS-0001"));
        return false;
    }

    return true;
}

std::string DiskController::getFileMD5(const std::string& filePath)
{
    string cmd = "md5sum " + filePath;
    string output;
    output = OSController::execCmd(cmd.c_str());

    if(output.find("md5sum:") != string::npos)
    {
        return "";
    }
    else
    {
        output.resize(32);
        return output;
    }
}
