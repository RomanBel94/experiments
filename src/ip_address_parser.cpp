#include <array>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

class IPaddress
{
private:
    std::array<uint8_t, 4> octets{};

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
        {
            current_octet |= byte_view >> i * 8;
            set_octet(i, current_octet);
            current_octet = 0;
        }
    }

    IPaddress() = default;
    IPaddress(const IPaddress&) = default;
    IPaddress(IPaddress&&) noexcept = default;
    IPaddress& operator=(const IPaddress&) = default;
    IPaddress& operator=(IPaddress&&) noexcept = default;
    ~IPaddress() = default;

    std::string get_string() const noexcept { return *this; }
    uint32_t get_byte_view() const noexcept { return *this; }
    uint8_t get_octet(size_t index) const noexcept { return octets.at(index); }
    void set_octet(size_t index, uint8_t octet) { octets.at(index) = octet; }

    operator std::string() const noexcept
    {
        std::ostringstream result;
        result << static_cast<uint16_t>(get_octet(3)) << '.'
               << static_cast<uint16_t>(get_octet(2)) << '.'
               << static_cast<uint16_t>(get_octet(1)) << '.'
               << static_cast<uint16_t>(get_octet(0));
        return result.str();
    }

    operator uint32_t() const noexcept
    {
        uint32_t result{0};
        result |= get_octet(3), result <<= 8;
        result |= get_octet(2), result <<= 8;
        result |= get_octet(1), result <<= 8;
        result |= get_octet(0);

        return result;
    }

    static void validate_ip(const std::string& addr)
    {
        std::regex ip_regex{R"(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})"};

        if (!std::regex_match(addr, ip_regex))
        {
            std::ostringstream message;
            message << __FUNCTION__ << "(" << addr << "): bad ip address";
            throw std::invalid_argument{message.str()};
        }
    }

    static IPaddress create_IP(const std::string& addr)
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

std::ostream& operator<<(std::ostream& stream, const IPaddress& addr) noexcept
{
    stream << addr.get_string();
    return stream;
}

template <typename Integer>
void show_bits(Integer num)
{
    Integer offset{sizeof(Integer) * 8 - 1};
    for (Integer ptr{static_cast<Integer>(1 << offset)}; ptr; ptr >>= 1)
    {
        if (num & ptr)
            std::cout << '1';
        else
            std::cout << '0';
        std::cout << ' ';
    }
}

void describe_IP(const IPaddress& addr)
{
    std::cout << addr << '\n';
    for (int i{3}; i >= 0; --i)
    {
        std::cout << "Octet" << i << ": ("
                  << static_cast<uint16_t>(addr.get_octet(i)) << ") ";
        show_bits(addr.get_octet(i));
        std::cout << '\n';
    }

    std::cout << "Full address: (" << addr.get_byte_view() << ") ";
    show_bits(addr.get_byte_view());
    std::cout << '\n';
}

int main(int argc, char* argv[])
{
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
            addr = IPaddress::create_IP(input);
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            continue;
        }

        describe_IP(addr);
    };

    return EXIT_SUCCESS;
}
