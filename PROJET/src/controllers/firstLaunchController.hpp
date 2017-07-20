#ifndef DEF_FIRSTLAUNCHCONTROLLER
#define DEF_FIRSTLAUNCHCONTROLLER

#include "abstractController.hpp"

enum FirstLaunchStages
{
    WELCOME,
    ADD_PART,
    CREATE_FS,
    INSERT_FSTAB,
    CREATE_DATA_FOLDER,
    MOUNT_PART,
    CREATE_USER_FOLDERS,
    FINISH,
    RESTART
};

class FirstLaunchController : public AbstractController
{
public:
    FirstLaunchController(Controller* controller);
    ~FirstLaunchController();

    virtual FirstLaunchController* control();

    virtual FirstLaunchController* inputControl();

    FirstLaunchController* firstLaunch();

    FirstLaunchController* setFirstLaunch(bool firstLaunch);
    bool isFirstLaunch();

    FirstLaunchStages getCurrentStage();
    FirstLaunchController* setCurrentStage(FirstLaunchStages stage);
    FirstLaunchController* gotToNextStage();

    FirstLaunchController* welcome();
    FirstLaunchController* addPartition();
    FirstLaunchController* createFs();
    FirstLaunchController* createDataFolder();
    FirstLaunchController* insertFstab();
    FirstLaunchController* mountPart();
    FirstLaunchController* createDestinationFolders();
    FirstLaunchController* finish();
    FirstLaunchController* restart();

    FirstLaunchController* unsetFirstLaunch();

protected:
    bool m_firstLaunch = 0;

    FirstLaunchStages m_currentStage = FirstLaunchStages::WELCOME;
};

#endif
