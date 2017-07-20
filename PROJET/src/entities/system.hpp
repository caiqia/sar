#ifndef DEF_SYSTEM
#define DEF_SYSTEM

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>

// Classe System, qui contiens l'ensemble des systèmes de jeux et données associés

enum RequiresBios
{
    ACCEPTS_NO_BIOS,
    BIOS_IS_OPTIONAL,
    AT_LEAST_ONE_BIOS_REQUIRED,
    REQUIRES_BIOS
};

class Game;

class System
{
public:
    System(const std::string& name, const std::string& displayName, const std::string& logoFile, const std::string& greyedLogoFile, const std::string& backgroundImage43, const std::string& backgroundImage169, const std::string& romPath, const std::string& savePath, const std::string& biosPath, const std::vector<std::string>& requiredBiosMD5, const std::vector<std::string>& atLeastOneBiosMD5, const std::vector<std::string>& optionalBiosMD5, const std::vector<std::string>& extensions, const std::string& launchCmd);
    ~System();

    int getId() const;

    static bool populate();

    int loadGames();

    std::string getName() const;
    void setName(const std::string& name);

    std::string getDisplayName() const;
    void setDisplayName( const std::string& displayName);

    std::string getLogoFile() const;
    void setLogoFile(const std::string& file);

    std::string getGreyedLogoFile() const;
    System* setGreyedLogoFile(const std::string& file);

    std::string getBackground43() const;
    void setBackGround43(const std::string& file);

    std::string getBackground169() const;
    void setBackGround169(const std::string& file);

    std::string getRomPath() const;
    System* setRomPath(const std::string& path);


    std::string getSavePath() const;
    System* setSavePath(const std::string& path);

    std::string getBiosPath() const;
    System* setBiosPath(const std::string& path);

    std::vector<std::string> getRequiredBiosMD5() const;
    System* setRequiredBiosMD5(const std::vector<std::string>& requiredBiosMD5);

    std::vector<std::string> getAtLeastOneBiosMD5() const;
    System* setAtLeastOneBiosMD5(const std::vector<std::string>& atLeastOneBiosMD5);

    std::vector<std::string> getOptionalBiosMD5() const;
    System* setOptionalBiosMD5(const std::vector<std::string>& optionalBiosMD5);

    RequiresBios getReqiresBios() const;
    System* setRequiresBios(const RequiresBios& requiresBios);

    std::vector<std::string> getExtensions() const;
    System* setExtensions(const std::vector<std::string>& extensions);

    std::string getLaunchCmd() const;
    System* setLaunchCmd(const std::string& cmd);

    std::map<int, Game*> getGames() const;
    Game* getGame(const int& id) const;
    System* clearGames();
    System* addGame(Game* game);
    System* sortGames();

    int getNumberGamesAvailable() const;
    System* setNumberGamesAvailable(const int& number);

    bool hasGames() const;
    bool hasCorrectBIOS() const;
    bool isAvailable() const;
    System* checkAvailability();

    System* setSelectedGame(const int& selectedGame);
    int getSelectedGame() const;
    Game* getSelectedGameEntity() const;

    static bool pointerCompare(const Game* a, const Game* b);

    static std::map<int, System*> instances;
    static std::vector<System*> availableinstances;
    static std::vector<System*> notavailableinstances;


private:
    int id;
    static int idCount;
    std::string m_name;
    std::string m_displayName;
    std::string m_logoFile;
    std::string m_greyedLogoFile;
    std::string m_background43;
    std::string m_background169;
    std::string m_romPath;
    std::string m_savePath;
    std::string m_biosPath;
    std::vector<std::string> m_requiredBiosMD5;
    std::vector<std::string> m_atLeastOneBiosMD5;
    std::vector<std::string> m_optionalBiosMD5;
    RequiresBios m_requiresBios = ACCEPTS_NO_BIOS;
    std::vector<std::string> m_extensions;
    std::string m_launchCmd;

    bool m_hasGames = false;
    bool m_hasCorrectBIOS = false;
    bool m_available = false;

    std::map<int, Game*> m_availableGames;

    int m_numberGamesAvailable = 0;
    int m_selectedGame = 0;

};

#endif
