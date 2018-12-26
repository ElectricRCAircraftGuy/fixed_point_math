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
    cp fixed_point_math3.cpp fixed_point_math3_copy.c && gcc -Wall -o fixed_point_math3_c fixed_point_math3_copy.c && ./fixed_point_math3_c
As a C++ program:
    g++ -Wall -o fixed_point_math3_cpp fixed_point_math3.cpp && ./fixed_point_math3_cpp

*/

#include <stdio.h>
#include <stdint.h>

#define BITS_PER_BYTE 8

#define FRACTION_BITS 16 // 1 << 16 = 2^16 = 65536
#define FRACTION_DIVISOR (1 << FRACTION_BITS)
#define FRACTION_MASK (FRACTION_DIVISOR - 1) // 65535 (all LSB set, all MSB clear)

// // Conversions [NEVERMIND, LET'S DO THIS MANUALLY INSTEAD OF USING THESE MACROS TO HELP ENGRAIN IT IN US BETTER]:
// #define INT_2_FIXED_PT_NUM(num)     (num << FRACTION_BITS)      // Regular integer number to fixed point number
// #define FIXED_PT_NUM_2_INT(fp_num)  (fp_num >> FRACTION_BITS)   // Fixed point number back to regular integer number

int main(int argc, char * argv[])
{
    printf("Begin.\n");

    // We know how many bits we will use for the fraction, but how many bits are remaining for the whole number, 
    // and what's the whole number's max range? Let's calculate it.
    const uint8_t WHOLE_NUM_BITS = sizeof(uint32_t)*BITS_PER_BYTE - FRACTION_BITS;
    const uint32_t MAX_WHOLE_NUM = (1 << WHOLE_NUM_BITS) - 1;
    printf("fraction bits = %u.\n", FRACTION_BITS);
    printf("whole number bits = %u.\n", WHOLE_NUM_BITS);
    printf("max whole number = %u.\n\n", MAX_WHOLE_NUM);

    // Create a variable called `price`, and let's do some fixed point math on it.
    uint32_t price = 500 << FRACTION_BITS;
    price += 10 << FRACTION_BITS;
    price *= 3;
    price /= 7; // now our price is ((500 + 10)*3/7) = 218.571428571.

    printf("price as a true double is %3.9f.\n", ((double)500 + 10)*3/7);
    printf("price as integer is %u.\n", price >> FRACTION_BITS);
    printf("price fractional part is %u (of %u).\n", price & FRACTION_MASK, FRACTION_DIVISOR);
    printf("price fractional part as decimal is %f.\n", (double)(price & FRACTION_MASK) / FRACTION_DIVISOR);

    // Now, if you don't have float support (neither in hardware via a Floating Point Unit [FPU], nor in software
    // via built-in floating point math libraries as part of your processor's C implementation), then you may have
    // to manually print the whole number and fractional number parts separately as follows. Be sure to make note
    // of the following points:
    // - 1) the digits after the decimal are determined by the multiplier: 
    //     0 digits: * 10^0 ==> * 1         <== 0 zeros
    //     1 digit : * 10^1 ==> * 10        <== 1 zero
    //     2 digits: * 10^2 ==> * 100       <== 2 zeros
    //     3 digits: * 10^3 ==> * 1000      <== 3 zeros
    //     4 digits: * 10^4 ==> * 10000     <== 4 zeros
    //     5 digits: * 10^5 ==> * 100000    <== 5 zeros
    // - 2) Be sure to use the proper printf format statement to enforce the proper number of leading zeros in front of
    //   the fractional part of the number. ie: refer to the "%01", "%02", "%03", etc. below.

    // 1 digit after the decimal
    printf("price (manual float, 1 digit  after decimal) is %u.%01lu.\n", price >> FRACTION_BITS, 
           (uint64_t)(price & FRACTION_MASK) * 10 / FRACTION_DIVISOR); 
    // 2 digits after decimal
    printf("price (manual float, 2 digits after decimal) is %u.%02lu.\n", price >> FRACTION_BITS, 
           (uint64_t)(price & FRACTION_MASK) * 100 / FRACTION_DIVISOR); 
    // 3 digits after decimal
    printf("price (manual float, 3 digits after decimal) is %u.%03lu.\n", price >> FRACTION_BITS, 
           (uint64_t)(price & FRACTION_MASK) * 1000 / FRACTION_DIVISOR); 
    // 4 digits after decimal
    printf("price (manual float, 4 digits after decimal) is %u.%04lu.\n", price >> FRACTION_BITS, 
           (uint64_t)(price & FRACTION_MASK) * 10000 / FRACTION_DIVISOR); 
    // 5 digits after decimal
    printf("price (manual float, 5 digits after decimal) is %u.%05lu.\n\n", price >> FRACTION_BITS, 
           (uint64_t)(price & FRACTION_MASK) * 100000 / FRACTION_DIVISOR); 


    // With rounding now:
    // - To do rounding with integers, during division, use the following formula:
    //   (dividend + divisor/2)/divisor.
    // 0 decimal places (nearest whole number)///////////////
    // uint32_t price_rounded = price * (10 + ((10/2) << FRACTION_BITS))/10;
    // printf("rounded price (manual float, 0 digits) is %u.\n", (price_rounded >> FRACTION_BITS)/10); 
    // 1 decimal place
    // 2 decimal places
    // 3 decimal places
    // 4 decimal places
    // 5 decimal places

    uint32_t price_rounded1 = price + (1 << (FRACTION_BITS - 0))/2/1; // round to 1 decimal digit
    uint32_t price_rounded2 = price + (1 << (FRACTION_BITS - 1))/2/10; // round to 2 decimal digits
    uint32_t price_rounded3 = price + (1 << (FRACTION_BITS - 2))/2/100; // round to 3 decimal digits
    uint32_t price_rounded4 = price + (1 << (FRACTION_BITS - 3))/2/1000; // round to 4 decimal digits
    uint32_t price_rounded5 = price + (1 << (FRACTION_BITS - 4))/2/10000; // round to 5 decimal digits

    printf("rounded price (manual float, 1 digit  after decimal) is %u.%01lu.\n", price_rounded1 >> FRACTION_BITS, 
           (uint64_t)(price_rounded1 & FRACTION_MASK) * 10 / FRACTION_DIVISOR); 
    printf("rounded price (manual float, 2 digits after decimal) is %u.%02lu.\n", price_rounded2 >> FRACTION_BITS, 
           (uint64_t)(price_rounded2 & FRACTION_MASK) * 100 / FRACTION_DIVISOR); 
    printf("rounded price (manual float, 3 digits after decimal) is %u.%03lu.\n", price_rounded3 >> FRACTION_BITS, 
           (uint64_t)(price_rounded3 & FRACTION_MASK) * 1000 / FRACTION_DIVISOR); 
    printf("rounded price (manual float, 4 digits after decimal) is %u.%04lu.\n", price_rounded4 >> FRACTION_BITS, 
           (uint64_t)(price_rounded4 & FRACTION_MASK) * 10000 / FRACTION_DIVISOR); 
    printf("rounded price (manual float, 5 digits after decimal) is %u.%05lu.\n\n", price_rounded5 >> FRACTION_BITS, 
           (uint64_t)(price_rounded5 & FRACTION_MASK) * 100000 / FRACTION_DIVISOR); 

    return 0;
}
