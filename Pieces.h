//
// Created by Alexandr on 18/03/2025.
//

#ifndef PIECES_H
#define PIECES_H

#include "BitboardClass.h"


//pawn attacks table [side][square]
extern U64 pawn_attacks[2][64];
//knight attacks table [square], because side doesnt matter
extern U64 knight_attacks[64];
//king atrtacks
extern U64 king_attacks[64];

void init_leapers_attacks();
U64 mask_pawn_attacks(int square, int side);
U64 mask_knight_attacks(int square);
U64 mask_king_attacks(int square);
U64 mask_bishop_attacks(int square);
U64 mask_rook_attacks(int square);
// generate bishop attacks on the fly
U64 bishop_attacks_on_fly(int square, U64 block);
U64 rook_attacks_on_fly(int square, U64 block);

#endif //PIECES_H
