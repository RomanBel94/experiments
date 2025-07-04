#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

class Timer final
{
public:
    template <class Callable, typename... Args>
    inline void operator()(Callable&& func_object, Args&&... args) const
    {
        std::chrono::steady_clock::time_point start{
            std::chrono::steady_clock::now()};
        func_object(std::forward<Args>(args)...);
        std::chrono::steady_clock::time_point end{
            std::chrono::steady_clock::now()};
        std::chrono::duration<double> time_passed{
            std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                      start)};

        std::clog << "Time passed: " << std::fixed << std::setprecision(5)
                  << time_passed.count() << "ms\n";
    }

    template <typename ReturnType, class Callable, typename... Args>
    inline ReturnType operator()(Callable&& func_object, Args&&... args) const
    {
        std::chrono::steady_clock::time_point start{
            std::chrono::steady_clock::now()};
        ReturnType temp = func_object(std::forward<Args>(args)...);
        std::chrono::steady_clock::time_point end{
            std::chrono::steady_clock::now()};
        std::chrono::duration<double> time_passed{
            std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                      start)};
        std::cout << "Time passed: " << std::fixed << std::setprecision(5)
                  << time_passed.count() << "ms\n";

        return temp;
    }
};

class IPaddress final
{
private:
    std::array<uint8_t, 4> octets{};
    static constexpr auto VALID_IP_REGEX =
        R"(((\d|1\d{1,2}|2[0-5]{1,2}|2[0-5]\d|[1-9]\d)\.){3}(1\d{1,2}|2[0-5]{1,2}|2[0-]\d|[1-9]\d|\d))";

public:
    IPaddress(uint8_t oct3, uint8_t oct2, uint8_t oct1, uint8_t oct0)
    {
        set_octet(oct0, 0);
        set_octet(oct1, 1);
        set_octet(oct2, 2);
        set_octet(oct3, 3);
    };

    explicit IPaddress(uint32_t byte_view)
    {
        uint8_t current_octet{0};
        for (size_t i{0}; i < 4; ++i)
            set_octet(i, current_octet | byte_view >> i * 8);
    }

    IPaddress() = default;
    IPaddress(const IPaddress&) = default;
    IPaddress(IPaddress&&) noexcept = default;
    IPaddress& operator=(const IPaddress&) = default;
    IPaddress& operator=(IPaddress&&) noexcept = default;
    ~IPaddress() = default;

    inline std::string to_string() const noexcept
    {
        std::ostringstream result;
        result << static_cast<uint16_t>(get_octet(3)) << '.'
               << static_cast<uint16_t>(get_octet(2)) << '.'
               << static_cast<uint16_t>(get_octet(1)) << '.'
               << static_cast<uint16_t>(get_octet(0));
        return result.str();
    }

    inline uint32_t to_uint32() const noexcept
    {
        uint32_t result{0};
        result |= get_octet(3), result <<= 8;
        result |= get_octet(2), result <<= 8;
        result |= get_octet(1), result <<= 8;
        result |= get_octet(0);

        return result;
    }

    inline uint8_t get_octet(size_t index) const noexcept
    {
        return octets.at(index);
    }

    inline void set_octet(size_t index, uint8_t octet)
    {
        octets.at(index) = octet;
    }

    inline operator std::string() const noexcept { return to_string(); }

    inline operator uint32_t() const noexcept { return to_uint32(); }

    static inline void validate_ip(const std::string& addr)
    {
        std::regex ip_regex{VALID_IP_REGEX};
        if (!std::regex_match(addr, ip_regex))
        {
            std::ostringstream message;
            message << __FUNCTION__ << "(" << addr << "): bad ip address";
            throw std::invalid_argument{message.str()};
        }
    }

    static inline IPaddress create_IP(const std::string& addr)
    {
        validate_ip(addr);

        IPaddress result;
        std::istringstream istr{addr};
        std::string buffer;
        for (int i{3}; i >= 0; --i)
        {
            std::getline(istr, buffer, '.');
            result.set_octet(i, stoi(buffer));
        }
        return result;
    }
};

inline std::ostream& operator<<(std::ostream& stream,
                                const IPaddress& addr) noexcept
{
    stream << addr.to_string();
    return stream;
}

template <typename Integer>
inline void show_bits(Integer num)
{
    uint8_t offset{sizeof(Integer) * 8 - 1};
    for (Integer ptr{static_cast<Integer>(1 << offset)}; ptr; ptr >>= 1)
    {
        if (num & ptr)
            std::cout << '1';
        else
            std::cout << '0';
        std::cout << ' ';
    }
}

inline void describe_IP(const IPaddress& addr)
{
    std::cout << addr << '\n';
    for (int i{3}; i >= 0; --i)
    {
        std::cout << "Octet" << i << ": ("
                  << static_cast<uint16_t>(addr.get_octet(i)) << ") ";
        show_bits(addr.get_octet(i));
        std::cout << '\n';
    }

    std::cout << "Full address: (" << addr.to_uint32() << ") ";
    show_bits(addr.to_uint32());
    std::cout << '\n';
}

int main(int argc, char* argv[])
{
    Timer measure;
    IPaddress addr;
    std::string input;

    while (true)
    {
        std::cout << "Enter IP address (or type \"exit\"): ";
        std::getline(std::cin, input, '\n');
        if (input == "exit")
        {
            std::cout << "Exited by user\n";
            break;
        }

        try
        {
            addr = measure.operator()<IPaddress>(IPaddress::create_IP, input);
            describe_IP(addr);
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    };

    return EXIT_SUCCESS;
}
