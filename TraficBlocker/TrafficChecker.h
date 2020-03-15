#pragma once
#include <set>
#include "utils.h"
#include "bitoperations.h"
#include <cstdint>

enum class CidrAllowanceStatus {
    CIDR_BLOCKED,
    CIDR_ALLOWED,
    CIDR_ERROR
};

class PortRange {
private:
    uint16_t min;
    uint16_t max;

public:

    static bool CheckIfOverlapping(const PortRange range1, const PortRange range2);

    PortRange(uint16_t min, uint16_t max);


    uint16_t GetMin() const;
    uint16_t GetMax() const;

    // used by std::set to sort ranges
    bool operator < (const PortRange comparedTo) const;
    bool operator == (const PortRange comparedTo) const;
    PortRange operator + (PortRange const addedRange) const;

};

class PortBlocker {
private:
    std::set<PortRange> blockedPortsSet; // i have chosen a set becuse it keeps all the data sorted via a provided less than operator
    void mergeOverlappingIntervals();
public:
    PortBlocker();
    PortBlocker(std::initializer_list<PortRange> ranges);
    void addRange(PortRange range);
    void addRanges(std::initializer_list<PortRange> ranges);
    void deleteRange(PortRange rangeToDelete);
    bool checkIfPortBlocked(uint16_t port);
};


class CidrBlocker {
private:
    uint32_t cidrBlockage; // if the nth bit is set the cidr block n is not allowed
    // checks if a cidr block is valid
    bool static checkCidr(unsigned char);
public:
    CidrBlocker();
    bool BlockCidr(unsigned char); // blocks the specified cidr block
    bool AllowCidr(unsigned char); // unblocks the specified cidr block
    // returns 
    CidrAllowanceStatus IsCidrBlocked(unsigned char); // checks if the specified cidr block is set
};


