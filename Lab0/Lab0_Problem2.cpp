/*
Author: Shu Gong
Class: ECE6122 QSZ
Last Date Modified: 9/12/2022

Description:

The write a console program that continuously takes in a number n from the console and outputs to
the console the number of 1's that must be added to the positive integer n before the process
above ends.

*/

#include <iostream>
#include <string>
#include <climits>
using namespace std;

void calculate();
void welcome();
void reportInvalidInput();
void reportInstCount(const int& instCount);
void calcInstOfValidDigit(int& validDigit);
bool rawSeqIsNotEmpty(const string& rawSeq);
bool cvtRawSeq2RawDigit(const string& rawSeq, string& rawDigit);
bool cvtRawDigit2ValidDigit(const string& rawDigit, int& validDigit, int& flag);

int main()
{
	calculate();
}

/******************************************************************/
// @brief: the main function to process the input sequence
/******************************************************************/
void calculate()
{

	while (1)
	{
		string rawSeq = "";
		string rawDigit = "";
		int validDigit = 0;

		welcome();
		getline(cin, rawSeq);

		if (rawSeqIsNotEmpty(rawSeq))
		{
			if (cvtRawSeq2RawDigit(rawSeq, rawDigit))
			{
				int flag = 0;
				if (cvtRawDigit2ValidDigit(rawDigit, validDigit, flag))
				{
					calcInstOfValidDigit(validDigit);
				}
				else
				{
					if (flag == 1)
					{
						break;
					}
					else
					{
						reportInvalidInput();
					}
				}
			}
			else
			{
				reportInvalidInput();
			}
		}
		else
		{
			reportInvalidInput();
		}
	}
}

/******************************************************************/
// @brief: ask the user to input a number.
/******************************************************************/
void welcome()
{
	cout << "Please enter the starting number n: ";
}

/******************************************************************/
// @brief: report invalid input.
/******************************************************************/
void reportInvalidInput()
{
	cout << "Invalid input!! Please try again." << endl;
}

/******************************************************************/
// @brief: report the calculation results.
/******************************************************************/
void reportInstCount(const int& instCount)
{
	cout << "The sequence had " << instCount << " instances of the number 1 being added." << endl;
}

/******************************************************************/
// @brief: calculate the count the number 1 being added.
// @para: validDigit is a real number, meaning valid digit of the inputs.
/******************************************************************/
void calcInstOfValidDigit(int& validDigit)
{
	int instCount = 0;
	while (1)
	{
		if (validDigit == 1)
		{
			break;
		}
		if (validDigit % 7 == 0)
		{
			validDigit /= 7;
		}
		else
		{
			validDigit += 1;
			instCount += 1;
		}
	}
	reportInstCount(instCount);
}

/******************************************************************/
// @brief: check if raw sequence of input is empty
// @para: rawSeq means raw sequence of input
// @ret: return true if the input is not empty
/******************************************************************/
bool rawSeqIsNotEmpty(const string& rawSeq)
{
	if (rawSeq.length() == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
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
bool cvtRawDigit2ValidDigit(const string& rawDigit, int& validDigit, int& flag)
{
	if (rawDigit[0] == '0')
	{
		if (rawDigit.length() > 1)
		{
			flag = 0;
			return false;
		}
		else
		{
			flag = 1;
			return false;
		}
	}
	else
	{
		try
		{
			validDigit = stoi(rawDigit);
		}
		catch (const std::exception&)
		{
			flag = 2;
			return false;
		}
	}

	if (validDigit == INT_MAX)
	{
		flag = 3;
		return false;
	}
	else
	{
		flag = 4;
		return true;
	}
}