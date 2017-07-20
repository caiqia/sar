#ifndef DEF_GAME
#define DEF_GAME

#include <string>

class System;

class Game
{
public:
    Game(System* system, std::string path);
    Game(System* system, std::string path, std::string name, std::string description, std::string imagePath, std::string type, int rating, int releaseYear, int numberPlayers, bool favorite);

    std::string getPath() const;
    Game* setPath(std::string path);

    std::string getName() const;
    Game* setName(std::string name);

    std::string getDescription() const;
    Game* setDescription(std::string description);

    std::string getImagePath() const;
    Game* setImagePath(std::string path);

    std::string getType() const;
    Game* setType(std::string type);

    int getRating() const;
    Game* setRating(int rating);

    int getReleaseYear() const;
    Game* setReleaseYear(int year);

    int getNumberPlayers() const;
    Game* setNumberPlayers(int number);

    bool getFavorite() const;
    Game* setFavorite(bool favorite);

    System* getSystem() const;
    Game* setSystem(System* system);

    bool operator < (Game const& a) const;

protected:
    std::string m_path;
    std::string m_name;
    std::string m_description;
    std::string m_imagePath;
    std::string m_type;
    int m_rating;
    int m_releaseYear;
    int m_numberPlayers;
    bool m_favorite;
    System* m_pSystem;
};

#endif
