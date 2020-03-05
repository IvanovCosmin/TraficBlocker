#pragma once
#include <assert.h>


namespace BitOperations {

    template <typename T>
    bool IsBitSet(T bits, unsigned char bitIndex) {
        assert(bitIndex < sizeof(T) * 8);
        return (bool)(bits & (1 << bitIndex));
    }

    template <typename T>
    void UnsetBit(T& bits, unsigned char bitIndex) {
        assert(bitIndex < sizeof(T) * 8);
        bits &= ~(1 << (bitIndex));
    }

    template <typename T>
    void SetBit(T& bits, unsigned char bitIndex) {
        assert(bitIndex < sizeof(T) * 8);
        bits |= (1 << (bitIndex));
    }
}