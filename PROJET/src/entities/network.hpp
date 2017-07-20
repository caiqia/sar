#ifndef DEF_NETWORK
#define DEF_NETWORK

#include <string>

enum WifiSignalPower
{
    VERY_WEAK_SIGNAL = 100,
    WEAK_SIGNAL = 85,
    MEDIUM_SIGNAL = 70,
    STRONG_SIGNAL = 50,
};

enum AuthType
{
    NONE,
    WEP,
    WPA,
    WPA2
};

enum ConnexionType
{
    LAN,
    WIFI
};

struct ConnexionState
{
    ConnexionType connexionType;
    bool connectedToRouter = false;
    std::string SSID = "";
    bool ipObtained = false;
    std::string ip = "";
    bool connectedToInternet = false;
};

struct VisibleNetwork
{
    std::string SSID;
    WifiSignalPower signalLevel;
    AuthType auth = AuthType::NONE;

    bool operator < (const VisibleNetwork& x) const
    {
        return signalLevel < x.signalLevel;
    }
};


class Network
{
public:
    Network();
    ~Network();


protected:
    std::string m_ssid;
    std::string m_password;
    AuthType m_authType;
};

#endif
