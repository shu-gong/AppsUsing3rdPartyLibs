/*
Author: Shu Gong
Class: ECE6122 QSZ
Last Date Modified: 11/20/2022

Description:

In this assignment you will be creating a TCP debug logging server

*/
#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include <ctime>
#include <fstream>

#pragma warning(disable : 4996).
using namespace std;
void reportInvalidInput(const string& stringName);
bool cvtRawSeq2RawDigit(const string& rawSeq, string& rawDigit);
bool cvtRawDigit2ValidDigit(const string& rawDigit, int& validDigit);
bool checkPortNum(char* argv, int& validDigit);

int main(int argc, char* argv[])
{
    fstream outFile;
    outFile.open("server.log", ios::out);

    int validDigit = 0;
    int port;
    string address;

    if (argc != 2)
    {
        cout << "Invalid command line argument detected: invalid argument number" << endl;
        cout << "Please check your values and press any key to end the program!" << endl;
        return 0;
    }
    else
    {
        if (checkPortNum(argv[1], validDigit))
        {
            port = validDigit;
        }
        else
        {
            return 0;
        }
    }

    cout << "Current Server Port: " << port << endl;
    // Create a socket to listen to new connections
    sf::TcpListener listener;
    
    listener.listen(port);

    // Create a list to store the future clients
    std::list<sf::TcpSocket*> clients;

    // Create a selector
    sf::SocketSelector selector;

    // Add the listener to the selector
    selector.add(listener);

    // Endless loop that waits for new connections
    while (1)
    {
        // Make the selector wait for data on any socket
        if (selector.wait())
        {
            // Test the listener
            if (selector.isReady(listener))
            {
                // The listener is ready: there is a pending connection
                sf::TcpSocket* client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done)
                {
                    // Add the new client to the clients list
                    clients.push_back(client);
                    // Add the new client to the selector so that we will
                    // be notified when he sends something
                    selector.add(*client);
                    time_t now = time(0);
                    char* dt = ctime(&now);
                    dt[strlen(dt) - 1] = 0;
                    cout << dt << " :: " << client->getRemoteAddress() << " :: " << "Connected" << endl;
                    outFile << dt << " :: " << client->getRemoteAddress() << " :: " << "Connected" << endl;
                }
                else
                {
                    // Error, we won't get a new connection, delete the socket
                    delete client;
                }
            }
            else
            {
                // The listener socket is not ready, test all other sockets (the clients)
                for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                {
                    sf::TcpSocket& client = **it;
                    if (selector.isReady(client))
                    {
                        // The client has sent some data, we can receive it
                        sf::Packet packet;
                        if (client.receive(packet) == sf::Socket::Done)
                        {
                            time_t now = time(0);
                            char* dt = ctime(&now);
                            dt[strlen(dt) - 1] = 0;

                            std::string s;
                            packet >> s;
                            if ( s == "quit")
                            {
                                cout << dt << " :: " << client.getRemoteAddress() << " :: " << "Disconnected" << endl;;
                                outFile << dt << " :: " << client.getRemoteAddress() << " :: " << "Disconnected" << endl;;
                            }
                            else
                            {
                                cout << dt << " :: " << client.getRemoteAddress() << " :: " << s << endl;;
                                outFile << dt << " :: " << client.getRemoteAddress()<<" :: " << s << endl;;
                            }
                        }
                    }
                }
            }
        }
    }
}

/******************************************************************/
// @brief: report invalid input.
/******************************************************************/
void reportInvalidInput(const string& stringName)
{
    cout << "Invalid command line argument detected: invalid " << stringName << endl;
    cout << "Please check your values and press any key to end the program!" << endl;
}

/******************************************************************/
// @brief: convert raw sequence to raw digit
// @para: rawSeq means raw sequence of input, rawDigit is a all-digit string
// @ret: return true if convet successfully
/******************************************************************/
bool cvtRawSeq2RawDigit(const string& rawSeq, string& rawDigit)
{
    for (int i = 0; i < rawSeq.length(); i++)
    {
        // This loop is used to process raw input sequence and generate raw digit.
        if (isdigit(rawSeq[i]))
        {
            rawDigit += rawSeq[i];
        }
        else
        {
            return false;
        }
    }
    return true;
}

/******************************************************************/
// @brief: convert raw digit to valid digit
// @para: rawDigit is a all-digit string, validDigit is a real number
// @ret: return true if convert successfully
/******************************************************************/
bool cvtRawDigit2ValidDigit(const string& rawDigit, int& validDigit)
{
    if (rawDigit[0] == '0')
    {
        return false;
    }
    else
    {
        try
        {
            validDigit = stoi(rawDigit);
        }
        catch (const std::exception&)
        {
            return false;
        }
    }

    if (validDigit < 61000 || validDigit>65535)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/******************************************************************/
// @brief: check the port number
// @para: argv, validDigit->valid port number
// @ret: return true if port number is valid
/******************************************************************/
bool checkPortNum(char* argv, int& validDigit)
{

    string rawSeq = argv;
    string rawDigit = "";
    validDigit = 0;
    string stringName = "Port Number";

    if (cvtRawSeq2RawDigit(rawSeq, rawDigit))
    {
        int flag = 0;
        if (cvtRawDigit2ValidDigit(rawDigit, validDigit))
        {
            return true;
        }
        else
        {
            reportInvalidInput(stringName);
            return false;
        }
    }
    else
    {
        reportInvalidInput(stringName);
        return false;
    }
}
