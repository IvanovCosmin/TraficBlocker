#include "TrafficChecker.h"

// checks if a cidr block is valid
bool TrafficChecker::checkCidr(unsigned char cidr) {
    if (cidr > 32 || cidr == 0) {
        SHOW_ERROR("The specified cidr block: %d is not valid\n", cidr);
        return false;
    }
    return true;
}
TrafficChecker::TrafficChecker() {
    cidrBlockage = 0;
}

/// blocks a specified cidr block
/// returns false when a bad cidr block is given
bool TrafficChecker::BlockCidr(unsigned char cidr) {
    if (!checkCidr(cidr)) return false;
    BitOperations::SetBit<uint32_t>(cidrBlockage, cidr - 1);
    return true;
}


/// allows a specified cidr block
/// returns false when a bad cidr block is given
bool TrafficChecker::AllowCidr(unsigned char cidr) {
    if (!checkCidr(cidr)) return false;
    BitOperations::UnsetBit<uint32_t>(cidrBlockage, cidr - 1);
    return true;
}

// can return CIDR_ALLOWED or CIDR_BLOCKED; returns CIDR_ERROR if a bad CIDR is given.
CidrAllowanceStatus TrafficChecker::IsCidrBlocked(unsigned char cidr) {
    if (!TrafficChecker::checkCidr(cidr)) return CidrAllowanceStatus::CIDR_ERROR;
    if (BitOperations::IsBitSet<uint32_t>(cidrBlockage, cidr - 1)) {
        return CidrAllowanceStatus::CIDR_BLOCKED;
    }
    return CidrAllowanceStatus::CIDR_ALLOWED;
}