#include <iostream>
#include <cstdint>
#include <map>


template <typename T>
class UintProperty : public PropertyBase
{
public:
    using ValueType = T;

    UintProperty(IPropertySet& container, uint8_t index, const std::string& label)
    : PropertyBase(container, index, label) {}

    bool read(std::istream& is) override
    {        
        std::cout << "read " << uint16_t(index()) << "\n";
        return false;
    }

    bool write(std::ostream& os) const override
    {        
        return false;
    }

    bool print(std::ostream& os, uint16_t indent) const override
    {        
        return false;
    }

    bool parse(TokenStream& is) override
    {   
        std::cout << "parse " << uint16_t(index()) << "\n";
        return false;
    }

private:
    ValueType m_value;
};


using Uint8Property  = UintProperty<uint8_t>;
using Uint16Property = UintProperty<uint16_t>;




class Airport : public PropertySet
{
private:
    Uint8Property  prop1{*this, 0x01, "prop1"};
    Uint16Property prop2{*this, 0x02, "prop2"};
};


int main()
{
    Airport airport;
    airport.read_property(std::cin, 0x01);
    airport.read_property(std::cin, 0x02);

    uint8_t index;
    airport.parse_property(std::cin, "prop1", index);
    airport.parse_property(std::cin, "prop2", index);
}

