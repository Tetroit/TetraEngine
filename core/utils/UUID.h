#pragma once
#include <random>

namespace TetraEngine {
    struct UUID
    {
        uint64_t high;
        uint64_t low;

        UUID() : high(0), low(0) {}

        static UUID Next()
        {
            static std::random_device rd;
            static std::mt19937_64 rng(rd());

            UUID id;
            id.high = rng();
            id.low = rng();

            // UUID version 4
            id.high &= ~(0xF000ULL << 32);
            id.high |=  (4ULL << 44);

            // RFC 4122 variant
            id.low &= ~(0xC000ULL << 48);
            id.low |=  (0x8000ULL << 48);
            return id;
        }
        explicit UUID(const std::string& str) {
            uint64_t chunk = 0;
            const char* ptr = str.c_str();

            chunk = 0;
            std::from_chars(ptr, ptr + 8, chunk, 16);
            ptr += 9;
            high = chunk << 32;

            chunk = 0;
            std::from_chars(ptr, ptr + 4, chunk, 16);
            ptr += 5;
            high |= (chunk << 16);

            chunk = 0;
            std::from_chars(ptr, ptr + 4, chunk, 16);
            ptr += 5;
            high |= chunk;

            chunk = 0;
            std::from_chars(ptr, ptr + 4, chunk, 16);
            ptr += 5;
            low = chunk << 48;

            chunk = 0;
            std::from_chars(ptr, ptr + 12, chunk, 16);
            low |= chunk;
        }
        std::string ToString()
        {
            char buffer[37];
            snprintf(
                buffer,
                sizeof(buffer),
                "%08llx-%04llx-%04llx-%04llx-%012llx",
                high >> 32,
                (high >> 16) & 0xffff,
                high & 0xffff,
                low >> 48,
                low & 0xffffffffffff
            );
            return buffer;
        }
        bool operator==(const UUID & other) const {
            return high == other.high && low == other.low;
        }
        bool operator!=(const UUID & other) const {
            return !(*this == other);
        }
    };
    struct UUIDHasher {
        size_t operator()(const UUID& id) const noexcept {
            return std::hash<uint64_t>{}(id.high) ^ std::hash<uint64_t>{}(id.low);
        }
    };

}