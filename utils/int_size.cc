#include<limits>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<cstdint>

int main(){
std::cout << "si:\t" << sizeof(short int);
std::cout << "\t" << std::numeric_limits<short int>::min();
std::cout << " .. " << std::numeric_limits<short int>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<short int>::max() << std::dec << std::endl;

std::cout << "usi:\t" << sizeof(unsigned short int);
std::cout << "\t" << std::numeric_limits<unsigned short int>::min();
std::cout << " .. " << std::numeric_limits<unsigned short int>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<unsigned short int>::max() << std::dec << std::endl;

std::cout << "i:\t" << sizeof(int);
std::cout << "\t" << std::numeric_limits<int>::min();
std::cout << " .. " << std::numeric_limits<int>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<int>::max() << std::dec << std::endl;

std::cout << "ui:\t" << sizeof(unsigned int);
std::cout << "\t" << std::numeric_limits<unsigned int>::min();
std::cout << " .. " << std::numeric_limits<unsigned int>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<unsigned int>::max() << std::dec << std::endl;

std::cout << "li:\t" << sizeof(long int);
std::cout << "\t" << std::numeric_limits<long int>::min();
std::cout << " .. " << std::numeric_limits<long int>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<long int>::max() << std::dec << std::endl;

std::cout << "uli:\t" << sizeof(unsigned long int);
std::cout << "\t" << std::numeric_limits<unsigned long int>::min();
std::cout << " .. " << std::numeric_limits<unsigned long int>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<unsigned long int>::max() << std::dec << std::endl;

std::cout << "lli:\t" << sizeof(long long int);
std::cout << "\t" << std::numeric_limits<long long int>::min();
std::cout << " .. " << std::numeric_limits<long long int>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<long long int>::max() << std::dec << std::endl;

std::cout << "ulli:\t" << sizeof(unsigned long long int);
std::cout << "\t" << std::numeric_limits<unsigned long long int>::min();
std::cout << " .. " << std::numeric_limits<unsigned long long int>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<unsigned long long int>::max() << std::dec << std::endl;

std::cout << "i16:\t" << sizeof(std::int16_t);
std::cout << "\t" << std::numeric_limits<std::int16_t>::min();
std::cout << " .. " << std::numeric_limits<std::int16_t>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<std::int16_t>::max() << std::dec << std::endl;

std::cout << "ui16:\t" << sizeof(std::uint16_t);
std::cout << "\t" << std::numeric_limits<std::uint16_t>::min();
std::cout << " .. " << std::numeric_limits<std::uint16_t>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<std::uint16_t>::max() << std::dec << std::endl;

std::cout << "i32:\t" << sizeof(std::int32_t);
std::cout << "\t" << std::numeric_limits<std::int32_t>::min();
std::cout << " .. " << std::numeric_limits<std::int32_t>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<std::int32_t>::max() << std::dec << std::endl;

std::cout << "ui32:\t" << sizeof(std::uint32_t);
std::cout << "\t" << std::numeric_limits<std::uint32_t>::min();
std::cout << " .. " << std::numeric_limits<std::uint32_t>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<std::uint32_t>::max() << std::dec << std::endl;

std::cout << "i64:\t" << sizeof(std::int64_t);
std::cout << "\t" << std::numeric_limits<std::int64_t>::min();
std::cout << " .. " << std::numeric_limits<std::int64_t>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<std::int64_t>::max() << std::dec << std::endl;

std::cout << "ui64:\t" << sizeof(std::uint64_t);
std::cout << "\t" << std::numeric_limits<std::uint64_t>::min();
std::cout << " .. " << std::numeric_limits<std::uint64_t>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<std::uint64_t>::max() << std::dec << std::endl;

std::cout << "imax:\t" << sizeof(std::intmax_t);
std::cout << "\t" << std::numeric_limits<std::intmax_t>::min();
std::cout << " .. " << std::numeric_limits<std::intmax_t>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<std::intmax_t>::max() << std::dec << std::endl;

std::cout << "uimax:\t" << sizeof(std::uintmax_t);
std::cout << "\t" << std::numeric_limits<std::uintmax_t>::min();
std::cout << " .. " << std::numeric_limits<std::uintmax_t>::max() << std::endl;
std::cout << std::hex << std::numeric_limits<std::uintmax_t>::max() << std::dec << std::endl;

return 1;
}
