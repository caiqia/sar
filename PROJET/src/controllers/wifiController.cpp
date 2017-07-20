#include "wifiController.hpp"
#include "controller.hpp"
#include "osController.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <libconfig.h++>
#include <unistd.h>

using namespace std;

map<AuthType, string> WifiController::authTypeResolver = {{AuthType::NONE, "none"}, {AuthType::WEP, "WEP"}, {AuthType::WPA, "WPA"}, {AuthType::WPA2, "WPA2"}};

WifiController::WifiController(Controller* controller)
: AbstractController(controller, ControllersID::WIFI_CONTROLLER)
{
}

WifiController::~WifiController()
{

}

WifiController* WifiController::control()
{
    AbstractController::control();
    return this;
}

WifiController* WifiController::inputControl()
{
    AbstractController::inputControl();
    return this;
}

vector<VisibleNetwork> WifiController::scanWifi()
{
    vector<VisibleNetwork> networks;

    string cmd = "iw dev wlan0 scan";

    //if(!this->getController()->getDevMode())
    {
        string line;

        OSController* osController = static_cast<OSController*>(this->getController()->getController(OS_CONTROLLER));

        istringstream output;

        if(osController != nullptr)
        {
            output.str(osController->execCmd(cmd.c_str()));
        }



        int networkCount = 0;
        VisibleNetwork currentNetwork;

        regex newNetworkRgx("^BSS ([a-z0-9]{2}:[a-z0-9]{2}:[a-z0-9]{2}:[a-z0-9]{2}:[a-z0-9]{2}:[a-z0-9]{2})");
        regex SSIDrgx("SSID: (.+)");
        regex signalLevelRgx("signal: -([0-9]+).[0-9]+ dBm");
        regex wpaAuthRgx("WPA:");
        regex wpa2AuthRgx("RSN:");
        regex wepAuthRgx("capability: .+Privacy");


        smatch match;

        while(getline(output, line))
        {
            if(regex_search(line, match, newNetworkRgx))
            {
                if(networkCount > 0 && currentNetwork.SSID != "" && currentNetwork.SSID != "\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00")
                {
                    bool insert = true;
                    for(auto& ent : networks)
                    {
                        if(ent.SSID.compare(currentNetwork.SSID) == 0 && ent.auth == currentNetwork.auth)
                        {
                            insert = false;
                            if(ent.signalLevel > currentNetwork.signalLevel)
                            {
                                ent.signalLevel = currentNetwork.signalLevel;
                            }
                        }
                    }
                    if(insert)
                    {
                        networks.push_back(currentNetwork);
                    }

                }
                currentNetwork.SSID = "";
                currentNetwork.signalLevel = VERY_WEAK_SIGNAL;
                currentNetwork.auth = AuthType::NONE;
                ++networkCount;
            }
            else if(regex_search(line, match, SSIDrgx))
            {
                currentNetwork.SSID = match[1];
            }
            else if(regex_search(line, match, signalLevelRgx))
            {
                int power = stoi(match[1]);

                if(power < WifiSignalPower::STRONG_SIGNAL)
                {
                    currentNetwork.signalLevel = STRONG_SIGNAL;
                }
                else if(power < WifiSignalPower::MEDIUM_SIGNAL)
                {
                    currentNetwork.signalLevel = MEDIUM_SIGNAL;
                }
                else if(power < WifiSignalPower::WEAK_SIGNAL)
                {
                    currentNetwork.signalLevel = WEAK_SIGNAL;
                }
                else
                {
                    currentNetwork.signalLevel = VERY_WEAK_SIGNAL;
                }
            }
            else if(regex_search(line, match, wpa2AuthRgx))
            {
                currentNetwork.auth = AuthType::WPA2;
            }
            else if(regex_search(line, match, wpaAuthRgx) && currentNetwork.auth != AuthType::WPA2)
            {
                currentNetwork.auth = AuthType::WPA;
            }
            else if(regex_search(line, match, wepAuthRgx) && currentNetwork.auth != AuthType::WPA2 && currentNetwork.auth != AuthType::WPA)
            {
                currentNetwork.auth = AuthType::WEP;
            }
        }
        if(currentNetwork.SSID != "" && currentNetwork.SSID != "\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00")
        {
            bool insert = true;
            for(auto& ent : networks)
            {
                if(ent.SSID.compare(currentNetwork.SSID) == 0)// && ent.auth == currentNetwork.auth)
                {
                    insert = false;
                    if(ent.signalLevel > currentNetwork.signalLevel)
                    {
                        ent.signalLevel = currentNetwork.signalLevel;
                    }
                }
            }
            if(insert)
            {
                networks.push_back(currentNetwork);
            }
        }

        sort(networks.begin(), networks.end());

        for(auto & ent : networks)
        {
            cout << "Found : \"" << ent.SSID << "\", signal strength : -" << ent.signalLevel << " dBm, auth : " << this->authTypeResolver[ent.auth]  << endl;
        }
    }


    return networks;
}
/*
bool WifiController::connect(Network network)
{

    if(!this->getController()->getDevMode())
    {
        ifstream wpaSupplicant("/etc/wpa_supplicant.conf", ios::in);

        if(wpaSupplicant.is_open())
        {
            string line;
            int networkCounter = 0;
            bool hasMatched = false;
            bool isMatching = false;
            Network matched;
            matched.auth = AuthType::NONE;
            regex networkRgx("^network=\\{");
            regex SSIDrgx("^[ ]*ssid=\"(.+)\"");
            regex PSKrgx("^[ ]*psk=\"(.+)\"");
            regex WPA2rgx("^[ ]*proto=\"WPA RSN\"");
            regex WPArgx("^[ ]*key_mgmt=\"WPA-PSK WPA-EAP\"");
            regex WEPrgx("^[ ]*wep_key0=\"(.+)\"");
            regex endRgx("^}");
            smatch match;
            vector<string> origSupplicant;
            int matchLineFrom = 0;
            int matchLineTo = 0;
            int lineCounter = 0;

            while(getline(wpaSupplicant, line))
            {
                    if(regex_search(line, match, networkRgx))
                    {
                        ++networkCounter;
                    }
                    else if(networkCounter > 0)
                    {
                        if(regex_search(line, match, SSIDrgx))
                        {
                            if(match[1] == network.SSID)
                            {
                                hasMatched = true;
                                isMatching = true;
                                matched.SSID = match[1];
                                matchLineFrom = lineCounter - 2;
                            }
                        }
                        if(isMatching)
                        {
                            if(regex_search(line, match, PSKrgx))
                            {
                                matched.password = match[1];
                            }
                            else if(regex_search(line, match, WPA2rgx))
                            {
                                matched.auth = AuthType::WPA2;
                            }
                            else if(regex_search(line, match, WPArgx) && matched.auth != AuthType::WPA2)
                            {
                                matched.auth = AuthType::WPA;
                            }
                            else if(regex_search(line, match, WEPrgx)  && matched.auth != AuthType::WPA2 && matched.auth != AuthType::WPA)
                            {
                                matched.auth = AuthType::WEP;
                                matched.password = match[1];
                            }
                            else if(regex_search(line, match, endRgx))
                            {
                                isMatching = false;
                                matchLineTo = lineCounter;
                            }
                        }
                    }
                    ++ lineCounter;
                    origSupplicant.push_back(line);
            }

            wpaSupplicant.close();
            ofstream tmpWpaSupplicant("/etc/wpa_supplicant.conf.tmp", ios::out | ios::trunc);

            if(tmpWpaSupplicant.is_open())
            {
                int i =0;
                for(auto & ent : origSupplicant)
                {
                    if(hasMatched)
                    {
                        if(matchLineFrom < matchLineTo)
                        {
                            if(i < matchLineFrom || i > matchLineTo)
                            {
                                cout << ent << endl;
                                tmpWpaSupplicant << ent << endl;
                            }
                            else
                            {

                            }
                        }
                        else
                        {
                            cout << "Error while parsing wifi wpa supplicant" << endl;
                            return false;
                        }

                    }
                    else
                    {
                        cout << ent << endl;
                        tmpWpaSupplicant << ent << endl;
                    }
                    ++i;
                }

                tmpWpaSupplicant << endl << "network={" << endl;
                tmpWpaSupplicant << "    ssid=\"" << network.SSID << "\"" << endl;

                switch (network.auth) {
                    case AuthType::NONE :
                        tmpWpaSupplicant << "    key_mgmt=NONE" << endl;
                        break;

                    case AuthType::WPA :
                        tmpWpaSupplicant << "    key_mgmt=WPA-PSK" << endl;
                        tmpWpaSupplicant << "    proto=WPA" << endl;
                        tmpWpaSupplicant << "    pairwise=TKIP" << endl;
                        tmpWpaSupplicant << "    auth_alg=OPEN" << endl;
                        tmpWpaSupplicant << "    psk=\"" << network.password << "\"" << endl;
                        break;

                    case AuthType::WPA2 :
                        tmpWpaSupplicant << "    key_mgmt=WPA-PSK" << endl;
                        tmpWpaSupplicant << "    proto=RSN" << endl;
                        tmpWpaSupplicant << "    pairwise=CCMP" << endl;
                        tmpWpaSupplicant << "    auth_alg=OPEN" << endl;
                        tmpWpaSupplicant << "    psk=\"" << network.password << "\"" << endl;
                        break;

                    case AuthType::WEP :
                        tmpWpaSupplicant << "    key_mgmt=NONE" << endl;
                        tmpWpaSupplicant << "    wep_tx_keyidx=0" << endl;
                        tmpWpaSupplicant << "    wep_key0=" << network.password << endl;
                        break;
                }

                tmpWpaSupplicant << "}" << endl;

                tmpWpaSupplicant.close();

                system("mv /etc/wpa_supplicant.conf.tmp /etc/wpa_supplicant.conf");

                this->reloadChipParam();

                sleep(3);

                ConnexionState status;
                status = this->testConnexion();

                cout << "Connexion test over" << endl;
                cout << "SSID : " << status.SSID << endl;

                if(status.connectedToRouter)
                {
                    cout << "Connected to router true" << endl;
                    if(status.ipObtained)
                    {
                        cout << "IP obtained true" << endl;
                        if(status.connectedToInternet)
                        {
                            cout << "Retropac fully connected to internet, IP : " << status.ip << endl;
                            if(status.connexionType == ConnexionType::WIFI)
                            {
                                cout << "connected to : " << status.SSID << endl;
                            }
                            return true;
                        }
                        else
                        {
                            cout << "Retropac connected to router but cannot access internet" << endl;
                            return false;
                        }
                    }
                    else
                    {
                        cout << "Could not retrieve IP adress from router";
                        return false;
                    }
                }
                else
                {
                    cout << "Could not connect to local router" << endl;
                    return false;
                }

            }
            else
            {
                cout << "Could not create temporary file for wpa supplicant" << endl;
                return false;
            }

        }
        else
        {
            //TODO error
            cout << "Could not load wpa_supplicant.conf" << endl;
            return false;
        }
    }
    cout << "Automatically asumed connexion from dev mode" << endl;
    return true;
}*/

WifiController* WifiController::startChip()
{
    if(!this->getController()->getDevMode())
    {
        system("ifconfig wlan0 up");
    }
    return this;
}

WifiController* WifiController::stopChip()
{
    if(!this->getController()->getDevMode())
    {
        system ("ifconfig wlan0 down");
    }
    return this;
}

WifiController* WifiController::restartChip()
{
    this->stopChip();
    this->startChip();
    return this;
}

WifiController* WifiController::reloadChipParam()
{
    if(!this->getController()->getDevMode())
    {
        system("wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf");
        system("dhcpcd");
    }
    return this;
}
/*
ConnexionState WifiController::testConnexion()
{
    cout << "Determining connexion status" << endl;
    istringstream lanIfconfigOutput (this->getController()->getOSController()->execCmd("ifconfig eth0"));

    ConnexionState eth0;
    eth0.connexionType = ConnexionType::LAN;

    smatch match;
    regex ipConnexion("^[ ]*inet addr:([0-9]+.[0-9]+.[0-9]+.[0-9]+)");

    string line;
    while(getline(lanIfconfigOutput, line))
    {
        if(regex_search(line, match, ipConnexion))
        {
            cout << "Connected through LAN" << endl;
            eth0.connectedToRouter = true;
            eth0.ipObtained = true;
            eth0.ip = match[1];
            eth0.connectedToInternet = this->pingTest();
            return eth0;
        }
    }

    if(!eth0.ipObtained)
    {
        cout << "Trying wlan connexion" << endl;
        ConnexionState wlan0;
        wlan0.connexionType = ConnexionType::WIFI;

        istringstream iwconfigOutput(this->getController()->getOSController()->execCmd("iwconfig wlan0"));

        regex wlan0router("^wlan0[ ]*IEEE[ ]*[0-9]+.[0-9]+[a-z]+[ ]*ESSID:\"(.*)\"");

        while(getline(iwconfigOutput, line))
        {
            int i = 0;
            if(regex_search(line, match, wlan0router))
            {
                cout << "Found wlan connection to local router : " << match[1] << endl;
                cout << i << endl; ++i;
                wlan0.connectedToRouter = true;
                wlan0.SSID = match[1];
            }
        }

        if(wlan0.connectedToRouter)
        {
            cout << "Obtaining IP adress..." << endl;
            for(int i = 0; i < 10; ++i)
            {
                istringstream wlanIfconfigOutput (this->getController()->getOSController()->execCmd("ifconfig wlan0"));

                while(getline(wlanIfconfigOutput, line))
                {
                    cout << line << endl;
                    if(regex_search(line, match, ipConnexion))
                    {
                        cout << "Local IP assigned on wlan : " << match[1] << endl;
                        wlan0.ipObtained = true;
                        wlan0.ip = match[1];
                        i = 10;
                    }
                }

                if(i < 10)
                {
                    sleep(1);
                }
            }

            if(wlan0.ipObtained)
            {
                cout << "Pinging..." << endl;
                wlan0.connectedToInternet = this->pingTest();

                if(wlan0.connectedToInternet)
                {
                    cout << "Ping successfull" << endl;
                }
            }
        }

        return wlan0;
    }


    cout << "Should not get through here" << endl;

    //Return connexion status
}

bool WifiController::pingTest()
{
    bool connected = false;
    regex regexPingRgx("([0-9])+[ ]*packets transmitted,[ ]*([0-9])+ packets received");
    smatch match;
    string line;

    cout << "Pinging first server" << endl;
    istringstream ping1Output (this->getController()->getOSController()->execCmd("ping -c 3 -W 1 4.2.2.1"));

    while(getline(ping1Output, line))
    {
        cout << line << endl;
        if(regex_search(line, match, regexPingRgx))
        {
            cout  << "found pingable line : " << line << endl << "packets : " << match[1] << " " << match[2] << endl;
            if(stoi(match[2]) > 0)
            {
                cout << "successfull" << endl;
                connected = true;
            }

        }
    }

    if(!connected)
    {
        cout << "Pinging second server" << endl;
        istringstream ping2Output (this->getController()->getOSController()->execCmd("ping -c 3 -W 1 8.8.8.8"));

        while(getline(ping2Output, line))
        {
            cout << line << endl;
            if(regex_search(line, match, regexPingRgx))
            {
                if(stoi(match[2]) > 0)
                {
                    cout << "successfull" << endl;
                    connected = true;
                }
            }
        }

        if(!connected)
        {
            cout << "Pinging third server" << endl;
            istringstream ping3Output (this->getController()->getOSController()->execCmd("ping -c 3 -W 1 www.nostalgames.fr:80"));

            while(getline(ping3Output, line))
            {
                if(regex_search(line, match, regexPingRgx))
                {
                    if(stoi(match[2]) > 0)
                    {
                        cout << "successfull" << endl;
                        connected = true;
                    }
                }
            }
        }

    }

    return connected;
}*/
