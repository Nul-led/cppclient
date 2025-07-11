module;

#include <vector>

export module Writer;

struct Writer {
    std::vector<uint8_t> buffer;

    explicit Writer(size_t reserve_capacity = 0) {
        if (reserve_capacity > 0) {
            buffer.reserve(reserve_capacity);
        }
    }

    void bytes(const void *data, const size_t size) {
        const auto* byte_data = static_cast<const uint8_t*>(data);
        buffer.insert(buffer.end(), byte_data, byte_data + size);
    }

    void ntString(const std::string& str) {
        bytes(str.data(), str.length());
        byte('\0');
    }

    void shortString(const std::string& str) {
        if (str.length() > UINT8_MAX) {
            throw std::length_error("String is too long for shortString (max 255 chars).");
        }
        byte(static_cast<uint8_t>(str.length()));
        bytes(str.data(), str.length());
    }

    void varString(const std::string& str) {
        uleb128<uint64_t>(str.length());
        bytes(str.data(), str.length());
    }

    void byte(const uint8_t b) {
        buffer.push_back(b);
    }

    // ENSURE SAME ENDIAN WHEN READING!
    template<typename T>
    void trivial(const T& value) {
        static_assert(std::is_trivially_copyable_v<T>, "Cannot write trivial() on a type that is not trivially copyable");
        bytes(&value, sizeof(T));
    }

    template<typename T>
    void uleb128(T value) {
        static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>, "Cannot write uleb128() on a type that is not an unsigned integer");

        do {
            uint8_t current_byte = value & 0x7F;
            value >>= 7;
            if (value != 0) {
                current_byte |= 0x80; // Set continuation bit
            }
            byte(current_byte);
        } while (value != 0);
    }

    template<typename T>
    void sleb128(T value) {
        static_assert(std::is_integral_v<T> && std::is_signed_v<T>, "Cannot write sleb128() on a type that is not a signed integer");

        bool more;
        do {
            uint8_t current_byte = value & 0x7F;
            value >>= 7; // Arithmetic shift for signed types

            // Check if the remaining bits are all just sign extensions.
            // For a positive number, we are done when value is 0 and the last byte's sign bit is 0.
            // For a negative number, we are done when value is -1 and the last byte's sign bit is 1.
            bool sign_bit_is_set = (current_byte & 0x40) != 0;
            if ((value == 0 && !sign_bit_is_set) || (value == -1 && sign_bit_is_set)) {
                more = false;
            } else {
                more = true;
                current_byte |= 0x80; // Set continuation bit
            }
            byte(current_byte);
        } while (more);
    }

    // zigzag encoded integer encoding, more efficient for smaller numbers than sleb128
    template<typename T>
    void zleb128(T value) {
        static_assert(std::is_integral_v<T> && std::is_signed_v<T>, "Cannot write zleb128() on a type that is not a signed integer");

        using UnsignedT = std::make_unsigned_t<T>;
        constexpr unsigned int bits = sizeof(T) * 8;

        // Zigzag encode the signed value into an unsigned one
        const UnsignedT unsigned_val = (static_cast<UnsignedT>(value) << 1) ^ (value >> (bits - 1));

        // Write the result as a standard uleb128
        uleb128<UnsignedT>(unsigned_val);
    }

    [[nodiscard]] size_t size() const {
        return buffer.size();
    }

    [[nodiscard]] const std::vector<uint8_t>& getBuffer() const {
        return buffer;
    }

    [[nodiscard]] std::vector<uint8_t> takeBuffer() {
        return std::move(buffer);
    }
};
