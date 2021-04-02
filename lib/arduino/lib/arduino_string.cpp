#include <iostream>
#include <string>
#include "arduino_string.hpp"

my::String::String()
{
}

my::String::~String()
{
}

my::String::String(const std::string &str)
{
    this->data = str;
}

my::String my::String::operator+(const int &rhs)
{
    my::String lhs(data);
    lhs += this->to_string(rhs);
    return lhs;
}

my::String my::String::operator+(const unsigned int &rhs)
{
    my::String lhs(this->data);
    lhs += this->to_string(rhs);
    return lhs;
}

my::String my::String::operator+(const char &rhs)
{
    my::String lhs(this->data + rhs);
    return lhs;
}

my::String &my::String::operator=(const std::string &rhs)
{
    this->set_data(rhs);
    return *this;
}

/*
void my::String::operator+=(const int &rhs)
{
    this->data += std::to_string(rhs);
}
*/

my::String & my::String::operator+=(const unsigned int &rhs)
{
    this->set_data(this->get_data() + std::to_string(rhs));
    return *this;
}

my::String & my::String::operator+=(const std::string &rhs)
{
    this->set_data(this->get_data() + rhs);
    return *this;
}

my::String & my::String::operator+=(const my::String &rhs)
{
    this->set_data(this->get_data() + rhs.get_data());
    return *this;
}

my::String &my::String::operator+=(const int &rhs)
{
    this->set_data(this->get_data() + std::to_string(rhs));
    return *this;
}

my::String my::String::to_string(const int &rhs)
{
    my::String str(std::to_string(rhs));
    return str;
}

my::String my::String::to_string(const unsigned int &rhs)
{
    my::String str(std::to_string(rhs));
    return str;
}

void my::String::concat(const int &i)
{
    this->data += std::to_string(i);
}

void my::String::concat(const unsigned int &i)
{
    this->data += std::to_string(i);
}

std::string my::String::get_data() const
{
    return this->data;
}

void my::String::set_data(const std::string &str)
{
    this->data = str;
}

/*
std::ostream& operator<<(std::ostream& os, const myclass& obj)
{
      os << obj.somevalue;
      return os;
}
*/

/*
std::ostream &my::String::operator<<(std::ostream &os) 
{ 
    return os << this->data;
}
*/
/*
my::String::operator std::string() const 
{ 
    return this->data;
}
*/
//#define digitalWrite(pin, value) digitalWrite_standard(pin, value)

/*
template<typename _CharT, typename _Traits, typename _Alloc> void std::__cxx11::basic_string<_CharT, _Traits, _Alloc>::concat(int v)
{

}
*/