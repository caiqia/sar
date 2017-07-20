#ifndef DEF_DISKCONTROLLER
#define DEF_DISKCONTROLLER

#include "abstractController.hpp"

#include <boost/filesystem.hpp>

struct recursive_directory_range
{
    typedef boost::filesystem::recursive_directory_iterator iterator;
    recursive_directory_range(boost::filesystem::path p) : p_(p) {}

    iterator begin() { return boost::filesystem::recursive_directory_iterator(p_); }
    iterator end() { return boost::filesystem::recursive_directory_iterator(); }

    boost::filesystem::path p_;
};

class DiskController : public AbstractController
{
public:
    DiskController(Controller* controller);
    ~DiskController();

    virtual DiskController* control();
    virtual DiskController* inputControl();

    int createDataPartition();

    int createDataFileSystem();

    int addDataPartitionToFstab();

    int mountDataPartition();

    int createPublicRessourceFolders();

    static bool createDirectory(const std::string& directoryPath);

    static std::string getFileMD5(const std::string& filePath);

protected:
};

#endif
