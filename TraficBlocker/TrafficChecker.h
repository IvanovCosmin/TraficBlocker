#pragma once
#include "utils.h"
#include "bitoperations.h"
#include <cstdint>

enum class CidrAllowanceStatus {
    CIDR_BLOCKED,
    CIDR_ALLOWED,
    CIDR_ERROR
};

class TrafficChecker {
private:
    uint32_t cidrBlockage; // if the nth bit is set the cidr block n is not allowed
    // checks if a cidr block is valid
    bool static checkCidr(unsigned char);
public:
    TrafficChecker();
    bool BlockCidr(unsigned char); // blocks the specified cidr block
    bool AllowCidr(unsigned char); // unblocks the specified cidr block
    // returns 
    CidrAllowanceStatus IsCidrBlocked(unsigned char); // checks if the specified cidr block is set
};


