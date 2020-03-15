// TraficBlocker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <set>
#include <initializer_list>
#include "TrafficChecker.h"
#include "Tests.h"

int main()
{
    printf("Starting some tests");
    UnitTesterTests();
    RunCidrTests();
    RunPortBlockerTests();
}
