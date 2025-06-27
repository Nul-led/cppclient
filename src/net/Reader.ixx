module;

#include <vector>
#include <string>
#include <stdexcept>
#include <type_traits>

export module Reader;

export struct Reader {
    std::vector<uint8_t> buffer;
    size_t at = 0;

    explicit Reader(std::vector<uint8_t> data) : buffer(std::move(data)) {}

    // read bytes
    void bytes(void* data, size_t size) {
        if (at + size > buffer.size()) {
            throw std::out_of_range("Buffer read out of range while reading bytes.");
        }
        memcpy(data, buffer.data() + at, size);
        at += size;
    }

    // read byte
    uint8_t byte() {
        if (at >= buffer.size()) {
            throw std::out_of_range("Buffer read out of range while reading a byte.");
        }
        return buffer[at++];
    }

    // null terminated c-style string
    std::string ntString() {
        const auto start_it = buffer.begin() + at;
        const auto end_it = std::find(start_it, buffer.end(), '\0');

        if (end_it == buffer.end()) {
            throw std::runtime_error("Unterminated string: could not find null terminator.");
        }

        const size_t len = std::distance(start_it, end_it);
        std::string result(reinterpret_cast<const char*>(buffer.data() + at), len);

        at += len + 1; // Advance past the string and the null terminator
        return result;
    }

    // u8 length prefixed string
    std::string shortString() {
        const uint8_t len = byte();
        if (bytesLeft() < len) {
            throw std::out_of_range("Buffer read out of range while reading short string content.");
        }
        std::string result(reinterpret_cast<const char*>(buffer.data() + at), len);
        at += len;
        return result;
    }

    // vu64 length prefixed string
    std::string varString() {
        const auto len = uleb128<uint64_t>();
        if (bytesLeft() < len) {
            throw std::out_of_range("Buffer read out of range while reading var-string content.");
        }
        std::string result(reinterpret_cast<const char*>(buffer.data() + at), len);
        at += len;
        return result;
    }

    [[nodiscard]] size_t bytesLeft() const {
        return buffer.size() - at;
    }

    // ENSURE SAME ENDIAN WHEN WRITING!
    template<typename T>
    T trivial() {
        T value;
        bytes(&value, sizeof(T));
        return value;
    }

    template<typename T>
    T uleb128() {
        static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>, "Cannot read uleb128() on a type that is not an unsigned integer");

        T result = 0;
        unsigned int shift = 0;
        uint8_t current_byte;

        do {
            if (shift >= sizeof(T) * 8) {
                throw std::overflow_error("uleb128 value too large for the requested type.");
            }

            current_byte = byte();
            result |= static_cast<T>(current_byte & 0x7F) << shift;
            shift += 7;
        } while (current_byte & 0x80);

        return result;
    }

    // two's-complement integer encoding
    template<typename T>
    T sleb128() {
        static_assert(std::is_integral_v<T> && std::is_signed_v<T>, "Cannot read sleb128() on a type that is not a signed integer");

        T result = 0;
        unsigned int shift = 0;
        uint8_t current_byte;
        constexpr unsigned int total_bits = sizeof(T) * 8;

        do {
            if (shift >= total_bits) {
                throw std::overflow_error("sleb128 value too large for the requested type.");
            }
            current_byte = byte();
            result |= static_cast<T>(current_byte & 0x7F) << shift;
            shift += 7;
        } while (current_byte & 0x80);

        if (shift < total_bits && (current_byte & 0x40)) {
            result |= (~static_cast<T>(0) << shift);
        }

        return result;
    }

    // zigzag integer encoding
    template<typename T>
    T zleb128() {
        static_assert(std::is_integral_v<T> && std::is_signed_v<T>, "Cannot read zleb128() on a type that is not a signed integer");

        using UnsignedT = std::make_unsigned_t<T>;
        const UnsignedT unsigned_val = uleb128<UnsignedT>();

        return static_cast<T>((unsigned_val >> 1) ^ (-(unsigned_val & 1)));
    }
};