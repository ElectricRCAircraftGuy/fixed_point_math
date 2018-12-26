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
    cp fixed_point_math2.cpp fixed_point_math2_copy.c && gcc -Wall -o fixed_point_math2_c fixed_point_math2_copy.c && ./fixed_point_math2_c
As a C++ program:
    g++ -Wall -o fixed_point_math2_cpp fixed_point_math2.cpp && ./fixed_point_math2_cpp

*/

#include <stdio.h>
#include <stdint.h>

#define BITS_PER_BYTE 8

#define FRACTION_BITS 16 // 1 << 16 = 2^16 = 65536
#define FRACTION_DIVISOR (1 << FRACTION_BITS)
#define FRACTION_MASK (FRACTION_DIVISOR - 1) // 65535 (all LSB set, all MSB clear)

// Conversions:
#define NUM_2_FIXED_PT(num)     (num << FRACTION_BITS)      // Regular number to fixed point number
#define FIXED_PT_2_NUM(fp_num)  (fp_num >> FRACTION_BITS)   // Fixed point number back to regular number

int main(int argc, char * argv[])
{
    printf("Begin.\n");

    // We know how many bytes we will use for the fraction, but how many are remaining for the whole number, and what's 
    // it's max range? Let's calculate it.
    const uint8_t WHOLE_NUM_BITS = sizeof(uint32_t)*BITS_PER_BYTE - FRACTION_BITS;
    const uint32_t MAX_WHOLE_NUM = (1 << WHOLE_NUM_BITS) - 1;
    printf("fraction bits = %u.\n", FRACTION_BITS);
    printf("whole number bits = %u.\n", WHOLE_NUM_BITS);
    printf("max whole number = %u.\n\n", MAX_WHOLE_NUM);

    // Create a variable called `price`, and let's do some fixed point math on it.
    uint32_t price = NUM_2_FIXED_PT(500);
    price += NUM_2_FIXED_PT(10);
    price *= 3;
    price /= 7; // now our price is ((500 + 10)*3/7) = 218.571428571.

    printf("price as integer is %u.\n", price >> FRACTION_BITS);
    printf("price fractional part is %u.\n", price & FRACTION_MASK);
    printf("price fractional part as decimal is %f.\n", (double)(price & FRACTION_MASK) / FRACTION_DIVISOR);

    return 0;
}
