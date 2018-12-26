/*
fixed_point_math tutorial
- A tutorial-like practice code to learn how to do fixed-point math, manual "float"-like prints using integers only,
  "float"-like integer rounding, and fractional fixed-point math on large integers. 

By Gabriel Staples
www.ElectricRCAircraftGuy.com
- email available via the Contact Me link at the top of my website.
Started: 22 Dec. 2018 
Updated: 25 Dec. 2018 

References:
- https://stackoverflow.com/questions/10067510/fixed-point-arithmetic-in-c-programming

Commands to Compile & Run:
As a C program (the file must NOT have a C++ file extension or it will be automatically compiled as C++, so we will
make a copy of it and change the file extension to .c first):
See here: https://stackoverflow.com/a/3206195/4561887. 
    cp fixed_point_math.cpp fixed_point_math_copy.c && gcc -Wall -std=c99 -o ./bin/fixed_point_math_c fixed_point_math_copy.c && ./bin/fixed_point_math_c
As a C++ program:
    g++ -Wall -o ./bin/fixed_point_math_cpp fixed_point_math.cpp && ./bin/fixed_point_math_cpp

*/

#include <stdbool.h>
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

// Private function prototypes:
static void print_if_error_introduced(uint8_t num_digits_after_decimal);

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
    printf("price fractional part as decimal is %f (%u/%u).\n", (double)(price & FRACTION_MASK) / FRACTION_DIVISOR,
           price & FRACTION_MASK, FRACTION_DIVISOR);

    // Now, if you don't have float support (neither in hardware via a Floating Point Unit [FPU], nor in software
    // via built-in floating point math libraries as part of your processor's C implementation), then you may have
    // to manually print the whole number and fractional number parts separately as follows. Look for the patterns.
    // Be sure to make note of the following 2 points:
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
    printf("price (manual float, 0 digits after decimal) is %u.", 
           price >> FRACTION_BITS); print_if_error_introduced(0);
    // 1 digit after the decimal
    printf("price (manual float, 1 digit  after decimal) is %u.%01lu.", 
           price >> FRACTION_BITS, (uint64_t)(price & FRACTION_MASK) * 10 / FRACTION_DIVISOR); 
    print_if_error_introduced(1);
    // 2 digits after decimal
    printf("price (manual float, 2 digits after decimal) is %u.%02lu.", 
           price >> FRACTION_BITS, (uint64_t)(price & FRACTION_MASK) * 100 / FRACTION_DIVISOR); 
    print_if_error_introduced(2);
    // 3 digits after decimal
    printf("price (manual float, 3 digits after decimal) is %u.%03lu.", 
           price >> FRACTION_BITS, (uint64_t)(price & FRACTION_MASK) * 1000 / FRACTION_DIVISOR); 
    print_if_error_introduced(3);
    // 4 digits after decimal
    printf("price (manual float, 4 digits after decimal) is %u.%04lu.", 
           price >> FRACTION_BITS, (uint64_t)(price & FRACTION_MASK) * 10000 / FRACTION_DIVISOR); 
    print_if_error_introduced(4);
    // 5 digits after decimal
    printf("price (manual float, 5 digits after decimal) is %u.%05lu.", 
           price >> FRACTION_BITS, (uint64_t)(price & FRACTION_MASK) * 100000 / FRACTION_DIVISOR); 
    print_if_error_introduced(5);
    // 6 digits after decimal
    printf("price (manual float, 6 digits after decimal) is %u.%06lu.", 
           price >> FRACTION_BITS, (uint64_t)(price & FRACTION_MASK) * 1000000 / FRACTION_DIVISOR); 
    print_if_error_introduced(6);
    printf("\n");


    // Manual "floats" ***with rounding now***:
    // - To do rounding with integers, the concept is best understood by examples: 
    // BASE 10 CONCEPT:
    // 1. To round to the nearest whole number: 
    //    Add 1/2 to the number, then let it be truncated since it is an integer. 
    //    Examples:
    //      1.5 + 1/2 = 1.5 + 0.5 = 2.0. Truncate it to 2. Good!
    //      1.99 + 0.5 = 2.49. Truncate it to 2. Good!
    //      1.49 + 0.5 = 1.99. Truncate it to 1. Good!
    // 2. To round to the nearest tenth place:
    //    Multiply by 10 (this is equivalent to doing a single base-10 left-shift), then add 1/2, then let 
    //    it be truncated since it is an integer, then divide by 10 (this is a base-10 right-shift).
    //    Example:
    //      1.57 x 10 + 1/2 = 15.7 + 0.5 = 16.2. Truncate to 16. Divide by 10 --> 1.6. Good.
    // 3. To round to the nearest hundredth place:
    //    Multiply by 100 (base-10 left-shift 2 places), add 1/2, truncate, divide by 100 (base-10 
    //    right-shift 2 places).
    //    Example:
    //      1.579 x 100 + 1/2 = 157.9 + 0.5 = 158.4. Truncate to 158. Divide by 100 --> 1.58. Good.
    //
    // BASE 2 CONCEPT:
    // - We are dealing with fractional numbers stored in base-2 binary bits, however, and we have already 
    //   left-shifted by FRACTION_BITS (num << FRACTION_BITS) when we converted our numbers to fixed-point 
    //   numbers. Therefore, *all we have to do* is add the proper value, and we get the same effect when we 
    //   right-shift by FRACTION_BITS (num >> FRACTION_BITS) in our conversion back from fixed-point to regular
    //   numbers. Here's what that looks like for us:
    // - Note: "addend" = "a number that is added to another".
    //   (see https://www.google.com/search?q=addend&oq=addend&aqs=chrome.0.0l6.1290j0j7&sourceid=chrome&ie=UTF-8).
    // - Rounding to 0 digits means simply rounding to the nearest whole number.
    // Round to:        Addends:
    // 0 digits: add 5/10 * FRACTION_DIVISOR       ==> + FRACTION_DIVISOR/2
    // 1 digits: add 5/100 * FRACTION_DIVISOR      ==> + FRACTION_DIVISOR/20
    // 2 digits: add 5/1000 * FRACTION_DIVISOR     ==> + FRACTION_DIVISOR/200
    // 3 digits: add 5/10000 * FRACTION_DIVISOR    ==> + FRACTION_DIVISOR/2000
    // 4 digits: add 5/100000 * FRACTION_DIVISOR   ==> + FRACTION_DIVISOR/20000
    // 5 digits: add 5/1000000 * FRACTION_DIVISOR  ==> + FRACTION_DIVISOR/200000
    // 6 digits: add 5/10000000 * FRACTION_DIVISOR ==> + FRACTION_DIVISOR/2000000
    // etc.

    printf("WITH MANUAL INTEGER-BASED ROUNDING:\n");

    // Calculate addends used for rounding (see definition of "addend" above).
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
    printf("rounded price (manual float, rounded to 0 digits after decimal) is %u.\n", 
           price_rounded0 >> FRACTION_BITS); 
    printf("rounded price (manual float, rounded to 1 digit  after decimal) is %u.%01lu.\n", 
           price_rounded1 >> FRACTION_BITS, (uint64_t)(price_rounded1 & FRACTION_MASK) * 10 / FRACTION_DIVISOR); 
    printf("rounded price (manual float, rounded to 2 digits after decimal) is %u.%02lu.\n", 
           price_rounded2 >> FRACTION_BITS, (uint64_t)(price_rounded2 & FRACTION_MASK) * 100 / FRACTION_DIVISOR); 
    printf("rounded price (manual float, rounded to 3 digits after decimal) is %u.%03lu.\n", 
           price_rounded3 >> FRACTION_BITS, (uint64_t)(price_rounded3 & FRACTION_MASK) * 1000 / FRACTION_DIVISOR); 
    printf("rounded price (manual float, rounded to 4 digits after decimal) is %u.%04lu.\n", 
           price_rounded4 >> FRACTION_BITS, (uint64_t)(price_rounded4 & FRACTION_MASK) * 10000 / FRACTION_DIVISOR); 
    printf("rounded price (manual float, rounded to 5 digits after decimal) is %u.%05lu.\n", 
           price_rounded5 >> FRACTION_BITS, (uint64_t)(price_rounded5 & FRACTION_MASK) * 100000 / FRACTION_DIVISOR); 


    // =================================================================================================================

    // RELATED CONCEPTS:
    // Now let's practice handling (doing math on) large integers (ie: large relative to their integer type),
    // withOUT resorting to using larger integer types (because they may not exist for our target processor), 
    // and withOUT using floating point math, since that might also either not exist for our processor, or be too
    // slow or program-space-intensive for our application.
    // - These concepts are especially useful when you hit the limits of your architecture's integer types: ex: 
    //   if you have a uint64_t nanosecond timestamp that is really large, and you need to multiply it by a fraction
    //   to convert it, but you don't have uint128_t types available to you to multiply by the numerator before 
    //   dividing by the denominator. What do you do?
    // - We can use fixed-point math to achieve desired results. Let's look at various approaches.
    // - Let's say my goal is to multiply a number by a fraction < 1 withOUT it ever growing into a larger type.
    // - Essentially we want to multiply some really large value (near its range limit for its integer type)
    //   by some_number/some_larger_number (ie: a fraction < 1). The problem is that if we multiply by the numerator
    //   first, it will overflow, and if we divide by the denominator first we will lose resolution via bits 
    //   right-shifting out.
    // Here are various examples and approaches.

    // -----------------------------------------------------
    // Goal: Use only 16-bit values & math to find 65401 * 16/127.
    // Result: Great! All 3 approaches work, with the 3rd being the best. 
    uint16_t num16 = 65401; // 1111 1111 0111 1001 
    uint16_t times = 16;
    uint16_t divide = 127;
    
    // Find the true answer.
    // First, let's cheat to know the right answer by letting it grow into a larger type. 
    // Multiply *first* (before doing the divide) to avoid losing resolution.
    printf("\n%u * %u/%u = %u. <== true answer\n", num16, times, divide, (uint32_t)num16*times/divide);
    
    // 1st approach: just divide first to prevent overflow, and lose precision right from the start.
    uint16_t num16_result = num16/divide * times;
    printf("1st approach:\n");
    printf("num16_result = %u. <== Loses bits that right-shift out during the initial divide.\n", num16_result);
    
    // 2nd approach: split the 16-bit number into 2 8-bit numbers stored in 16-bit numbers, 
    // placing all 8 bits of each sub-number to the ***far right***, with 8 bits on the left to grow
    // into when multiplying. Then, multiply and divide each part separately. 
    // - The problem, however, is that you'll lose meaningful resolution on the upper-8-bit number when you 
    //   do the division, since there's no bits to the right for the right-shifted bits during division to 
    //   be retained in.
    // Re-sum both sub-numbers at the end to get the final result. 
    // - NOTE THAT 257 IS THE HIGHEST *TIMES* VALUE I CAN USE SINCE 2^16/0b0000,0000,1111,1111 = 65536/255 = 257.00392.
    //   Therefore, any *times* value larger than this will cause overflow.
    uint16_t num16_upper8 = num16 >> 8; // 1111 1111
    uint16_t num16_lower8 = num16 & 0xFF; // 0111 1001
    num16_upper8 *= times;
    num16_lower8 *= times;
    num16_upper8 /= divide;
    num16_lower8 /= divide;
    num16_result = (num16_upper8 << 8) + num16_lower8;
    printf("2nd approach:\n");
    printf("num16_result = %u. <== Loses bits that right-shift out during the divide.\n", num16_result);
    
    // 3rd approach: split the 16-bit number into 2 8-bit numbers stored in 16-bit numbers, 
    // placing all 8 bits of each sub-number ***in the center***, with 4 bits on the left to grow when 
    // multiplying and 4 bits on the right to not lose as many bits when dividing. 
    // This will help stop the loss of resolution when we divide, at the cost of overflowing more easily when we 
    // multiply.
    // - NOTE THAT 16 IS THE HIGHEST *TIMES* VALUE I CAN USE SINCE 2^16/0b0000,1111,1111,0000 = 65536/4080 = 16.0627.
    //   Therefore, any *times* value larger than this will cause overflow.
    num16_upper8 = (num16 >> 4) & 0x0FF0;
    num16_lower8 = (num16 << 4) & 0x0FF0;
    num16_upper8 *= times;
    num16_lower8 *= times;
    num16_upper8 /= divide;
    num16_lower8 /= divide;
    num16_result = (num16_upper8 << 4) + (num16_lower8 >> 4);
    printf("3rd approach:\n");
    printf("num16_result = %u. <== Perfect! Retains the bits that right-shift during the divide.\n", num16_result);

    // -----------------------------------------------------
    // Goal: Use only 16-bit values & math to find 65401 * 99/127.
    // Result: ///////////
    // 2ND APPROACH DOESN'T WORK! OVERFLOWS since times > 16! NOTE THAT 16 IS THE HIGHEST *TIMES* VALUE I CAN USE SINCE 
    // 2^16/0b1111,1111,0000 = 65536/4080 = 16.0627.
    num16 = 65401; // 1111 1111 0111 1001 
    times = 99;
    divide = 127;
    printf("\n%u * %u/%u = %u. <== true answer\n", num16, times, divide, (uint32_t)num16*times/divide);
    // 1st approach: place all 8 bits to the far right
    num16_upper8 = num16 >> 8; // 1111 1111
    num16_lower8 = num16 & 0xFF; // 0111 1001
    num16_upper8 *= times;
    num16_lower8 *= times;
    num16_upper8 /= divide;
    num16_lower8 /= divide;
    num16_result = (num16_upper8 << 8) + num16_lower8;
    printf("num16_result = %u. <== Loses bits that right-shift out during the divide, but is close.\n", num16_result);
    // 2nd approach: place all 8 bits in the *center*, with 4 on the left to grow when multiplying and 4 on the right
    // to not lose as many bits when dividing. This will help stop the lose of resolution when we divide.
    num16_upper8 = (num16 >> 4) & 0x0FF0;
    num16_lower8 = (num16 << 4) & 0x0FF0;
    num16_upper8 *= times;
    num16_lower8 *= times;
    num16_upper8 /= divide;
    num16_lower8 /= divide;
    num16_result = (num16_upper8 << 4) + (num16_lower8 >> 4);
    printf("num16_result = %u. <== Completely wrong since it overflows during the multiply!\n", num16_result);

    // num8_1 = 245 & 
    // num8_upper4 = num8 >> 4;
    // num8_lower4 = num8 & 0x0F;
    // printf("245 * 99/127 = %u\n", 245*99/127);
    // printf("num8 = %u, num8_upper4 = %u, num8_lower4 = %u\n", num8, num8_upper4, num8_lower4);
    // num8_upper4 *= 99 >> 4;
    // num8_lower4 *= 99 & 0x0F;
    // printf("num8_upper4 = %u, num8_lower4 = %u\n", num8_upper4, num8_lower4);
    // num8_upper4 /= 127 >> 4;
    // num8_lower4 /= 127 & 0x0F;
    // printf("num8_upper4 = %u, num8_lower4 = %u\n", num8_upper4, num8_lower4);
    // num8 = (num8_upper4 << 4) + num8_lower4;
    // printf("num8 = %u, num8_upper4 = %u, num8_lower4 = %u\n\n", num8, num8_upper4, num8_lower4); // FAILED

    // num8 = 245;
    // num8_upper4 = num8 >> 4;
    // num8_lower4 = num8 & 0x0F;
    // printf("245 * 99/127 = %u\n", 245*99/127);
    // printf("num8 = %u, num8_upper4 = %u, num8_lower4 = %u\n", num8, num8_upper4, num8_lower4);
    // num8_upper4 *= 99 >> 4;
    // num8_lower4 *= 99 & 0x0F;
    // printf("num8_upper4 = %u, num8_lower4 = %u\n", num8_upper4, num8_lower4);
    // num8_upper4 /= 127 >> 4;
    // num8_lower4 /= 127 & 0x0F;
    // printf("num8_upper4 = %u, num8_lower4 = %u\n", num8_upper4, num8_lower4);
    // num8 = (num8_upper4 << 4) + num8_lower4;
    // printf("num8 = %u, num8_upper4 = %u, num8_lower4 = %u\n\n", num8, num8_upper4, num8_lower4); // FAILED

    // // Now let's do something similar with forced 128-bit types:
    // uint64_t num64 = 18000000000000000000UL; // 1.8e19; Note: max uint64_t is 1.8446744e+19.
    // // Let's do num64 x 99999/900000. Ans: 1.8e19 x 99999/900000 = 1.99998e+18. The intermediate step, 
    // // 1.8e19 x 99999 = 1.799982e+24, however, is way too big to store into a uint64_t, so I'll have to 
    // // emulate uint128_t types since my target system doesn't natively support them. 
    // printf("num64 = %lu, num64 as hex = 0x%lX\n", num64, num64);
    // uint64_t num64_upper32 = num64 >> 32;
    // uint64_t num64_lower32 = num64 & 0xFFFFFFFF;
    // printf("num64_upper32 = 0x%lX, num64_lower32 = 0x%lX\n", num64_upper32, num64_lower32);

    // num64_upper32 *= 99999UL >> 32;
    // // printf("%lu\n", 99999UL >> 32);
    // num64_lower32 *= 99999UL & 0xFFFFFFFF;
    // printf("num64_upper32 = 0x%lX, num64_lower32 = 0x%lX\n", num64_upper32, num64_lower32);
    // // printf("num64 = %lu, num64_upper32 = %lu, num64_lower32 = %lu\n", num64, num64_upper32, num64_lower32);
    
    // // num64_upper32 /= 900000UL >> 32; /////////DON'T DIVIDE BY ZERO!/////
    // num64_lower32 /= 900000UL & 0xFFFFFFFF;
    // printf("num64_upper32 = 0x%lX, num64_lower32 = 0x%lX\n", num64_upper32, num64_lower32);
    // // printf("num64 = %lu, num64_upper32 = %lu, num64_lower32 = %lu\n", num64, num64_upper32, num64_lower32);

    // num64 = (num64_upper32 << 32) + num64_lower32;
    // // ERROR: Floating point exception (core dumped) :(
    // // printf("1.8e19 * 99999/900000 = %L\n", (long double)1.8e19*99999.0/900000.0); 
    // printf("num64 = %lu\n", num64);

    // ///////////////////////
    // // Now let's do something similar with forced 128-bit types:
    // num64 = 18000000000000000000UL; // 1.8e19; Note: max uint64_t is 1.8446744e+19.
    // num64 = (__uint128_t)num64*(__uint128_t)99999/(__uint128_t)900000;
    // printf("num64 = %lu\n", num64);

    // // // Let's do num64 x 99999/900000. Ans: 1.8e19 x 99999/900000 = 1.99998e+18. The intermediate step, 
    // // // 1.8e19 x 99999 = 1.799982e+24, however, is way too big to store into a uint64_t, so I'll have to 
    // // // emulate uint128_t types since my target system doesn't natively support them. 
    // // printf("num64 = %lu, num64 as hex = 0x%lX\n", num64, num64);
    // // uint64_t num64_upper32 = num64 >> 32;
    // // uint64_t num64_lower32 = num64 & 0xFFFFFFFF;
    // // printf("num64_upper32 = 0x%lX, num64_lower32 = 0x%lX\n", num64_upper32, num64_lower32);

    // // num64_upper32 *= 99999UL >> 32;
    // // // printf("%lu\n", 99999UL >> 32);
    // // num64_lower32 *= 99999UL & 0xFFFFFFFF;
    // // printf("num64_upper32 = 0x%lX, num64_lower32 = 0x%lX\n", num64_upper32, num64_lower32);
    // // // printf("num64 = %lu, num64_upper32 = %lu, num64_lower32 = %lu\n", num64, num64_upper32, num64_lower32);
    
    // // // num64_upper32 /= 900000UL >> 32; /////////DON'T DIVIDE BY ZERO!/////
    // // num64_lower32 /= 900000UL & 0xFFFFFFFF;
    // // printf("num64_upper32 = 0x%lX, num64_lower32 = 0x%lX\n", num64_upper32, num64_lower32);
    // // // printf("num64 = %lu, num64_upper32 = %lu, num64_lower32 = %lu\n", num64, num64_upper32, num64_lower32);

    // // num64 = (num64_upper32 << 32) + num64_lower32;
    // // // ERROR: Floating point exception (core dumped) :(
    // // // printf("1.8e19 * 99999/900000 = %L\n", (long double)1.8e19*99999.0/900000.0); 
    // // printf("num64 = %lu\n", num64);

    // num8 = 245;
    // num8_upper4 = num8 >> 4;
    // num8_lower4 = num8 & 0x0F;
    // printf("245 * 99/127 = %u\n", 245*99/127);
    // printf("num8 = %u, num8_upper4 = %u, num8_lower4 = %u\n", num8, num8_upper4, num8_lower4);
    // num8_upper4 *= 99 >> 4;
    // num8_lower4 *= 99 & 0x0F;
    // printf("num8_upper4 = %u, num8_lower4 = %u\n", num8_upper4, num8_lower4);
    // num8_upper4 /= 127 >> 4;
    // num8_lower4 /= 127 & 0x0F;
    // printf("num8_upper4 = %u, num8_lower4 = %u\n", num8_upper4, num8_lower4);
    // num8 = (num8_upper4 << 4) + num8_lower4;
    // printf("num8 = %u, num8_upper4 = %u, num8_lower4 = %u\n\n", num8, num8_upper4, num8_lower4); // FAILED


    return 0;
}

// PRIVATE FUNCTION DEFINITIONS:

/// @brief A function to help identify at what decimal digit error is introduced, based on how many bits you are using
///        to represent the fractional portion of the number in your fixed-point number system.
/// @details    Note: this function relies on an internal static bool to keep track of if it has already
///             identified at what decimal digit error is introduced, so once it prints this fact once, it will never 
///             print again. This is by design just to simplify usage in this demo.
/// @param[in]  num_digits_after_decimal    The number of decimal digits we are printing after the decimal 
///             (0, 1, 2, 3, etc)
/// @return     None
static void print_if_error_introduced(uint8_t num_digits_after_decimal)
{
    static bool already_found = false;

    // Array of power base 10 values, where the value = 10^index:
    const uint32_t POW_BASE_10[] = 
    {
        1, // index 0 (10^0)
        10, 
        100, 
        1000, 
        10000, 
        100000,
        1000000,
        10000000,
        100000000,
        1000000000, // index 9 (10^9); 1 Billion: the max power of 10 that can be stored in a uint32_t
    };

    if (already_found == true)
    {
        goto done;
    }

    if (POW_BASE_10[num_digits_after_decimal] > FRACTION_DIVISOR)
    {
        already_found = true;
        printf(" <== Fixed-point math decimal error first\n"
               "    starts to get introduced here since the fixed point resolution (1/%u) now has lower resolution\n"
               "    than the base-10 resolution (which is 1/%u) at this decimal place. Decimal error may not show\n"
               "    up at this decimal location, per say, but definitely will for all decimal places hereafter.", 
               FRACTION_DIVISOR, POW_BASE_10[num_digits_after_decimal]);
    }

done:
    printf("\n");
}
