//
// Created by Alexandr on 18/03/2025.
//
#include <iostream>
#include "BitboardClass.h"
#include "Pieces.h"

//pawn attacks table [side][square]
U64 pawn_attacks[2][64];

//generate pawn attacks
U64 mask_pawn_attacks(int side, int square) {
    //result attacks bitboard
    U64 attacks = 0ULL;

    //piece bitboard
    Bitboard bitboard(0ULL);

    //set piece on board
    bitboard.set_bit(square);
    U64 board = bitboard.getBitboard();

    //white pawns
    if (!side) {
        if ((board >> 7) & not_a_file) attacks |= (board >> 7);
        if ((board >> 9) & not_h_file) attacks |= (board >> 9);
    } else {
        //black pawns
        if ((board << 7) & not_h_file) attacks |= (board << 7);
        if ((board << 9) & not_a_file) attacks |= (board << 9);
    }

    return attacks;
}


//knight attack

//knight attacks table [square], because side doesnt matter
U64 knight_attacks[64];

U64 mask_knight_attacks(int square) {
    U64 attacks = 0ULL;

    Bitboard bitboard(0ULL);

    bitboard.set_bit(square);
    U64 board = bitboard.getBitboard();
    if ((board >> 15) & not_a_file) attacks |= (board >> 15);
    if ((board >> 17) & not_h_file) attacks |= (board >> 17);
    if ((board >> 10) & not_hg_file) attacks |= (board >> 10);
    if ((board << 6) & not_hg_file) attacks |= (board << 6);
    if ((board << 15) & not_h_file) attacks |= (board << 15);
    if ((board << 17) & not_a_file) attacks |= (board << 17);
    if ((board << 10) & not_ab_file) attacks |= (board << 10);
    if ((board >> 6) & not_ab_file) attacks |= (board >> 6);

    return attacks;
}

//king attacks
U64 king_attacks[64];

U64 mask_king_attacks(int square) {
    U64 attacks = 0ULL;

    Bitboard bitboard(0ULL);

    bitboard.set_bit(square);
    U64 board = bitboard.getBitboard();

    if ((board >> 7) & not_a_file) attacks |= (board >> 7);
    if (board >> 8) attacks |= (board >> 8);
    if ((board >> 9) & not_h_file) attacks |= (board >> 9);
    if ((board >> 1) & not_h_file) attacks |= (board >> 1);
    if ((board << 7) & not_h_file) attacks |= (board << 7);
    if (board << 8) attacks |= (board << 8);
    if ((board << 9) & not_a_file) attacks |= (board << 9);
    if ((board << 1) & not_a_file) attacks |= (board << 1);

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
