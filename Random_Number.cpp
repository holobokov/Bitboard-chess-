//
// Created by Alexandr on 21/03/2025.
//
#include <iostream>
#include "BitboardClass.h"
#include "Pieces.h"

// implementing XORSHIFT32 algorithm to generate pseudo random numbers
unsigned int state = 1804289383;

unsigned int get_random_U32_number() {
    //get current state
    unsigned int num = state;

    //XOR shift algorithm
    num ^= num << 13;
    num ^= num >> 17;
    num ^= num << 5;

    //update random number state
    state = num;

    return num;
}

//generate 64-bit pseudo legal numbers
U64 get_random_U64_number() {
    //define 4 random numbers
    U64 num1, num2, num3, num4;

    num1 = (U64) (get_random_U32_number() & 0xFFFF);
    num2 = (U64) (get_random_U32_number() & 0xFFFF);
    num3 = (U64) (get_random_U32_number() & 0xFFFF);
    num4 = (U64) (get_random_U32_number() & 0xFFFF);

    return num1 | (num2 << 16) | (num3 << 32) | (num4 << 48);
}

// generate magic number candidate
U64 generate_magic_number() {
    return get_random_U64_number() & get_random_U64_number() & get_random_U64_number();
}