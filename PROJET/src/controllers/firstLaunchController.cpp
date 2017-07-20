#include "firstLaunchController.hpp"
#include "controller.hpp"
#include "diskController.hpp"
#include "osController.hpp"
#include "../entities/log.hpp"

#include <libconfig.h++>
#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;
using namespace libconfig;

FirstLaunchController::FirstLaunchController(Controller* controller)
: AbstractController(controller, FIRST_LAUNCH_CONTROLLER)
{

    Config cfg;

    try
    {
        cfg.readFile("/nostal/emul.cfg");
    }
    catch(const FileIOException &fieoex)
    {
        Log::addEntry(1, "/!\\ Could not read system config file : /nostal/emul.cfg !");
        return;
    }
    catch(const ParseException &pex)
    {
        std::string file, line, error;
        file = pex.getFile();
        line = pex.getLine();
        error = pex.getError();
        Log::addEntry(1, "/!\\ Could not parse " + file + " : " + line + " - " + error);
        return;
     }

     const Setting& root = cfg.getRoot();

     try
     {
         bool firstLaunch;
         root.lookupValue("firstLaunch", firstLaunch);
         this->setFirstLaunch(firstLaunch);
     }
     catch(const SettingNotFoundException &nfex)
     {
         cout << "Could not find firstLaunch param in emul.cfg" << endl;
     }
}

FirstLaunchController::~FirstLaunchController()
{

}

FirstLaunchController* FirstLaunchController::control()
{
    AbstractController::control();

    this->getController()->setParam("firstLaunch", this->isFirstLaunch() ? "true" : "false");

    if(this->isFirstLaunch())
    {
        switch (this->getCurrentStage()) {

            case FirstLaunchStages::WELCOME :
                this->getController()->setParam("firstLaunchStage", "welcome");
                break;

            case FirstLaunchStages::ADD_PART :
                this->getController()->setParam("firstLaunchStage", "add_partition");
                this->addPartition();
                break;

            case FirstLaunchStages::CREATE_FS :
                this->getController()->setParam("firstLaunchStage", "create_fs");
                this->createFs();
                break;

            case FirstLaunchStages::INSERT_FSTAB :
                this->getController()->setParam("firstLaunchStage", "insert_fstab");
                this->insertFstab();
                break;

            case FirstLaunchStages::CREATE_DATA_FOLDER :
                this->getController()->setParam("firstLaunchStage", "create_data_folder");
                this->createDataFolder();
                break;

            case FirstLaunchStages::MOUNT_PART :
                this->getController()->setParam("firstLaunchStage", "mount_part");
                this->mountPart();
                break;
            case FirstLaunchStages::CREATE_USER_FOLDERS :
                this->getController()->setParam("firstLaunchStage", "create_user_folder");
                this->createDestinationFolders();
                break;
            case FirstLaunchStages::FINISH :
                this->getController()->setParam("firstLaunchStage", "finish");
                this->finish();
                break;
            case FirstLaunchStages::RESTART :
                this->getController()->setParam("firstLaunchStage", "restart");
                this->restart();
                break;
            default :
                this->getController()->setParam("firstLaunchStage", "error");
        }
    }
    return this;
}

FirstLaunchController* FirstLaunchController::inputControl()
{
    AbstractController::inputControl();

    if(this->isFirstLaunch())
    {
        switch (this->getCurrentStage()) {
            case FirstLaunchStages::WELCOME :
                this->welcome();
                break;
            }
    }

    return this;
}

FirstLaunchController* FirstLaunchController::firstLaunch()
{
    DiskController* diskController = static_cast<DiskController*>(this->getController()->getController(DISK_CONTROLLER));

    if(diskController != nullptr)
    {
        diskController->createDataPartition();
    }

    return this;
}

FirstLaunchController* FirstLaunchController::setFirstLaunch(bool firstLaunch)
{
    this->m_firstLaunch = firstLaunch;
    return this;
}

bool FirstLaunchController::isFirstLaunch()
{
    return this->m_firstLaunch;
}

FirstLaunchStages FirstLaunchController::getCurrentStage()
{
    return this->m_currentStage;
}

FirstLaunchController* FirstLaunchController::setCurrentStage(FirstLaunchStages stage)
{
    this->m_currentStage = stage;
    return this;
}

FirstLaunchController* FirstLaunchController::gotToNextStage()
{

    switch (this->getCurrentStage()) {
        case FirstLaunchStages::WELCOME :
            this->setCurrentStage(FirstLaunchStages::ADD_PART);
            break;
        case FirstLaunchStages::ADD_PART :
            this->setCurrentStage(FirstLaunchStages::CREATE_FS);
            break;
        case FirstLaunchStages::CREATE_FS :
            this->setCurrentStage(FirstLaunchStages::INSERT_FSTAB);
            break;
        case FirstLaunchStages::INSERT_FSTAB :
            this->setCurrentStage(FirstLaunchStages::CREATE_DATA_FOLDER);
            break;
        case FirstLaunchStages::CREATE_DATA_FOLDER :
            this->setCurrentStage(FirstLaunchStages::MOUNT_PART);
            break;
        case FirstLaunchStages::MOUNT_PART :
            this->setCurrentStage(FirstLaunchStages::CREATE_USER_FOLDERS);
            break;
        case FirstLaunchStages::CREATE_USER_FOLDERS :
            this->setCurrentStage(FirstLaunchStages::FINISH);
            break;
        case FirstLaunchStages::FINISH :
            this->setCurrentStage(FirstLaunchStages::RESTART);
            break;


    }

    return this;
}

FirstLaunchController* FirstLaunchController::welcome()
{
    bool pushed = 0;
    for(auto& ent : this->getController()->getAdvancedControls())
    {
        if(this->getController()->getGamepadControl(ent.first, GamepadActions::A) || this->getController()->getGamepadControl(ent.first, GamepadActions::B))
        {
            pushed = 1;
            this->gotToNextStage();
        }
    }
    return this;
}

FirstLaunchController* FirstLaunchController::addPartition()
{
    DiskController* diskController = static_cast<DiskController*>(this->getController()->getController(DISK_CONTROLLER));

    if(diskController != nullptr)
    {
        cout << "Crétion d'une nouvelle partition..." << endl;
        if(!diskController->createDataPartition())
        {
            cout << "Nouvelle partition crée !" << endl;
            this->gotToNextStage();
        }
        else
        {
            //Add major error
        }
    }
    else
    {
        //TODO Error
    }
    return this;
}
FirstLaunchController* FirstLaunchController::createFs()
{

    DiskController* diskController = static_cast<DiskController*>(this->getController()->getController(DISK_CONTROLLER));

    if(diskController != nullptr)
    {
        cout << "Création du système de fichier..." << endl;
        if(!diskController->createDataFileSystem())
        {
            cout << "Système de fichier crée" << endl;
            this->gotToNextStage();
        }
        else
        {
            //add majot error
        }
    }
    else
    {
        //TODO error
    }

    return this;
}

FirstLaunchController* FirstLaunchController::createDataFolder()
{
    OSController* osController = static_cast<OSController*>(this->getController()->getController(OS_CONTROLLER));

    if(osController != nullptr)
    {
        cout << "Création du point de montage..." << endl;
        //check if /data exists !
        string output = osController->execCmd("mkdir /data");
        cout << "Point de montage créé !" << endl;
        this->gotToNextStage();
    }
    else
    {
        //TODO error
    }

    return this;
}

FirstLaunchController* FirstLaunchController::insertFstab()
{
    DiskController* diskController = static_cast<DiskController*>(this->getController()->getController(DISK_CONTROLLER));

    if(diskController != nullptr)
    {
        cout << "Insertion de la partition dans fstab..." << endl;
        if(!diskController->addDataPartitionToFstab())
        {
            cout  << "Partition insérée dans fstab !" << endl;
            this->gotToNextStage();
        }
        else
        {
            //add major error
        }
    }
    else
    {
        //TODO Error
    }

    return this;
}

FirstLaunchController* FirstLaunchController::mountPart()
{
    DiskController* diskController = static_cast<DiskController*>(this->getController()->getController(DISK_CONTROLLER));

    if(diskController != nullptr)
    {
        cout << "Montage de la partition..." << endl;
        if(!diskController->mountDataPartition())
        {
            cout << "Partition montée !" << endl;
            this->gotToNextStage();
        }
        else
        {
            //add major error
        }
    }
    else
    {
        //TODO error
    }

    return this;
}

FirstLaunchController* FirstLaunchController::createDestinationFolders()
{
    DiskController* diskController = static_cast<DiskController*>(this->getController()->getController(DISK_CONTROLLER));

    if(diskController != nullptr)
    {
        cout << "Création des dossiers de destination.." << endl;
        if(!diskController->createPublicRessourceFolders())
        {
            cout << "Dossiers créés !" << endl;
            this->gotToNextStage();
        }
        else
        {
            //add major error
        }
    }
    else
    {
        //TODO error
    }

    return this;
}

FirstLaunchController* FirstLaunchController::finish()
{
    cout << "Nous avons finis la première installation, le système va maintenant redémarrer" << endl;
    this->gotToNextStage();
}

FirstLaunchController* FirstLaunchController::restart()
{
    this->unsetFirstLaunch();
    sleep(10);
    this->getController()->stop();

    OSController* osController = static_cast<OSController*>(this->getController()->getController(OS_CONTROLLER));

    if(osController != nullptr)
    {
        osController->reboot();
    }

    return this;
}

FirstLaunchController* FirstLaunchController::unsetFirstLaunch()
{
    Config cfg;

    try
    {
        cfg.readFile("/nostal/emul.cfg");
    }
    catch(const FileIOException &fieoex)
    {
        Log::addEntry(1, "/!\\ Could not read system config file : /nostal/emul.cfg !");
        return this;
    }
    catch(const ParseException &pex)
    {
        std::string file, line, error;
        file = pex.getFile();
        line = pex.getLine();
        error = pex.getError();
        Log::addEntry(1, "/!\\ Could not parse " + file + " : " + line + " - " + error);
        return this;
     }

     const Setting& root = cfg.getRoot();

     Setting& firstLaunch = cfg.lookup("firstLaunch");
     firstLaunch = false;
     try
  {
    cfg.writeFile("/nostal/emul.cfg");

  }
  catch(const FileIOException &fioex)
  {
    cerr << "I/O error while writing file: /nostal/emul.cfg"<< endl;
    return this;
  }
  return this;
 }
