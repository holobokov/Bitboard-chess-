#include <iostream>
#include "bbc.h"


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

//attacks

//pawn attacks table [side][square]
U64 pawn_attacks[2][64];

//generate pawn attacks
U64 mask_pawn_attacks(int side, int square) {
    //result attacks bitboard
    U64 attacks = 0ULL;

    //piece bitboard
    U64 bitboard = 0ULL;

    //set piece on board
    set_bit(bitboard, square);

    //white pawns
    if (!side) {
        if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
        if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    } else {
        //black pawns
        if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
        if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    }

    //return attack map
    return attacks;
}


//knight attack

//knight attacks table [square], because side doesnt matter
U64 knight_attacks[64];

U64 mask_knight_attacks(int square) {
    U64 attacks = 0ULL;

    U64 bitboard = 0ULL;

    set_bit(bitboard, square);
    if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if ((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
    if ((bitboard << 6) & not_hg_file) attacks |= (bitboard << 6);
    if ((bitboard << 15) & not_h_file) attacks |= (bitboard << 15);
    if ((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
    if ((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
    if ((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);

    return attacks;
}

//king attacks
U64 king_attacks[64];

U64 mask_king_attacks(int square) {
    U64 attacks = 0ULL;

    U64 bitboard = 0ULL;

    set_bit(bitboard, square);

    if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if (bitboard >> 8) attacks |= (bitboard >> 8);
    if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    if ((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);
    if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if (bitboard << 8) attacks |= (bitboard << 8);
    if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    if ((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);

    return attacks;
}

//mask bishop attacks
U64 mask_bishop_attacks(int square) {
    U64 attacks = 0ULL;

    //init ranks and files
    int r, f;

    //init target ranks and files
    int tr = square / 8;
    int tf = square % 8;

    //mask relevant bishop occupancy bits
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (r * 8 + f));

    return attacks;
}

//mask rook attacks
U64 mask_rook_attacks(int square) {
    U64 attacks = 0ULL;

    //init ranks and files
    int r, f;

    //init target ranks and files
    int tr = square / 8;
    int tf = square % 8;

    //mask relevant rook occupancy bits
    for (r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));


    return attacks;
}

// generate bishop attacks on the fly
U64 bishop_attacks_on_fly(int square, U64 block) {
    U64 attacks = 0ULL;

    //init ranks and files
    int r, f;

    //init target ranks and files
    int tr = square / 8;
    int tf = square % 8;

    //generate bishop attacks
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }

    return attacks;
}

U64 rook_attacks_on_fly(int square, U64 block) {
    U64 attacks = 0ULL;

    //init ranks and files
    int r, f;

    //init target ranks and files
    int tr = square / 8;
    int tf = square % 8;

    //generate rook attacks
    for (r = tr + 1; r <= 7; r++) {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block) break;
    }
    for (r = tr - 1; r >= 0; r--) {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block) break;
    }
    for (f = tf + 1; f <= 7; f++) {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block) break;
    }
    for (f = tf - 1; f >= 0; f--) {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block) break;
    }


    return attacks;
}

//init leaper pieces attacks
void init_leapers_attacks() {
    //loop over 64 board squares
    for (int square = 0; square < 64; square++) {
        //init pawn arttacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);

        //init knight attacks
        knight_attacks[square] = mask_knight_attacks(square);

        //init king attacks
        king_attacks[square] = mask_king_attacks(square);
    }
}
