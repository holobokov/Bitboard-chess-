//
// Created by Alexandr on 21/06/2025.
//

#ifndef MOVE_H
#define MOVE_H

/*
 *                  binary move bits representation                              hexadecimal constants
 *
 * 0000 0000 0000 0000 0011 1111 source square (63 -> 111111)                           0x3f
 * 0000 0000 0000 1111 1100 0000 target square (63 -> 111111)                           0xfc0
 * 0000 0000 1111 0000 0000 0000 piece (P ... k) -> 11 pieces (1011)                    0xf000
 * 0000 1111 0000 0000 0000 0000 promoted piece (P ... k) -> 11 pieces (1011)           0xf0000
 * 0001 0000 0000 0000 0000 0000 capture flag (yes or no)                               0x100000
 * 0010 0000 0000 0000 0000 0000 double push flag (yes or no)                           0x200000
 * 0100 0000 0000 0000 0000 0000 enpassant flag (yes or no)                             0x400000
 * 1000 0000 0000 0000 0000 0000 castling flag (yes or no)                              0x800000
 */

//encode move macro
#define encode_move(source, target, piece, promoted, capture, double, enpassant, castling) \
(source) | (target << 6) | (piece << 12) | (promoted << 16) | (capture << 20) | (double << 21) | (enpassant << 22) | (castling << 23) \
//exctract source square
#define get_move_source(move) (move & 0x3f)
//extract tarfget square
#define get_move_target(move) ((move & 0xfc0) >> 6)
//extract piece
#define get_move_piece(move) ((move & 0xf000) >> 12)
//extract promoted piece
#define get_move_promoted(move) ((move & 0xf0000) >> 16)
//extract capture flag
#define get_move_capture(move) (move & 0x100000)
//extract double pawn push flag
#define get_move_double(move) (move & 0x200000)
//extract enpassant flag
#define get_move_enpassant(move) (move & 0x400000)
//extract castling flag
#define get_move_castling(move) (move & 0x800000)

//move list structure
typedef struct {
    //moves
    int moves[256];

    //move count
    int count;
} moves;

//move types
enum {all_moves, only_captures};

/*
 *                                                          castling   move           in       in
 *                                                          right      update         binary   decimal
 *
 *                       king & rooks didn't move:            1111   &  1111      =   1111     15
 *
 *                               white king moved:            1111   &  1100      =   1100     12
 *                        white king's rook moved:            1111   &  1110      =   1110     14
 *                       white queen's rook moved:            1111   &  1101      =   1101     13
 *
 *                               black king moved:            1111   &  0011      =   0011     3
 *                        white king's rook moved:            1111   &  1011      =   1011     11
 *                       white queen's rook moved:            1111   &  0111      =   0111     7
 *
 */


//castling rights update constants
const int castling_rights[64] = {
    7, 15, 15, 15, 3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};


//half move counter
extern int ply;

//best move
extern int best_move;


void print_move(int move);
void print_move_list(moves *move_list);
void add_move(moves *move_list, int move);
void search_position(int depth);
int parse_move(char *move_string);
void parse_position(char *command);
void parse_go(char *command);
void uci_loop();
int evaluate();
int negamax(int alpha, int beta, int depth);

void generate_moves(moves *move_list);
int make_move(int move, int move_flag);

#endif //MOVE_H
