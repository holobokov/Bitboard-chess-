//
// Created by Alexandr on 18/03/2025.
//
#include <iostream>
#include "Bitboard.h"
#include "Pieces.h"
#include "Random_Number.h"

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

U64 bitboards[12];
// occupancy bitboards
U64 occupancies[3]; // white occupancy, black and all
//side to move
int side = black; //where equal to white!!
//empassant square
int enpassant = e3;
//castling rights
int castle =+ wk + wq + bk + bq;



//print board
void print_board() {
    //loop over ranks
    for (int rank = 0; rank < 8; rank++) {
        //over files
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;

            if (!file)
                std::cout << 8 - rank << "  ";

            //define piece variable
            int piece = -1;

            //loop over all piece bitboards
            for (int bb_piece = P; bb_piece <= k; bb_piece++) {
                if (get_bit(bitboards[bb_piece], square)) {
                    piece = bb_piece;
                }
            }

#ifdef WIN64
            printf("  %c", (piece == -1) ? '.' : ascii_pieces[piece]);
#else
            if (piece == -1)
                printf("  %c", '.');
            else
                printf("  %s", unicode_pieces[piece]);
#endif
        }
        std::cout << std::endl;
    }

    std::cout << "\n     a  b  c  d  e  f  g  h" << std::endl;

    //print side to move
    if (!side) std::cout << "\n     Side:      white";
    else std::cout << "\n     Side:      black";

    //print enpassant square
    if (enpassant != no_sqr) std::cout << "\n     Enpassant:    " << square_to_coordinates[enpassant];
    else std::cout << "\n     Enpassant: no";

    //print castlings rights
    std::cout << "\n     Castling:   ";
    if (castle & wk) std::cout << 'K';
    else std::cout << '-';
    if (castle & wq) std::cout << 'Q';
    else std::cout << '-';
    if (castle & bk) std::cout << 'k';
    else std::cout << '-';
    if (castle & bq) std::cout << 'q';
    else std::cout << '-';

    std::cout << std::endl;
}

//parse FEN string
void parse_fen(char *fen) {
    //reset board positions state variables
    memset(bitboards, 0ULL, sizeof(bitboards));
    memset(occupancies, 0ULL, sizeof(occupancies));
    side = 0;
    enpassant = no_sqr;
    castle = 0;

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;

            //match ASCII charachters
            if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z')) {
                //init piece typy
                int piece = char_pieces[*fen];

                //set piece on corresponding bitboard
                set_bit(bitboards[piece], square);

                *fen++;
            }

            //match empty square numbers within FEN string
            if (*fen >= '0' && *fen <= '9') {
                //init offset
                int offset = *fen - '0';


                //define piece variable
                int piece = -1;

                //loop over all piece bitboards
                for (int bb_piece = P; bb_piece <= k; bb_piece++) {
                    if (get_bit(bitboards[bb_piece], square)) {
                        piece = bb_piece;
                    }
                }

                //if no piece on current square
                if (piece == -1) {
                    file--;
                }

                file += offset;
                *fen++;
            }

            //match rank separator
            if (*fen == '/') {
                *fen++;
            }
        }
    }

    //go to parsing side to move (incrementing)
    *fen++;
    (*fen == 'w') ? side = white : side = black;

    //parsing castling rights
    fen += 2;
    while (*fen != ' ') {
        switch (*fen) {
            case 'K': castle |= wk; break;
            case 'Q': castle |= wq; break;
            case 'k': castle |= bk; break;
            case 'q': castle |= bq; break;
            case '-': break;
        }

        *fen++;
    }

    //parsing enpassant square
    *fen++;

    if (*fen != '-') {
        //parse enpassant file and rank
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');

        //init enpassant squre
        enpassant = rank * 8 + file;
    } else {
        enpassant = no_sqr;
    }

    //init white occupancies
    for (int piece = P; piece <= K; piece++) {
        //populate white occupancy bitboard
        occupancies[white] |= bitboards[piece];
    }

    //init black occupancies
    for (int piece = p; piece <= k; piece++) {
        //populate white occupancy bitboard
        occupancies[black] |= bitboards[piece];
    }

    //init all ocupancies
    occupancies[both] |= occupancies[white] | occupancies[black];

    printf("fen: %s\n", fen);
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
int get_ls1b_index(U64 bitboard) {
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
        int square = get_ls1b_index(attack_mask);
        pop_bit(attack_mask, square);

        if (index & (1 << i)) {
            //update occupancy map
            occupancy |= (1ULL << square);
        }
    }

    return occupancy;
}

// is current given square is attacked
int is_square_attacked(int square, int side) {
    //in case of pawns
    if ((side == white) && (pawn_attacks[black][square] & bitboards[P])) return 1;
    if ((side == black) && (pawn_attacks[white][square] & bitboards[p])) return 1;

    //in case of knights
    if (knight_attacks[square] & ((side == white) ? bitboards[N] : bitboards[n])) return 1;

    //in case of king
    if (king_attacks[square] & ((side == white) ? bitboards[K] : bitboards[k])) return 1;

    //in case of bishop
    if (get_bishop_attacks(square, occupancies[both]) & ((side == white) ? bitboards[B] : bitboards[b])) return 1;

    //in case of rook
    if (get_rook_attacks(square, occupancies[both]) & ((side == white) ? bitboards[R] : bitboards[r])) return 1;

    //in case of queen
    if (get_queen_attacks(square, occupancies[both]) & ((side == white) ? bitboards[Q] : bitboards[q])) return 1;

    return 0;
}

//print attacked squares
void print_attacked_squares(int side) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;

            if (!file) printf("%d  ", 8-rank);

            is_square_attacked(square, side);

            printf("  %d", is_square_attacked(square, side) ? 1 : 0);
        }

        std::cout << std::endl;
    }

    std::cout << "\n     a  b  c  d  e  f  g  h \n\n" << std::endl;
}

//initialize all variables
void init_all() {
    init_leapers_attacks();
    //init_magic_numbers();
    init_slider_attacks(bishop);
    init_slider_attacks(rook);
}
