/*
fixed_point_math
- Practice doing decimal fractions and stuff using fixed point math and manual larger types, such as uint64_t on
  Arduino, or even uint128_t.

Gabriel Staples
Started: 22 Dec. 2018 
Updated: 23 Dec. 2018 

References:
- https://stackoverflow.com/questions/10067510/fixed-point-arithmetic-in-c-programming

Compile & Run:
As a C program (the file must NOT have a C++ file extension or it will be automatically compiled as C++):
See here: https://stackoverflow.com/a/3206195/4561887. 
    cp fixed_point_math5.cpp fixed_point_math5_copy.c && gcc -Wall -o fixed_point_math5_c fixed_point_math5_copy.c && ./fixed_point_math5_c
As a C++ program:
    g++ -Wall -o fixed_point_math5_cpp fixed_point_math5.cpp && ./fixed_point_math5_cpp

*/

#include <stdio.h>
#include <stdint.h>

// Define our fixed point type.
typedef uint32_t fixed_point_t;

#define BITS_PER_BYTE 8

#define FRACTION_BITS 16 // 1 << 16 = 2^16 = 65536
#define FRACTION_DIVISOR (1 << FRACTION_BITS)
#define FRACTION_MASK (FRACTION_DIVISOR - 1) // 65535 (all LSB set, all MSB clear)

// // Conversions [NEVERMIND, LET'S DO THIS MANUALLY INSTEAD OF USING THESE MACROS TO HELP ENGRAIN IT IN US BETTER]:
// #define INT_2_FIXED_PT_NUM(num)     (num << FRACTION_BITS)      // Regular integer number to fixed point number
// #define FIXED_PT_NUM_2_INT(fp_num)  (fp_num >> FRACTION_BITS)   // Fixed point number back to regular integer number

void print_if_error(fixed_point_t price)
{
    if ((price & FRACTION_MASK) > FRACTION_DIVISOR)
    {
        printf(" <== Error introduced here since the fractional number (%u) > %u.", 
               price & FRACTION_MASK, FRACTION_DIVISOR);
    }
    else
    {
        printf(" price & FRACTION_MASK = %u, FRACTION_DIVISOR = %u.", price & FRACTION_MASK, FRACTION_DIVISOR);
    }
    printf("\n");
}

int main(int argc, char * argv[])
{
    printf("Begin.\n");

    // We know how many bits we will use for the fraction, but how many bits are remaining for the whole number, 
    // and what's the whole number's max range? Let's calculate it.
    const uint8_t WHOLE_NUM_BITS = sizeof(fixed_point_t)*BITS_PER_BYTE - FRACTION_BITS;
    const fixed_point_t MAX_WHOLE_NUM = (1 << WHOLE_NUM_BITS) - 1;
    printf("fraction bits = %u.\n", FRACTION_BITS);
    printf("whole number bits = %u.\n", WHOLE_NUM_BITS);
    printf("max whole number = %u.\n\n", MAX_WHOLE_NUM);

    // Create a variable called `price`, and let's do some fixed point math on it.
    const fixed_point_t PRICE_ORIGINAL = 503;
    fixed_point_t price = PRICE_ORIGINAL << FRACTION_BITS;
    price += 10 << FRACTION_BITS;
    price *= 3;
    price /= 7; // now our price is ((500 + 10)*3/7) = 218.571428571.

    printf("price as a true double is %3.9f.\n", ((double)PRICE_ORIGINAL + 10)*3/7);
    printf("price as integer is %u.\n", price >> FRACTION_BITS);
    printf("price fractional part is %u (of %u).\n", price & FRACTION_MASK, FRACTION_DIVISOR);
    printf("price fractional part as decimal is %f.\n", (double)(price & FRACTION_MASK) / FRACTION_DIVISOR);

    // Now, if you don't have float support (neither in hardware via a Floating Point Unit [FPU], nor in software
    // via built-in floating point math libraries as part of your processor's C implementation), then you may have
    // to manually print the whole number and fractional number parts separately as follows. Be sure to make note
    // of the following 2 points:
    // - 1) the digits after the decimal are determined by the multiplier: 
    //     0 digits: * 10^0 ==> * 1         <== 0 zeros
    //     1 digit : * 10^1 ==> * 10        <== 1 zero
    //     2 digits: * 10^2 ==> * 100       <== 2 zeros
    //     3 digits: * 10^3 ==> * 1000      <== 3 zeros
    //     4 digits: * 10^4 ==> * 10000     <== 4 zeros
    //     5 digits: * 10^5 ==> * 100000    <== 5 zeros
    // - 2) Be sure to use the proper printf format statement to enforce the proper number of leading zeros in front of
    //   the fractional part of the number. ie: refer to the "%01", "%02", "%03", etc. below.
    // Manual "floats":
    // 0 digits after the decimal
    printf("price (manual float, 0 digits after decimal) is %u.", price >> FRACTION_BITS); 
    print_if_error(price);
    // 1 digit after the decimal
    printf("price (manual float, 1 digit  after decimal) is %u.%01lu.", price >> FRACTION_BITS, 
           (uint64_t)(price & FRACTION_MASK) * 10 / FRACTION_DIVISOR); 
    print_if_error(price);
    // 2 digits after decimal
    printf("price (manual float, 2 digits after decimal) is %u.%02lu.", price >> FRACTION_BITS, 
           (uint64_t)(price & FRACTION_MASK) * 100 / FRACTION_DIVISOR); 
    print_if_error(price);
    // 3 digits after decimal
    printf("price (manual float, 3 digits after decimal) is %u.%03lu.", price >> FRACTION_BITS, 
           (uint64_t)(price & FRACTION_MASK) * 1000 / FRACTION_DIVISOR); 
    print_if_error(price);
    // 4 digits after decimal
    printf("price (manual float, 4 digits after decimal) is %u.%04lu.", price >> FRACTION_BITS, 
           (uint64_t)(price & FRACTION_MASK) * 10000 / FRACTION_DIVISOR); 
    print_if_error(price);
    // 5 digits after decimal
    printf("price (manual float, 5 digits after decimal) is %u.%05lu.", price >> FRACTION_BITS, 
           (uint64_t)(price & FRACTION_MASK) * 100000 / FRACTION_DIVISOR); 
    print_if_error(price);
    // 6 digits after decimal
    printf("price (manual float, 6 digits after decimal) is %u.%06lu.\n", price >> FRACTION_BITS, 
           (uint64_t)(price & FRACTION_MASK) * 1000000 / FRACTION_DIVISOR); 
    print_if_error(price);


    // Manual "floats" ***with rounding now***:
    // - To do rounding with integers, during division, use the following formula:
    //   (dividend + divisor/2)/divisor.
    // 0 decimal places (nearest whole number)///////////////
    // fixed_point_t price_rounded = price * (10 + ((10/2) << FRACTION_BITS))/10;
    // printf("rounded price (manual float, 0 digits) is %u.\n", (price_rounded >> FRACTION_BITS)/10); 
    // 1 decimal place
    // 2 decimal places
    // 3 decimal places
    // 4 decimal places
    // 5 decimal places

    ///////////////TODO: 1
    // whole digit: add 1/2, otherwise known as 5/10, 50/100, 500/1000, etc.
    // 1) make this a git tracked repo
    // 2) 1 digit: add 5/10 * FRACTION_DIVISOR
    //    2: add 5/100 * FRACTION_DIVISOR
    //    3: add 5/1000 * FRACTINO_DIVISOR
    //    4: add 5/10000 * FRACTION_DIVISOR 
    // etc


    // Calculate addends used for rounding.
    // Note: "addend" = "a number that is added to another" 
    // (see https://www.google.com/search?q=addend&oq=addend&aqs=chrome.0.0l6.1290j0j7&sourceid=chrome&ie=UTF-8).
    fixed_point_t addend0 = FRACTION_DIVISOR/2;
    fixed_point_t addend1 = FRACTION_DIVISOR/20;
    fixed_point_t addend2 = FRACTION_DIVISOR/200;
    fixed_point_t addend3 = FRACTION_DIVISOR/2000;
    fixed_point_t addend4 = FRACTION_DIVISOR/20000;
    fixed_point_t addend5 = FRACTION_DIVISOR/200000;

    // Print addends used for rounding.
    printf("addend0 = %u.\n", addend0);
    printf("addend1 = %u.\n", addend1);
    printf("addend2 = %u.\n", addend2);
    printf("addend3 = %u.\n", addend3);
    printf("addend4 = %u.\n", addend4);
    printf("addend5 = %u.\n", addend5);

    // Calculate rounded prices
    fixed_point_t price_rounded0 = price + addend0; // round to 0 decimal digits
    fixed_point_t price_rounded1 = price + addend1; // round to 1 decimal digits
    fixed_point_t price_rounded2 = price + addend2; // round to 2 decimal digits
    fixed_point_t price_rounded3 = price + addend3; // round to 3 decimal digits
    fixed_point_t price_rounded4 = price + addend4; // round to 4 decimal digits
    fixed_point_t price_rounded5 = price + addend5; // round to 5 decimal digits

    // Print manually rounded prices of manually-printed fixed point integers as though they were "floats".
    printf("rounded price (manual float, 0 digits after decimal) is %u.\n", price_rounded0 >> FRACTION_BITS); 
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
