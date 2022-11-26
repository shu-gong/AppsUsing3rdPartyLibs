/*
Author: Shu Gong
Class: ECE6122 QSZ
Last Date Modified: 11/20/2022

Description:

In this assignment you will be creating a TCP debug logging server

*/
#include <iostream>
#include<string>
#include <SFML/Network.hpp>
#include<Windows.h>

using namespace std;
void reportInvalidInput(const string& stringName);
bool cvtRawSeq2RawDigit(const string& rawSeq, string& rawDigit);
bool cvtRawDigit2ValidDigit(const string& rawDigit, int& validDigit);
bool checkPortNum(char* argv, int& validDigit);
int main(int argc, char* argv[])
{
	int validDigit = 0;
	int port;
	string address;

	cout << "Client" << endl;
	cout << "IP: " << argv[1] << endl;
	cout << "Port Number: " << argv[2] << endl;

	if (argc != 3)
	{
		cout << "Invalid command line argument detected: invalid argument number" << endl;
		cout << "Please check your values and press any key to end the program!" << endl;
		return 0;
	}
	else
	{
		if (checkPortNum(argv[2], validDigit))
		{
			port = validDigit;
			address = argv[1];
		}
		else
		{
			return 0;
		}
	}

    // Ask for the server address
    sf::IpAddress server;
	server = address;

    // Create a socket for communicating with the server
    sf::TcpSocket socket;

    // Connect to the server
	if (socket.connect(server, port) != sf::Socket::Done)
	{
		cout << "Failed to connect to the server at "<< address << " on "<< port << endl;
		cout << "Please check your values and press any key to end program!" << endl;
		return 0;
	}

    while (1)
    {
        cout << "Please enter a message: ";

		std::string s;
		getline(cin,s);

        if (s == "quit")
        {
            // Group the variables to send into a packet
            sf::Packet packet;
            packet << s;
            // Send it over the network (socket is a valid sf::TcpSocket)
            socket.send(packet);
            break;
        }
        else
        {
            // Group the variables to send into a packet
            sf::Packet packet;
            packet << s;
            // Send it over the network (socket is a valid sf::TcpSocket)
            socket.send(packet);
        }
    }
}



/******************************************************************/
// @brief: report invalid input.
/******************************************************************/
void reportInvalidInput(const string& stringName)
{
	cout << "Invalid command line argument detected: invalid "<< stringName << endl;
	cout<<"Please check your values and press any key to end the program!" << endl;
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
// @para: rawDigit is a all-digit string, validDigit is a real number, flag is used to indicate the "return type"
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

	if (validDigit< 61000 || validDigit>65535)
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
bool checkPortNum(char* argv, int &validDigit)
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
