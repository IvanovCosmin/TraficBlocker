#include "UnitTester.h"

void UnitTest::SetExceptionMessage(const char* exceptionMessage) {
    if (exceptionMessage == nullptr) {
        SHOW_ERROR("Tried to set an nullptr exception message");
        return;
    }
    this->exceptionMessage = exceptionMessage;
}

UnitTest::UnitTest(std::string testName, TestFunction testFunction, std::string passedTestMessage, std::string failedTestMessage) {
    this->testName = testName;
    this->testFunction = testFunction;
    this->passedTestMessage = passedTestMessage;
    this->failedTestMessage = failedTestMessage;
}

TestResult UnitTest::RunTest() {
    try {
        if (this->testFunction() == true) {
            testResult = TestResult::TEST_PASSED;
        }
        else {
            testResult = TestResult::TEST_FAILED;
        }
    }
    catch (const std::exception & exception) {
        SetExceptionMessage(exception.what());
        testResult = TestResult::TEST_CRASHED;
    }
    return testResult;
}

std::string UnitTest::GetFailedTestMessage() {
    return this->failedTestMessage;
}

std::string UnitTest::GetPassedTestMessage() {
    return this->passedTestMessage;
}

std::string UnitTest::GetTestName() {
    return this->testName;
}

// exception message is set only after running calling RunTest() if there was an exception

std::string UnitTest::GetExceptionMessage() {
    return this->exceptionMessage;
}

TestResult UnitTest::GetTestResult() {
    return testResult;
}

// to be called after calling AddTest and RunTests

void UnitTester::ShowTestsResults(bool verbose) {
    printf("\n\nShowing results: \n");
    if (tests.size() == 0) {
        SHOW_ERROR("No UnitTests were added. Call AddTest to add a test.\n");
        return;
    }
    if (testsFailed + testsPassed + testsCrashed == 0) {
        SHOW_ERROR("No test was run. Call RunTests to run the tests\n");
        return;
    }
    if (verbose) {
        for (auto& test : tests) {
            TestResult result = test.GetTestResult();

            if (result == TestResult::TEST_CRASHED) {
                SHOW_CRASHED("Test: \"%s\" has crashed with the following message: \"%s\"\n",
                    test.GetTestName().c_str(),
                    test.GetExceptionMessage().c_str()
                );
            }
            else if (result == TestResult::TEST_FAILED) {
                SHOW_ERROR("Test: \"%s\" has failed with the following message: \"%s\"\n",
                    test.GetTestName().c_str(),
                    test.GetFailedTestMessage().c_str()
                );
            }
            else if (result == TestResult::TEST_PASSED) {
                SHOW_SUCCESS("Test: \"%s\" has passed with the following message: \"%s\"\n",
                    test.GetTestName().c_str(),
                    test.GetPassedTestMessage().c_str()
                );
            }
            else {
                printf("Test: %s was not executed", test.GetTestName().c_str());
            }
        }
    }
    printf("\n-------------------\n");
    printf("Tests passed: %d\n", testsPassed);
    printf("Tests failed: %d\n", testsFailed);
    printf("Tests crashed: %d\n", testsCrashed);
}

// runs all the added tests

void UnitTester::RunTests() {
    for (auto& test : tests) {
        TestResult testResult = test.RunTest();
        switch (testResult)
        {
        case TestResult::TEST_PASSED:
            testsPassed++;
            break;
        case TestResult::TEST_FAILED:
            testsFailed++;
            break;
        case TestResult::TEST_CRASHED:
            testsCrashed++;
        }
    }
}

void UnitTester::AddTest(UnitTest test) {
    tests.push_back(test);
}
