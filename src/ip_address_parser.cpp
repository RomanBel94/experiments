#include <array>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

class IPaddress
{
private:
    uint8_t octet0 = 0;
    uint8_t octet1 = 0;
    uint8_t octet2 = 0;
    uint8_t octet3 = 0;

public:
    IPaddress(uint8_t oct3, uint8_t oct2, uint8_t oct1, uint8_t oct0)
        : octet3{oct3}, octet2{oct2}, octet1{oct1}, octet0{oct0} {};
    IPaddress() = default;
    IPaddress(const IPaddress&) = default;
    IPaddress(IPaddress&&) noexcept = default;
    IPaddress& operator=(const IPaddress&) = default;
    IPaddress& operator=(IPaddress&&) = default;
    ~IPaddress() = default;

    std::string get_address() const noexcept { return *this; }
    uint32_t get_byte_view() const noexcept { return *this; }
    uint8_t get_octet0() const noexcept { return octet0; }
    uint8_t get_octet1() const noexcept { return octet1; }
    uint8_t get_octet2() const noexcept { return octet2; }
    uint8_t get_octet3() const noexcept { return octet3; }

    operator std::string() const noexcept
    {
        std::ostringstream result;
        result << static_cast<uint16_t>(get_octet3()) << '.'
               << static_cast<uint16_t>(get_octet2()) << '.'
               << static_cast<uint16_t>(get_octet1()) << '.'
               << static_cast<uint16_t>(get_octet0());
        return result.str();
    }

    operator uint32_t() const noexcept
    {
        uint32_t result{0};
        result |= octet0 | octet1 << 8 | octet2 << 16 | octet3 << 24;

        return result;
    }

    static IPaddress create_IP(const std::string& addr)
    {
        std::regex ip_regex{R"(\d+\.\d+\.\d+\.\d+)"};
        if (!std::regex_match(addr, ip_regex))
        {
            throw std::invalid_argument{__FUNCTION__ "(): bad ip address"};
        }
        std::array<uint8_t, 4> octs{};
        std::istringstream istr{addr};
        std::string temp;
        for (int i{0}; i < 4; ++i)
        {
            std::getline(istr, temp, '.');
            octs.at(3 - i) = std::stoi(temp);
        }
        return IPaddress{octs[3], octs[2], octs[1], octs[0]};
    }
};

std::ostream& operator<<(std::ostream& stream, const IPaddress& addr) noexcept
{
    stream << addr.get_address();
    return stream;
}

int main(int argc, char* argv[])
{
    IPaddress addr;
    try
    {
        addr = IPaddress::create_IP("10.243.25.168");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::cout << addr << '\n';
    return EXIT_SUCCESS;
}
