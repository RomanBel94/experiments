#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <format>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <regex>
#include <source_location>
#include <sstream>

class Timer final
{
    using clock = std::chrono::steady_clock;
    using time_point = std::chrono::steady_clock::time_point;
    using duration = std::chrono::duration<double>;

public:
    template <class Callable, typename... Args>
    inline void operator()(Callable&& func_object, Args&&... args) const
    {
        time_point start{clock::now()};
        func_object(std::forward<Args>(args)...);
        duration time_passed{
            std::chrono::duration_cast<duration>(clock::now() - start)};

        std::clog << std::format("Time passed: {:.5}s\n", time_passed.count());
    }

    template <typename ReturnType, class Callable, typename... Args>
    inline ReturnType operator()(Callable&& func_object, Args&&... args) const
    {
        time_point start{clock::now()};
        ReturnType temp = func_object(std::forward<Args>(args)...);
        duration time_passed{
            std::chrono::duration_cast<duration>(clock::now() - start)};

        std::clog << std::format("Time passed: {:.5}s\n", time_passed.count());

        return temp;
    }
};

class IPaddress final
{
private:
    std::array<uint8_t, 4> octets{};
    static constexpr auto IP_REGEX =
        R"(((\d|1\d\d|25[0-5]|2[0-4]\d|[1-9]\d)\.){3}(1\d\d|25[0-5]|2[0-4]\d|[1-9]\d|\d))";

public:
    IPaddress(uint8_t oct3, uint8_t oct2, uint8_t oct1, uint8_t oct0)
    {
        set_octet(0, oct0);
        set_octet(1, oct1);
        set_octet(2, oct2);
        set_octet(3, oct3);
    };

    explicit IPaddress(uint32_t byte_view)
    {
        uint8_t current_octet{0};
        for (size_t i{0}; i < 4; ++i)
            std::ranges::for_each(
                std::views::iota(0, 4), [this, current_octet, byte_view](auto i)
                { set_octet(i, current_octet | byte_view >> i * 8); });
    }

    IPaddress() = default;
    IPaddress(const IPaddress&) = default;
    IPaddress(IPaddress&&) noexcept = default;
    IPaddress& operator=(const IPaddress&) = default;
    IPaddress& operator=(IPaddress&&) noexcept = default;
    ~IPaddress() = default;

    inline std::string to_string() const noexcept
    {
        return std::format("{}.{}.{}.{}", get_octet(3), get_octet(2),
                           get_octet(1), get_octet(0));
    }

    inline uint32_t to_uint32() const noexcept
    {
        uint32_t result{0};
        for (int i{3}; i >= 0; --i)
        {
            result |= get_octet(i);
            if (i > 0)
                result <<= 8;
        }

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
        std::regex ip_regex{IP_REGEX};
        if (!std::regex_match(addr, ip_regex))
        {
            throw std::invalid_argument{std::format(
                "{}({}): bad ip address",
                std::source_location::current().function_name(), addr)};
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
    for (Integer mask{static_cast<Integer>(1 << offset)}, counter{1}; mask;
         mask >>= 1, ++counter)
    {
        std::cout << static_cast<bool>(num & mask);

        // every 8th bit and not last
        if (counter % 8 == 0 && mask != 1)
            std::cout << " |";
        std::cout << ' ';
    }
}

inline void show_octets(const IPaddress& addr)
{
    for (int i{3}; i >= 0; --i)
    {
        std::cout << std::format("Octet {}: ({:>3}) ", i,
                                 static_cast<uint16_t>(addr.get_octet(i)));
        show_bits(addr.get_octet(i));
        std::cout << '\n';
    }
}

inline void show_full(const IPaddress& addr)
{
    std::cout << std::format("Full address: ({}) ", addr.to_uint32());
    show_bits(addr.to_uint32());
}

inline void describe_IP(const IPaddress& addr)
{
    std::cout << addr << '\n';
    show_octets(addr);

    show_full(addr);
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
