#pragma once
#pragma pack(push, 1)

/**
 * Pair.
 * 
 * Brief:
 * - Struct defines value type which contain values of types T1, T2
 * 
 * Memory usage:
 * sizeof(T1) + sizeof(T2)
 * 
 * Details:
 * - header use #pragma pack(push, 1) to avoid rounding memory to 8 bytes for each pair (default behaviour in c++)
 * 
 */
template <typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;

    Pair() = default;
    Pair(const T1 _first, const T2 _second): first(_first), second(_second) {}

    inline bool operator==(const Pair<T1, T2>& other) const {
        return (first == other.first) && (second == other.second);
    }

    inline bool operator!=(const Pair<T1, T2>& other) const {
        return !(*this == other);
    }
};

#pragma pack(pop)