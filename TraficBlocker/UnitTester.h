#pragma once
#include <exception>
#include "utils.h"
#include <string>
#include <iostream>
#include <vector>

enum class TestResult {
    TEST_NOT_EXECUTED,
    TEST_PASSED,
    TEST_FAILED,
    TEST_CRASHED
};

typedef bool (*TestFunction)();

class UnitTest {

protected:
    const char* exceptionMessage = nullptr;

    TestFunction testFunction = nullptr;
    std::string passedTestMessage;
    std::string failedTestMessage;
    std::string testName;
    TestResult testResult = TestResult::TEST_NOT_EXECUTED;
    
    void SetExceptionMessage(const char* exceptionMessage) {
        if (exceptionMessage == nullptr) {
            SHOW_ERROR("Tried to set an nullptr exception message");
            return;
        }
        this->exceptionMessage = exceptionMessage;
    }

public:
    UnitTest(std::string testName, TestFunction testFunction, std::string passedTestMessage, std::string failedTestMessage) {
        this->testName = testName;
        this->testFunction = testFunction;
        this->passedTestMessage = passedTestMessage;
        this->failedTestMessage = failedTestMessage;
    }
    TestResult RunTest() {
        try {
            if (this->testFunction() == true) {
                testResult = TestResult::TEST_PASSED;
            }
            else {
                testResult = TestResult::TEST_FAILED;
            }
        }
        catch (const std::exception& exception) {
            SetExceptionMessage(exception.what());
            testResult = TestResult::TEST_CRASHED;
        }
        return testResult;
    }
    std::string GetFailedTestMessage() {
        return this->failedTestMessage;
    }
    std::string GetPassedTestMessage() {
        return this->passedTestMessage;
    }
    std::string GetTestName() {
        return this->testName;
    }
    
    // exception message is set only after running calling RunTest() if there was an exception
    std::string GetExceptionMessage() {
        if (exceptionMessage == nullptr) {
            return "No exception";
        }
        return this->exceptionMessage;
    }
    TestResult GetTestResult(){
        return testResult;
    }
};

class UnitTester
{

protected:
    std::vector<UnitTest> tests;
    unsigned short testsFailed = 0;
    unsigned short testsPassed = 0;
    unsigned short testsCrashed = 0;
    

public:

    // to be called after calling AddTest and RunTests
    void ShowTestsResults(bool verbose) {
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
                    SHOW_CRASHED("Test: \"%s\" has crashed with the following message: %s\n",
                        test.GetTestName().c_str(),
                        test.GetExceptionMessage().c_str()
                    );
                }
                else if (result == TestResult::TEST_FAILED) {
                    SHOW_ERROR("Test: \"%s\" has failed with the following message: %s\n",
                        test.GetTestName().c_str(),
                        test.GetFailedTestMessage().c_str()
                    );
                }
                else if (result == TestResult::TEST_PASSED) {
                    SHOW_SUCCESS("Test: \"%s\" has passed with the following message: %s\n",
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
    void RunTests() {
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

    void AddTest(UnitTest test) {
        tests.push_back(test);
    }
};

