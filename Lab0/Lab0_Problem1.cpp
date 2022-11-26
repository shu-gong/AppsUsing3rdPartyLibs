/*
Author: Shu Gong
Class: ECE6122 QSZ
Last Date Modified: 9/12/2022

Description:

This problem is very simple. Write a C++ program using the insertion stream operator and
escape sequences that outputs the following text to your terminal screen when executed:

*/

#include <iostream>

int main()
{

    std::cout << "My name is: Shu Gong\n";
    std::cout << "This (\") is a double quote.\n";
    std::cout << "This (\') is a single quote.\n";
    std::cout << "This (\\) is a backslash.\n";
    std::cout << "This (/) is a forward slash.\n";

    return 0;

}
