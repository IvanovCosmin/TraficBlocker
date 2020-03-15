#include "Tests.h"

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
            CidrBlocker checker;

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
            CidrBlocker checker;

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
        "Blocking and then allowing the same cidr multiple times works correcly",
            "Expected cidr to be allowed"
            ));

    TrafficCheckerUnitTester.AddTest(UnitTest(
        "Block test",
        []() -> bool {
            CidrBlocker checker;

            checker.BlockCidr(1);
            return checker.IsCidrBlocked(1) == CidrAllowanceStatus::CIDR_BLOCKED;
        },
        "Block works correcly",
            "Expected cidr to be blocked"
            ));

    TrafficCheckerUnitTester.AddTest(UnitTest(
        "Limits test",
        []() -> bool {
            CidrBlocker checker;
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

    TrafficCheckerUnitTester.RunTests();
    TrafficCheckerUnitTester.ShowTestsResults(true);
}

void RunPortBlockerTests() {
    UnitTester portBlockerUnitTester;
    portBlockerUnitTester.AddTest(UnitTest("Block and allow test",
        []() -> bool {
            PortBlocker portBlocker;
            portBlocker.addRanges({
                PortRange(10, 15),
                PortRange(11, 22),
                PortRange(1,2),
                PortRange(15, 22),
                PortRange(1,100),
                PortRange(10,102)
                });

            portBlocker.addRange(PortRange(123, 456));

            if (portBlocker.checkIfPortBlocked(10) == false) return false;
            if (portBlocker.checkIfPortBlocked(0) == true) return false;
            if (portBlocker.checkIfPortBlocked(123) == false) return false;
            if (portBlocker.checkIfPortBlocked(456) == false) return false;
            if (portBlocker.checkIfPortBlocked(150) == false) return false;
            if (portBlocker.checkIfPortBlocked(3) == false) return false;

            portBlocker.deleteRange(PortRange(10, 15));
            portBlocker.deleteRange(PortRange(0, 5));
            portBlocker.deleteRange(PortRange(101, 110));
            portBlocker.deleteRange(PortRange(16, 100));

            if (portBlocker.checkIfPortBlocked(10) == true) return false;
            if (portBlocker.checkIfPortBlocked(0) == true) return false;
            if (portBlocker.checkIfPortBlocked(456) == false) return false;
            if (portBlocker.checkIfPortBlocked(100) == true) return false;

            portBlocker.deleteRange(PortRange(123, 456));
            if (portBlocker.checkIfPortBlocked(123) == true) return false;

            return true;


        },
        "The port blocker works correctly",
            "Operations on port blocker do not give expected results"
            ));
    portBlockerUnitTester.RunTests();
    portBlockerUnitTester.ShowTestsResults(true);
}
