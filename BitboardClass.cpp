//
// Created by Alexandr on 19/03/2025.
//
#include <iostream>
#include "BitboardClass.h"

//
// Created by Alexandr on 18/03/2025.
//
#include <iostream>
#include "BitboardClass.h"

U64 Bitboard::get_bit(int square) {
    return (getBitboard() & (1ULL << square));
}
void Bitboard::set_bit(int square) {
    setBitboard(getBitboard() | (1ULL << square));
}
void Bitboard::pop_bit(int square) {
    //(get_bit(square) ? getBitboard() ^= (1ULL << square) : 0);
    if (get_bit(square)) {
        setBitboard(getBitboard() ^ (1ULL << square));
    }
}


//print bitboard function
void Bitboard::print_bitboard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            //converte i & j into square index
            int square = i * 8 + j;

            if (!j) std::cout << 8 - i << "    ";

            std::cout << (get_bit(square) ? 1 : 0) << "  ";
        }

        std::cout << std::endl;
    }

    //print board files
    std::cout << "\n     a  b  c  d  e  f  g  h" << std::endl;

    //print bitboard as unsigned decimal number
    std::cout << "Bitboard: " << getBitboard() << std::endl;
}

//count bits function
int Bitboard::count_bits() {
    U64 bitboard = getBitboard();
    int count = 0;

    while (bitboard) {
        count++;
        bitboard &= bitboard - 1;
    }

    return count;
}

// getting index of least significant 1st bit
int Bitboard::index_first_bit() {
    U64 bitboard = getBitboard();
    if (bitboard) {
        bitboard |= (bitboard & -bitboard) - 1;
        return count_bits();
    } else {
        return -1;
    }
}

//set occupancy
U64 Bitboard::set_occupancy(int index, int bits_in_mask) {
    //occupancy map
    Bitboard occupancy(0ULL);

    for (int i = 0; i < bits_in_mask; i++) {
        //get LS1B index of attack mask
        int square = index_first_bit();
        pop_bit(square);

        if (index & (1 << i)) {
            //update occupancy map
            occupancy.setBitboard((1ULL << square));
        }
    }

    return occupancy.getBitboard();
}
