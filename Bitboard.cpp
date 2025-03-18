//
// Created by Alexandr on 18/03/2025.
//
#include <iostream>
#include "Bitboard.h"
#include "Pieces.h"

//print bitboard function
void print_bitboard(U64 bitboard) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            //converte i & j into square index
            int square = i * 8 + j;

            if (!j) std::cout << 8 - i << "    ";

            std::cout << (get_bit(bitboard, square) ? 1 : 0) << "  ";
        }

        std::cout << std::endl;
    }

    //print board files
    std::cout << "\n     a  b  c  d  e  f  g  h" << std::endl;

    //print bitboard as unsigned decimal number
    std::cout << "Bitboard: " << bitboard << std::endl;
}

//count bits function
int count_bits(U64 bitboard) {
    int count = 0;

    while (bitboard) {
        count++;
        bitboard &= bitboard - 1;
    }

    return count;
}

// getting index of least significant 1st bit
int index_first_bit(U64 bitboard) {
    if (bitboard) {
        return count_bits((bitboard & -bitboard) - 1);
    } else {
        return -1;
    }
}

//set occupancy
U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask) {
    //occupancy map
    U64 occupancy = 0ULL;

    for (int i = 0; i < bits_in_mask; i++) {
        //get LS1B index of attack mask
        int square = index_first_bit(attack_mask);
        pop_bit(attack_mask, square);

        if (index & (1 << i)) {
            //update occupancy map
            occupancy |= (1ULL << square);
        }
    }

    return occupancy;
}