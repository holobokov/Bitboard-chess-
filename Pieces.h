//
// Created by Alexandr on 18/03/2025.
//

#ifndef PIECES_H
#define PIECES_H

//TODO: redefine some pieces

//pawn attacks table [side][square]
extern U64 pawn_attacks[2][64];
//knight attacks table [square], because side doesnt matter
extern U64 knight_attacks[64];
//king atrtacks
extern U64 king_attacks[64];
//bishop attacks mask
extern U64 bishop_masks[64];
//bishop attacks table
extern U64 bishop_attacks[64][512];
//rook attacks mask
extern U64 rook_masks[64];
//rook attacks table
extern U64 rook_attacks[64][4096];

void init_leapers_attacks();

void init_slider_attacks(int bishop);

void generate_moves();

U64 mask_pawn_attacks(int square, int side);

U64 mask_knight_attacks(int square);

U64 mask_king_attacks(int square);

U64 mask_bishop_attacks(int square);

U64 mask_rook_attacks(int square);

// generate bishop attacks on the fly
U64 bishop_attacks_on_fly(int square, U64 block);

U64 rook_attacks_on_fly(int square, U64 block);

U64 get_bishop_attacks(int square, U64 occupancy);

U64 get_rook_attacks(int square, U64 occupancy);

U64 get_queen_attacks(int square, U64 occupancy);

#endif //PIECES_H
