//
// Created by Alexandr on 18/03/2025.
//
#include <iostream>;
#include "Bitboard.h"
#include "Pieces.h"
#include "Random_Number.h"

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


U64 bishop_masks[64];
U64 bishop_attacks[64][512];
U64 rook_masks[64];
U64 rook_attacks[64][4096];

//init slider pieces attack tables
void init_slider_attacks(int bishop) {
    for (int square = 0; square < 64; square++) {
        //init bishop & rook masks
        bishop_masks[square] = mask_bishop_attacks(square);
        rook_masks[square] = mask_rook_attacks(square);

        //init current mask
        U64 attack_mask = bishop ? bishop_masks[square] : rook_masks[square];

        //init relevant occupancy bit count
        int relevant_bits_count = count_bits(attack_mask);

        //init occupancy indicies
        int occupancy_indicies = (1 << relevant_bits_count);

        for (int i = 0; i < occupancy_indicies; i++) {
            //bishop
            if (bishop) {
                U64 occupancy = set_occupancy(i, relevant_bits_count, attack_mask);

                int magic_index = occupancy * bishop_magic_numbers[square] >> (64 - bishop_relevant_bits[square]);

                bishop_attacks[square][magic_index] = bishop_attacks_on_fly(square, occupancy);
            } //rook
            else {
                U64 occupancy = set_occupancy(i, relevant_bits_count, attack_mask);

                int magic_index = occupancy * rook_magic_numbers[square] >> (64 - rook_relevant_bits[square]);

                rook_attacks[square][magic_index] = rook_attacks_on_fly(square, occupancy);
            }
        }
    }
}

//get bishop attacks
U64 get_bishop_attacks(int square, U64 occupancy) {
    //get bishop attacks assuming current board occuppancy
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64 - bishop_relevant_bits[square];

    return bishop_attacks[square][occupancy];
}

//get rook attacks
U64 get_rook_attacks(int square, U64 occupancy) {
    //get rook attacks assuming current board occuppancy
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_relevant_bits[square];

    return rook_attacks[square][occupancy];
}

U64 get_queen_attacks(int square, U64 occupancy) {
    U64 queen_attacks = 0ULL;

    U64 bishop_occupancies = occupancy;
    U64 rook_occupancies = occupancy;

    bishop_occupancies &= bishop_masks[square];
    bishop_occupancies *= bishop_magic_numbers[square];
    bishop_occupancies >>= 64 - bishop_relevant_bits[square];

    queen_attacks = bishop_attacks[square][bishop_occupancies];

    rook_occupancies &= rook_masks[square];
    rook_occupancies *= rook_magic_numbers[square];
    rook_occupancies >>= 64 - rook_relevant_bits[square];

    queen_attacks |= rook_attacks[square][rook_occupancies];

    return queen_attacks;
}


//get rook attacks

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
