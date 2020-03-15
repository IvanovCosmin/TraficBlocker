#include "TrafficChecker.h"

// checks if a cidr block is valid
bool CidrBlocker::checkCidr(unsigned char cidr) {
    if (cidr > 32 || cidr == 0) {
        SHOW_ERROR("The specified cidr block: %d is not valid\n", cidr);
        return false;
    }
    return true;
}
CidrBlocker::CidrBlocker() {
    cidrBlockage = 0;
}

/// blocks a specified cidr block
/// returns false when a bad cidr block is given
bool CidrBlocker::BlockCidr(unsigned char cidr) {
    if (!checkCidr(cidr)) return false;
    BitOperations::SetBit<uint32_t>(cidrBlockage, cidr - 1);
    return true;
}


/// allows a specified cidr block
/// returns false when a bad cidr block is given
bool CidrBlocker::AllowCidr(unsigned char cidr) {
    if (!checkCidr(cidr)) return false;
    BitOperations::UnsetBit<uint32_t>(cidrBlockage, cidr - 1);
    return true;
}

// can return CIDR_ALLOWED or CIDR_BLOCKED; returns CIDR_ERROR if a bad CIDR is given.
CidrAllowanceStatus CidrBlocker::IsCidrBlocked(unsigned char cidr) {
    if (!CidrBlocker::checkCidr(cidr)) return CidrAllowanceStatus::CIDR_ERROR;
    if (BitOperations::IsBitSet<uint32_t>(cidrBlockage, cidr - 1)) {
        return CidrAllowanceStatus::CIDR_BLOCKED;
    }
    return CidrAllowanceStatus::CIDR_ALLOWED;
}

inline bool PortRange::CheckIfOverlapping(const PortRange range1, const PortRange range2) {
    if (range1.GetMax() >= range2.GetMin()) {
        return true;
    }
    return false;
}

inline PortRange::PortRange(uint16_t min, uint16_t max) {
    if (min > max) {
        SHOW_ERROR("Invalid port range, expected first argument to be smaller than the second one. Got %d and %d", min, max);
        SHOW_DEBUG("Inverting range arguments");
        this->max = min;
        this->min = max;

    }
    else {
        this->min = min;
        this->max = max;
    }
}

inline uint16_t PortRange::GetMin() const {
    return min;
}

inline uint16_t PortRange::GetMax() const {
    return max;
}

bool PortRange::operator<(const PortRange comparedTo) const
{
    if (this->GetMin() == comparedTo.GetMin()) return this->GetMax() < comparedTo.GetMax();
    return this->GetMin() < comparedTo.GetMin();
}

bool PortRange::operator==(const PortRange comparedTo) const
{
    return this->GetMin() == comparedTo.GetMin() && this->GetMax() == comparedTo.GetMax();
}

PortRange PortRange::operator+(PortRange const addedRange) const
{
    if (CheckIfOverlapping(*this, addedRange)) {
        return PortRange(this->GetMin(), max(addedRange.GetMax(), this->GetMax()));
    }
    else {
        SHOW_ERROR("Tried to add non-overlapping ranges");
        SHOW_DEBUG("Returning the first range");
        return *this;
    }
}

inline void PortBlocker::mergeOverlappingIntervals() {
    std::set<PortRange>::iterator iterator1 = blockedPortsSet.begin();
    std::set<PortRange>::iterator iterator2 = blockedPortsSet.begin(); // la start avem ambii iteratori la inceputul setului
    iterator2++; // iar al doilea va pleca de la al doilea element

    std::set<PortRange> mergedIntervals;
    bool mergedAnything = false;

    while (iterator1 != blockedPortsSet.end() && iterator2 != blockedPortsSet.end()) {
        if (PortRange::CheckIfOverlapping(*iterator1, *iterator2)) {
            mergedIntervals.insert((*iterator1) + (*iterator2)); // facem un nou interval
            std::set<PortRange>::iterator deleteIterator = iterator1;
            if (std::distance(iterator2, blockedPortsSet.end()) >= 2) { // verificam daca putem face mutarea
                iterator1 = std::next(iterator1, 2); //mutam ambele iteratoare cu 2 pozitii
                iterator2 = std::next(iterator2, 2);
            }
            else {
                iterator1 = blockedPortsSet.end();
                iterator2 = blockedPortsSet.end();
            }

            blockedPortsSet.erase(deleteIterator++); //stergem elementele care au fost lipite
            blockedPortsSet.erase(deleteIterator);
            mergedAnything = true;
        }
        else {
            // daca nu sunt overlapping mutam iteratorii cu cate o pozitie
            iterator1++;
            iterator2++;
        }
    }
    if (mergedAnything) {
        mergedIntervals.insert(blockedPortsSet.begin(), blockedPortsSet.end()); // adaugam ce a mai ramas in setul initial
        this->blockedPortsSet = mergedIntervals;
        mergeOverlappingIntervals();
    }

    return;

}

PortBlocker::PortBlocker() {
}

PortBlocker::PortBlocker(std::initializer_list<PortRange> ranges) {
    addRanges(ranges);
}

void PortBlocker::addRange(PortRange range) {
    blockedPortsSet.insert(range);
    mergeOverlappingIntervals();
}

void PortBlocker::addRanges(std::initializer_list<PortRange> ranges) {
    blockedPortsSet.insert(ranges);
    mergeOverlappingIntervals();
}

void PortBlocker::deleteRange(PortRange rangeToDelete) {
    std::set<PortRange> rangesToBeAdded;
    std::set<PortRange> rangesToBeDeleted;
    for (auto& range : this->blockedPortsSet) {
        if (range.GetMin() > rangeToDelete.GetMax()) {
            // nu are rost sa cautam mai departe
            break;
        }
        if (PortRange::CheckIfOverlapping(range, rangeToDelete)) {
            rangesToBeDeleted.insert(range);
            // avem 4 cazuri
            // cazul in care intervalul sters este in interiorul intervalului din care stergem.    stergem [10, 20] din  [5,25] -> [5-9], [21-25]
            if (rangeToDelete.GetMin() > range.GetMin() && rangeToDelete.GetMax() < range.GetMax()) {
                rangesToBeAdded.insert(PortRange(range.GetMin(), rangeToDelete.GetMin() - 1));
                rangesToBeAdded.insert(PortRange(rangeToDelete.GetMax() + 1, range.GetMax()));
            }

            //cazul in care intervalele coincid
            else if (range == rangeToDelete) {
                // nu trebuie sa facem nimic
                // deja am bagat rage-ul
            }

            // cazul in care intervalul sters e la inceputul intervalului din care stergem.   stergem [10,20] din [15,25] -> [21,25]
            else if (rangeToDelete.GetMax() < range.GetMax()) {
                rangesToBeAdded.insert(PortRange(rangeToDelete.GetMax() + 1, range.GetMax()));
            }
            // cazul in care intervalul sters e la sfarsitul intervalului din care stergem.   stergem [10,20] din [5, 15] -> [5, 9]
            else if (rangeToDelete.GetMax() >= range.GetMax()) {
                rangesToBeAdded.insert(PortRange(range.GetMin(), rangeToDelete.GetMin() - 1));
            }
        }
    }
    for (auto& range : rangesToBeDeleted) {
        this->blockedPortsSet.erase(range);
    }
    for (auto& range : rangesToBeAdded) {
        this->blockedPortsSet.insert(range);
    }

}

bool PortBlocker::checkIfPortBlocked(uint16_t port) {
    for (auto& range : this->blockedPortsSet) {
        // rangeurile sunt sortate dupa portul inferior (sau min). Daca minimul unuia depaseste portul cautat nu mai are rost sa verificam mai departe.
        if (range.GetMin() > port) {
            return false;
        }
        if (range.GetMin() <= port && port <= range.GetMax()) {
            // portul se afla intr un interval blocat
            return true;
        }
    }
    return false;
}
