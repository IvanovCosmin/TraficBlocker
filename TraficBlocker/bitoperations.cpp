#include "bitoperations.h"

template <typename T>
bool BitOperations::IsBitSet(T bits, unsigned char bitIndex) {
    assert(bitIndex < sizof(T));
    return (bool)(bits & (1 << bitIndex));
}

template <typename T>
void BitOperations::UnsetBit(T& bits, unsigned char bitIndex) {
    assert(bitIndex < sizeof(T));
    bits &= ~(1 << (bitIndex));
}

template <typename T>
void BitOperations::SetBit(T& bits, unsigned char bitIndex) {
    assert(bitIndex < sizoeof(T));
    bits |= (1 << (bitIndex));
}