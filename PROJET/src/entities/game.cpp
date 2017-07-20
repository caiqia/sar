#include <string>
#include <boost/filesystem.hpp>

#include "game.hpp"
#include "system.hpp"

using namespace std;
namespace fs = boost::filesystem;

//Game Object, stores information about a game

//Init
Game::Game(System* system, string path)
{
    /*
    For now the system only initialises based on the file name

    //TODO translate rom name to real game name
    Ex : "Legend of Zelda, The - Link's Awakening (V1.2) (U) [!].gb"
    Becomes : "The Legend of Zelda - Link's Awakening"

    //TODO Scrape information from online databases based of rom name and CRC/MD5
    */
    fs::path p(path);
    this->setPath(p.string());
    this->setName(p.stem().string());
    this->setSystem(system);
}

string Game::getPath() const
{
    return this->m_path;
}

Game* Game::setPath(string path)
{
    this->m_path = path;
    return this;
}

string Game::getName() const
{
    return this->m_name;
}

Game* Game::setName(string name)
{
    this->m_name = name;
    return this;
}

string Game::getDescription() const
{
    return this->m_description;
}

Game* Game::setDescription(string description)
{
    this->m_description = description;
    return this;
}

string Game::getImagePath() const
{
    return this->m_imagePath;
}

Game* Game::setImagePath(string path)
{
    this->m_imagePath = path;
    return this;
}

string Game::getType() const
{
    return this->m_type;
}

Game* Game::setType(string type)
{
    this->m_type = type;
    return this;
}

int Game::getRating() const
{
    return this->m_rating;
}

Game* Game::setRating(int rating)
{
    this->m_rating = rating;
    return this;
}

int Game::getReleaseYear() const
{
    return this->m_releaseYear;
}

Game* Game::setReleaseYear(int year)
{
    this->m_releaseYear = year;
    return this;
}

int Game::getNumberPlayers() const
{
    return this->m_numberPlayers;
}

Game* Game::setNumberPlayers(int number)
{
    this->m_numberPlayers = number;
    return this;
}

bool Game::getFavorite() const
{
    return this->m_favorite;
}

Game* Game::setFavorite(bool favorite)
{
    this->m_favorite = favorite;
    return this;
}

System* Game::getSystem() const
{
    return this->m_pSystem;
}

Game* Game::setSystem(System* system)
{
    this->m_pSystem = system;
    return this;
}

//Alphabetical comparions operator
bool Game::operator < (Game const& a) const
{
    string name = this->m_name;
    string aName = a.m_name;
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    transform(aName.begin(), aName.end(), aName.begin(), ::tolower);
    int result = name.compare(aName);
    if (result < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
