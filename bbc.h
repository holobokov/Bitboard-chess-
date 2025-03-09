//
// Created by Alexandr on 07/03/2025.
//

#ifndef BBC_H
#define BBC_H

// define a bitboard type
#define U64 unsigned long long //custom type

//set/get/pop macros
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0);

//not A file constant
const U64 not_a_file = 18374403900871474942ULL;
//not H file constant
const U64 not_h_file = 9187201950435737471ULL;
//not HG file constant
const U64 not_hg_file = 4557430888798830399ULL;
//not AB constant
const U64 not_ab_file = 18229723555195321596ULL;

//pawn attacks table [side][square]
extern U64 pawn_attacks[2][64];


// board square
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
    a0, b0, c0, d0, e0, f0, g0, h0
};

// sides to move (colors)
enum {
    white, black
};


void print_bitboard(U64 bitboard);
U64 mask_pawn_attacks(int square, int side);
void init_leapers_attacks();

#endif //BBC_H
