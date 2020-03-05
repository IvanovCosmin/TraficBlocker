// TraficBlocker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "TrafficChecker.h"
#include "UnitTester.h"

void UnitTesterTests() {
    UnitTester unitTestTests;
    unitTestTests.AddTest(UnitTest(
        "Error throw test",
        []() -> bool {
            throw std::runtime_error("This test was expected to crash");

        },
        "",
        ""
    ));
    unitTestTests.AddTest(UnitTest(
        "Success test",
        []() -> bool {
            return true;

        },
        "This test was expected to pass",
        ""
    ));
    unitTestTests.AddTest(UnitTest(
        "Fail test",
        []() -> bool {
            return false;
        },
        "",
        "This test was expected to fail"
    ));
    unitTestTests.RunTests();

    unitTestTests.ShowTestsResults(true);
}

void RunCidrTests() {
    UnitTester TrafficCheckerUnitTester;

    TrafficCheckerUnitTester.AddTest(UnitTest(
        "Block and allow test",
        []() -> bool {
            TrafficChecker checker;

            checker.BlockCidr(1);
            checker.AllowCidr(1);
            return checker.IsCidrBlocked(1) == CidrAllowanceStatus::CIDR_ALLOWED;
        },
        "Blocking and then allowing the same cidr works correcly",
        "Expected cidr to be allowed"
    ));

    TrafficCheckerUnitTester.AddTest(UnitTest(
        "Multiple block and allow test",
        []() -> bool {
            TrafficChecker checker;

            checker.BlockCidr(1);
            checker.BlockCidr(2);
            checker.AllowCidr(1);
            checker.BlockCidr(1);
            checker.AllowCidr(1);
            checker.AllowCidr(2);
            checker.BlockCidr(1);
            checker.AllowCidr(1);
            return checker.IsCidrBlocked(1) == CidrAllowanceStatus::CIDR_ALLOWED && checker.IsCidrBlocked(2) == CidrAllowanceStatus::CIDR_ALLOWED;
        },
        "Blocking and then allowing the same cidr works correcly",
        "Expected cidr to be allowed"
    ));

    TrafficCheckerUnitTester.AddTest(UnitTest(
        "Block test",
        []() -> bool {
            TrafficChecker checker;

            checker.BlockCidr(1);
            return checker.IsCidrBlocked(1) == CidrAllowanceStatus::CIDR_BLOCKED;
        },
        "Block works correcly",
        "Expected cidr to be blocked"
    ));

    TrafficCheckerUnitTester.AddTest(UnitTest(
        "Limits test",
        []() -> bool {
            TrafficChecker checker;
            if (checker.BlockCidr(0) == true) return false;
            if (checker.BlockCidr(33) == true) return false;
            if (checker.AllowCidr(0) == true) return false;
            if (checker.AllowCidr(33) == true) return false;
            if (checker.IsCidrBlocked(0) != CidrAllowanceStatus::CIDR_ERROR) return false;
            if (checker.IsCidrBlocked(33) != CidrAllowanceStatus::CIDR_ERROR) return false;

            return true;

        },
        "Superior and inferior limits are correcly handled",
        "Expected all tested function to return false or error."
    ));
}

int main()
{
    UnitTesterTests();
}
