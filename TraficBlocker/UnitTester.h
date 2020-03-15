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

    TestFunction testFunction = nullptr;
    std::string passedTestMessage;
    std::string exceptionMessage;
    std::string failedTestMessage;
    std::string testName;
    TestResult testResult = TestResult::TEST_NOT_EXECUTED;
    
    void SetExceptionMessage(const char* exceptionMessage);

public:
    UnitTest(std::string testName, TestFunction testFunction, std::string passedTestMessage, std::string failedTestMessage);
    TestResult RunTest();
    std::string GetFailedTestMessage();
    std::string GetPassedTestMessage();
    std::string GetTestName();
    
    // exception message is set only after running calling RunTest() if there was an exception
    std::string GetExceptionMessage();
    TestResult GetTestResult();
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
    void ShowTestsResults(bool verbose);

    // runs all the added tests
    void RunTests();

    void AddTest(UnitTest test);
};

