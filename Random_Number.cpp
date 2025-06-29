//
// Created by Alexandr on 21/03/2025.
//
#include <iostream>
#include "Bitboard.h"
#include "Pieces.h"
#include "Random_Number.h"

// implementing XORSHIFT32 algorithm to generate pseudo random numbers
unsigned int random_state = 1804289383;

unsigned int get_random_U32_number() {
    //get current state
    unsigned int num = random_state;

    //XOR shift algorithm
    num ^= num << 13;
    num ^= num >> 17;
    num ^= num << 5;

    //update random number state
    random_state = num;
    return num;
}

//generate 64-bit pseudo legal numbers
U64 get_random_U64_number() {
    //define 4 random numbers
    U64 num1, num2, num3, num4;

    num1 = ((U64) get_random_U32_number()) & 0xFFFF;
    num2 = ((U64) get_random_U32_number()) & 0xFFFF;
    num3 = ((U64) get_random_U32_number()) & 0xFFFF;
    num4 = ((U64) get_random_U32_number()) & 0xFFFF;

    return num1 | (num2 << 16) | (num3 << 32) | (num4 << 48);
}

// generate magic number candidate
U64 generate_magic_number() {
    return get_random_U64_number() & get_random_U64_number() & get_random_U64_number();
}

//find appropriate magic number
U64 find_magic_number(int square, int relevant_bits, int bishop) {
    //init occupancy
    U64 occupancies[4096];

    //init attack tables
    U64 attacks[4096];

    //init used attacks
    U64 used_attacks[4096];

    //init attack mask for current piece
    U64 attack_mask = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

    //init occupancy indicies
    int occupancy_indicies = 1 << relevant_bits;

    //loop over occupancy indicies
    for (int i = 0; i < occupancy_indicies; i++) {
        //init occupancies
        occupancies[i] = set_occupancy(i, relevant_bits, attack_mask);

        //init attacks
        attacks[i] = bishop
                         ? bishop_attacks_on_fly(square, occupancies[i])
                         : rook_attacks_on_fly(square, occupancies[i]);
    }

    //test magic numbers
    for (int random_count = 0; random_count < 1000000000000; random_count++) {
        //generate magic number candidate
        U64 magic_number = generate_magic_number();

        //skip inappropriate magic numbers
        if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) { continue; }

        //init used attacks array
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        // init index & fail flag
        int index, fail;

        //test magic index
        for (index = 0, fail = 0; !fail && index < occupancy_indicies; index++) {
            int magic_index = occupancies[index] * magic_number >> (64 - relevant_bits);

            if (used_attacks[magic_index] == 0ULL) {
                //init used attacks
                used_attacks[magic_index] = attacks[index];
            } else if (used_attacks[magic_index] != attacks[index]) {
                fail = 1;
            }
        }

        if (!fail) {
            return magic_number;
        }
    }
    std::cout << "Magic number fails!" << std::endl;
    return 0ULL;
}

//init magic number
void init_magic_numbers() {
    for (int square = 0; square < 64; square++) {
        //init rook magic numbers
        printf(" 0x%llxULL,\n", find_magic_number(square, rook_relevant_bits[square], rook));
        //rook_magic_numbers[square] = find_magic_number(square, rook_relevant_bits[square], rook);
    }

    printf("\n\n");

    for (int square = 0; square < 64; square++) {
        printf(" 0x%llxULL,\n", find_magic_number(square, bishop_relevant_bits[square], bishop));
        //bishop_magic_numbers[square] = find_magic_number(square, bishop_relevant_bits[square], bishop);
    }
}