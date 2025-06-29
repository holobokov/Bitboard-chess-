//
// Created by Alexandr on 18/03/2025.
//

#ifndef BITBOARD_H
#define BITBOARD_H

#define U64 unsigned long long //custom type

//TODO: remake macros
//set/get/pop macros
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0);

//FEN positions
#define empty_board "8/8/8/8/8/8/8/8 b - -"
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "


//preserve board state
#define copy_board()                                                        \
    U64 bitboards_copy[12], occupancies_copy[3];                            \
    int side_copy, enpassant_copy, castle_copy;                             \
    memcpy(bitboards_copy, bitboards, 96); /* sizeof(bitboards) -> 96 */    \
    memcpy(occupancies_copy, occupancies, 24); /* sizeof(occupancies) -> 24 */ \
    side_copy = side, enpassant_copy = enpassant, castle_copy = castle;     \
//restore board state
#define take_back()                                                             \
    memcpy(bitboards, bitboards_copy, 96); /* sizeof(bitboards) -> 96 */        \
    memcpy(occupancies, occupancies_copy, 24); /* sizeof(occupancies) -> 24 */  \
    side = side_copy, enpassant = enpassant_copy, castle = castle_copy;         \



//not A file constant
const U64 not_a_file = 18374403900871474942ULL;
//not H file constant
const U64 not_h_file = 9187201950435737471ULL;
//not HG file constant
const U64 not_hg_file = 4557430888798830399ULL;
//not AB constant
const U64 not_ab_file = 18229723555195321596ULL;

// board square
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sqr
};

constexpr char *square_to_coordinates[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

/* defining bitboards */

enum {
    wk = 1, wq = 2, bk = 4, bq = 8 // binary representation 0001 0010 0100 1000
};

//encode pieces
enum {
    P, N, B, R, Q, K, p, n, b, r, q, k
};

//ASCII pieces
const char ascii_pieces[] = "PNBRQKpnbrqk";

//define unicode pieces
constexpr char *unicode_pieces[] = {
    "♟", "♞", "♝", "♜", "♛", "♚", "♙", "♘", "♗", "♖", "♕", "♔"
};

//convert ASCII character piecs to incoded constants
const int char_pieces[] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,
    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k,
};

//promoted pieces
const char promoted_pieces[] = {
    [Q] = 'q',
    [R] = 'r',
    [B] = 'b',
    [N] = 'n',
    [q] = 'q',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n',
};

//piece bitboards
extern U64 bitboards[12];
// occupancy bitboards
extern U64 occupancies[3]; // white occupancy, black and all
//side to move
extern int side;
//empassant square
extern int enpassant;
//castling rights
extern int castle;


//relevant occupancy bit count for every square on board
const int bishop_relevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6,
};

const int rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12,
};

// sides to move (colors)
enum {
    white, black, both
};

enum { rook, bishop };

void print_bitboard(U64 bitboard);

void print_board();

void print_attacked_squares(int side);

void parse_fen(char *fen);

int count_bits(U64 bitboard);

int get_ls1b_index(U64 bitboard);

int is_square_attacked(int square, int side);

U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask);

void init_all();

#endif //BITBOARD_H
