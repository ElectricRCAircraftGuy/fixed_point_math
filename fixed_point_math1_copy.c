/*
fixed_point_math
- Practice doing decimal fractions and stuff using fixed point math and manual larger types, such as uint64_t on Arduino, or even uint128_t.

Gabriel Staples
22 Dec. 2018 

References:
- https://stackoverflow.com/questions/10067510/fixed-point-arithmetic-in-c-programming

Compile & Run:
As a C program (the file must NOT have a C++ file extension or it will be automatically compiled as C++):
See here: https://stackoverflow.com/a/3206195/4561887. 
    cp fixed_point_math1.cpp fixed_point_math1_copy.c && gcc -Wall -o fixed_point_math1_c fixed_point_math1_copy.c && ./fixed_point_math1_c
As a C++ program:
    g++ -Wall -o fixed_point_math1_cpp fixed_point_math1.cpp && ./fixed_point_math1_cpp

*/

#include <stdio.h>
#include <stdint.h>

#define SHIFT_AMOUNT 16 // 1 << 16 = 2^16 = 65536
#define SHIFT_DIVISOR (1 << SHIFT_AMOUNT)
#define SHIFT_MASK (SHIFT_DIVISOR - 1) // 65535 (all LSB set, all MSB clear)  

int main(int argc, char * argv[])
{
    printf("Begin.\n");

    uint32_t price = 500 << SHIFT_AMOUNT;
    price += 10 << SHIFT_AMOUNT;
    price *= 3;
    price /= 7; // now our price is ((500 + 10)*3/7) = 218.571428571.

    printf("price as integer is %u.\n", price >> SHIFT_AMOUNT);
    printf("price fractional part is %u.\n", price & SHIFT_MASK);
    printf("price fractional part as decimal is %f.\n", (double)(price & SHIFT_MASK) / SHIFT_DIVISOR);

    return 0;
}
