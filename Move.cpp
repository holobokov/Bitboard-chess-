//
// Created by Alexandr on 21/06/2025.
//
#include <iostream>;
#include "Bitboard.h"
#include "Pieces.h"
#include "Random_Number.h"
#include "Move.h"
#include "Perft.h"

void generate_moves(moves *move_list) {
    //init move count
    move_list->count = 0;

    //define source & target squares
    int source_square, target_square;

    //define current pieces bitboard copy
    U64 bitboard, attacks;

    //loop over all the bitboards
    for (int piece = P; piece <= k; piece++) {
        //init piece bitboard copy
        bitboard = bitboards[piece];

        //generate white pawns a white king castling moves
        if (side == white) {
            //pick up white pawn bitboards index
            if (piece == P) {
                while (bitboard) {
                    //init source square
                    source_square = get_ls1b_index(bitboard);
                    //printf("white pawn: %s\n", square_to_coordinates[source_square]);

                    //init target square
                    target_square = source_square - 8;

                    //generate quite pawn moves
                    if (!(target_square < a8) && !get_bit(occupancies[both], target_square)) {
                        //pawn promotion
                        if (source_square >= a7 && source_square <= h7) {
                            //add move into a move list

                            add_move(move_list, encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));
                        } else {
                            // one square ahead pawn move
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                            //two squares ahead pawn move
                            if ((source_square >= a2 && source_square <= h2) && !get_bit(
                                    occupancies[both], target_square - 8)) {
                                add_move(move_list, encode_move(source_square, target_square - 8, piece, 0, 0, 1, 0,
                                                                0));
                            }
                        }
                    }

                    //init pawn attacks bitboard
                    attacks = pawn_attacks[side][source_square] & occupancies[black];
                    //generate pawn capture moves
                    while (attacks) {
                        //init target square
                        target_square = get_ls1b_index(attacks);

                        //pawn promotion
                        if (source_square >= a7 && source_square <= h7) {
                            add_move(move_list, encode_move(source_square, target_square, piece, Q, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, R, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, B, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, N, 1, 0, 0, 0));
                        } else {
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                        }

                        //pop ls1b
                        pop_bit(attacks, target_square);
                    }

                    //generate enpassant captures
                    if (enpassant != no_sqr) {
                        //lookup pawn attacks and bitwise AND with enpassant square (bit)
                        U64 enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);

                        //make sure enpassant capture available
                        if (enpassant_attacks) {
                            //init enpassant capture target square
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    //pop ls1b from bitboard copy
                    pop_bit(bitboard, source_square);
                }
            }

            //generate castling moves
            if (piece == K) {
                //king side castling is available
                if (castle & wk) {
                    //make sure squares between king and king's rook are empty
                    if (!get_bit(occupancies[both], f1) && !get_bit(occupancies[both], g1)) {
                        //make sure king and the f1 square are not under attacks
                        if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black)) {
                            add_move(move_list, encode_move(e1, g1, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }

                //queen side castling is available
                if (castle & wq) {
                    //make sure squares between king and queens's rook are empty
                    if (!get_bit(occupancies[both], d1) && !get_bit(occupancies[both], c1) && !get_bit(
                            occupancies[both], b1)) {
                        //make sure king and the d1 square are not under attacks
                        if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black)) {
                            add_move(move_list, encode_move(e1, c1, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
            }
        }


        //generate black pawns a black king castling moves
        else {
            if (piece == p) {
                while (bitboard) {
                    //init source square
                    source_square = get_ls1b_index(bitboard);
                    //printf("white pawn: %s\n", square_to_coordinates[source_square]);

                    //init target square
                    target_square = source_square + 8;

                    //generate quite pawn moves
                    if (!(target_square > h1) && !get_bit(occupancies[both], target_square)) {
                        //pawn promotion
                        if (source_square >= a2 && source_square <= h2) {
                            //add move into a move list
                            add_move(move_list, encode_move(source_square, target_square, piece, q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, r, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, b, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, n, 0, 0, 0, 0));
                        } else {
                            // one square ahead pawn move
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                            //two squares ahead pawn move
                            if ((source_square >= a7 && source_square <= h7) && !get_bit(
                                    occupancies[both], target_square + 8)) {
                                add_move(move_list, encode_move(source_square, target_square + 8, piece, 0, 0, 1, 0,
                                                                0));
                            }
                        }
                    }

                    //init pawn attacks bitboard
                    attacks = pawn_attacks[side][source_square] & occupancies[white];
                    //generate pawn capture moves
                    while (attacks) {
                        //init target square
                        target_square = get_ls1b_index(attacks);

                        //pawn promotion
                        if (source_square >= a2 && source_square <= h2) {
                            add_move(move_list, encode_move(source_square, target_square, piece, q, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, r, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, b, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, n, 1, 0, 0, 0));
                        } else {
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                        }

                        //pop ls1b
                        pop_bit(attacks, target_square);
                    }

                    //generate enpassant captures
                    if (enpassant != no_sqr) {
                        //lookup pawn attacks and bitwise AND with enpassant square (bit)
                        U64 enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);

                        //make sure enpassant capture available
                        if (enpassant_attacks) {
                            //init enpassant capture target square
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    //pop ls1b from bitboard copy
                    pop_bit(bitboard, source_square);
                }
            }

            //generate castling moves
            if (piece == k) {
                //king side castling is available
                if (castle & bk) {
                    //make sure squares between king and king's rook are empty
                    if (!get_bit(occupancies[both], f8) && !get_bit(occupancies[both], g8)) {
                        //make sure king and the f8 square are not under attacks
                        if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white)) {
                            add_move(move_list, encode_move(e8, g8, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }

                //queen side castling is available
                if (castle & bq) {
                    //make sure squares between king and queen's rook are empty
                    if (!get_bit(occupancies[both], d8) && !get_bit(occupancies[both], c8) && !get_bit(
                            occupancies[both], b8)) {
                        //make sure king and the d8 square are not under attacks
                        if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white)) {
                            add_move(move_list, encode_move(e8, c8, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
            }
        }

        //generate knight moves
        if ((side == white) ? piece == N : piece == n) {
            //loop over source squares of piece bitboard copy
            while (bitboard) {
                //ijnit source square
                source_square = get_ls1b_index(bitboard);

                //init piece attacks in order to get set of target squares
                attacks = knight_attacks[source_square] & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

                //loop over target squares available from generates attacks
                while (attacks) {
                    //init target square
                    target_square = get_ls1b_index(attacks);

                    //quite move
                    if (!get_bit(((side==white) ? occupancies[black] : occupancies[white]), target_square)) {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }

                    //capture move
                    else {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }

                    //pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }


                //pop ls1b of current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }

        //generate bishop moves
        if ((side == white) ? piece == B : piece == b) {
            //loop over source squares of piece bitboard copy
            while (bitboard) {
                //ijnit source square
                source_square = get_ls1b_index(bitboard);

                //init piece attacks in order to get set of target squares
                attacks = get_bishop_attacks(source_square, occupancies[both]) & ((side == white)
                              ? ~occupancies[white]
                              : ~occupancies[black]);

                //loop over target squares available from generates attacks
                while (attacks) {
                    //init target square
                    target_square = get_ls1b_index(attacks);

                    //quite move
                    if (!get_bit(((side==white) ? occupancies[black] : occupancies[white]), target_square)) {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }

                    //capture move
                    else {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }

                    //pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }


                //pop ls1b of current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }

        //generate rook moves
        if ((side == white) ? piece == R : piece == r) {
            //loop over source squares of piece bitboard copy
            while (bitboard) {
                //ijnit source square
                source_square = get_ls1b_index(bitboard);

                //init piece attacks in order to get set of target squares
                attacks = get_rook_attacks(source_square, occupancies[both]) & ((side == white)
                              ? ~occupancies[white]
                              : ~occupancies[black]);

                //loop over target squares available from generates attacks
                while (attacks) {
                    //init target square
                    target_square = get_ls1b_index(attacks);

                    //quite move
                    if (!get_bit(((side==white) ? occupancies[black] : occupancies[white]), target_square)) {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }

                    //capture move
                    else {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }

                    //pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }


                //pop ls1b of current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }

        //generate queen moves
        if ((side == white) ? piece == Q : piece == q) {
            //loop over source squares of piece bitboard copy
            while (bitboard) {
                //ijnit source square
                source_square = get_ls1b_index(bitboard);

                //init piece attacks in order to get set of target squares
                attacks = get_queen_attacks(source_square, occupancies[both]) & ((side == white)
                              ? ~occupancies[white]
                              : ~occupancies[black]);

                //loop over target squares available from generates attacks
                while (attacks) {
                    //init target square
                    target_square = get_ls1b_index(attacks);

                    //quite move
                    if (!get_bit(((side==white) ? occupancies[black] : occupancies[white]), target_square)) {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }

                    //capture move
                    else {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }

                    //pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }


                //pop ls1b of current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }

        //generate king moves
        if ((side == white) ? piece == K : piece == k) {
            //loop over source squares of piece bitboard copy
            while (bitboard) {
                //ijnit source square
                source_square = get_ls1b_index(bitboard);

                //init piece attacks in order to get set of target squares
                attacks = king_attacks[source_square] & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

                //loop over target squares available from generates attacks
                while (attacks) {
                    //init target square
                    target_square = get_ls1b_index(attacks);

                    //quite move
                    if (!get_bit(((side==white) ? occupancies[black] : occupancies[white]), target_square)) {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }

                    //capture move
                    else {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }

                    //pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }


                //pop ls1b of current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }
    }
}


//print move (for UCI purposes)
void print_move(int move) {
    promoted_pieces[get_move_promoted(move)]
        ? (printf("%s%s%c", square_to_coordinates[get_move_source(move)], square_to_coordinates[get_move_target(move)],
                  promoted_pieces[get_move_promoted(move)]))
        : printf("%s%s", square_to_coordinates[get_move_source(move)], square_to_coordinates[get_move_target(move)]);
}

void add_move(moves *move_list, int move) {
    //store move
    move_list->moves[move_list->count] = move;

    //increment move count
    move_list->count++;
}

//print move list
void print_move_list(moves *move_list) {
    //do nothin on empty move list
    if (!move_list->count) {
        printf("\n          No move in move list\n");
        return;
    }

    printf("\n     move     piece     capture     double     enpassant     castling\n\n");

    //loop over moves within a move list
    for (int move_count = 0; move_count < move_list->count; move_count++) {
        //init move
        int move = move_list->moves[move_count];

#ifdef WIN64
        //print move
        //print move
        printf("     %s%s%c    %c         %d           %d          %d             %d\n",
               square_to_coordinates[get_move_source(move)],
               square_to_coordinates[get_move_target(move)],
               (get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' '), ascii_pieces[get_move_piece(move)],
               get_move_capture(move) ? 1 : 0, get_move_double(move) ? 1 : 0, get_move_enpassant(move) ? 1 : 0,
               get_move_castling(move) ? 1 : 0);
#else
        //print move
        printf("     %s%s%c    %s         %d           %d          %d             %d\n",
               square_to_coordinates[get_move_source(move)],
               square_to_coordinates[get_move_target(move)],
               (get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' '),
               unicode_pieces[get_move_piece(move)],
               get_move_capture(move) ? 1 : 0, get_move_double(move) ? 1 : 0, get_move_enpassant(move) ? 1 : 0,
               get_move_castling(move) ? 1 : 0);

#endif
    }

    //print total number of moves
    printf("\n\n    Total nnumer of moves: %d\n\n", move_list->count);
}


//make move on chess board
int make_move(int move, int move_flag) {
    //quite moves
    if (move_flag == all_moves) {
        //preserve board state
        copy_board();

        //parse move
        int source_square = get_move_source(move);
        int target_square = get_move_target(move);
        int piece = get_move_piece(move);
        int promoted_piece = get_move_promoted(move);
        int capture = get_move_capture(move);
        int double_push = get_move_double(move);
        int enpass = get_move_enpassant(move);
        int castling = get_move_castling(move);

        //move piece
        pop_bit(bitboards[piece], source_square);
        set_bit(bitboards[piece], target_square);

        //handling capture moves
        if (capture) {
            //pick up bitboard piece index ranges depending on side
            int start_piece, end_piece;

            //white to mive
            if (side == white) {
                start_piece = p;
                end_piece = k;
            }

            //black to move
            else {
                start_piece = P;
                end_piece = K;
            }

            //loop over bitboards opposite to the current side to move
            for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++) {
                //if there is a piece on the target square
                if (get_bit(bitboards[bb_piece], target_square)) {
                    //remove it from corresponding bitboard
                    pop_bit(bitboards[bb_piece], target_square);
                    break;
                }
            }
        }

        //handle opawn promotions
        if (promoted_piece) {
            //erase the pawn from the target square
            pop_bit(bitboards[(side == white) ? P : p], target_square);

            //set uo promoted piece on chess board
            set_bit(bitboards[promoted_piece], target_square);
        }

        //handle enpassant captures
        if (enpass) {
            //erase the pawn dependening on side tomove
            if (side == white) {
                pop_bit(bitboards[p], target_square + 8);
            } else
                pop_bit(bitboards[P], target_square - 8);
        }
        //reset enpassant square
        enpassant = no_sqr;

        //handle double pawn push
        if (double_push) {
            //set enpassant square depending on side to move
            if (side == white) {
                enpassant = target_square + 8;
            } else {
                enpassant = target_square - 8;
            }
        }

        //handle castling
        if (castling) {
            switch (target_square) {
                //white castle king side
                case (g1):
                    //move H rook
                    pop_bit(bitboards[R], h1);
                    set_bit(bitboards[R], f1);
                    break;

                //white castle queen side
                case (c1):
                    //move A rook
                    pop_bit(bitboards[R], a1);
                    set_bit(bitboards[R], d1);
                    break;
                //black castle king side
                case (g8):
                    //move H rook
                    pop_bit(bitboards[r], h8);
                    set_bit(bitboards[r], f8);
                    break;
                //black castle queen side
                case (c8):
                    //move A rook
                    pop_bit(bitboards[r], a8);
                    set_bit(bitboards[r], d8);
                    break;
            }
        }

        //update castling rights
        castle &= castling_rights[source_square];
        castle &= castling_rights[target_square];

        //reset occupancies
        memset(occupancies, 0ULL, 24);
        //loop over white pieces botboards
        for (int bb_piece = P; bb_piece <= K; bb_piece++) {
            //update white occupancies
            occupancies[white] |= bitboards[bb_piece];
        }
        //loop over black pieces botboards
        for (int bb_piece = p; bb_piece <= k; bb_piece++) {
            //update white occupancies
            occupancies[black] |= bitboards[bb_piece];
        }
        //update both sides occupancies
        occupancies[both] |= occupancies[white];
        occupancies[both] |= occupancies[black];

        //change the side
        side ^= 1;

        //make sure king has not been exposed into a check
        if (is_square_attacked((side == white) ? get_ls1b_index(bitboards[k]) : get_ls1b_index(bitboards[K]), side)) {
            //tacke move back
            take_back();

            //return illigal move
            return 0;
        } else return 1;
    }

    //capture moves
    else {
        //make sure move is the capture
        if (get_move_capture(move)) {
            make_move(move, all_moves);
        }

        //otherwise the move is not capture
        else
        //dont make it
            return 0;
    }
}


//UCI
//parse user/GUI move string input ("e7e8q")
int parse_move(char *move_string) {
    //create move list instance
    moves move_list[1];

    //generate moves
    generate_moves(move_list);

    //parse source square
    int source_square = (move_string[0] - 'a') + (8 - (move_string[1] - '0')) * 8;

    //
    int target_square = (move_string[2] - 'a') + (8 - (move_string[3] - '0')) * 8;

    //define promoted piece
    int promoted_piece = 0;

    for (int move_count = 0; move_count < move_list->count; move_count++) {
        //init move
        int move = move_list->moves[move_count];

        //make sure source and target squares are available within the generated move
        if ((source_square == get_move_source(move)) && (target_square == get_move_target(move))) {
            //init promoted piece
            int promoted_piece = get_move_promoted(move);

            //promoted piece is available
            if (promoted_piece) {
                //promotions
                if ((promoted_piece == Q || promoted_piece == q) && move_string[4] == 'q') {
                    return move;
                } else if ((promoted_piece == R || promoted_piece == r) && move_string[4] == 'r') {
                    return move;
                } else if ((promoted_piece == B || promoted_piece == b) && move_string[4] == 'b') {
                    return move;
                } else if ((promoted_piece == N || promoted_piece == n) && move_string[4] == 'n') {
                    return move;
                }

                //continue the loop on possible wrong ptomotions ("e7e8f")
                continue;
            }


            //return legal move
            return move;
        }
    }

    //return illegal move
    return 0;
}

/*
 * Example UCI command to init position on chess board
 *
 *      //init start position
 *      position startpos
 *
 *      //init startpos and make move
 *      position startpos moves e2e4 e7e5
 *
 *      //init position from fen string
 *      position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1
 *
 *      //init position from fen string and make moves on chess board
 *      position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 moves e2a6 e8g8
 */

//parse UCI "position" command
void parse_position(char *command) {
    //shift pointer to the right where next token begins
    command += 9;

    //init pinter to the current character in the command string
    char *current_char = command;

    //parse UCI "startpos" command
    if (strncmp(command, "startpos", 8) == 0) {
        //init chess board with start positions
        parse_fen(start_position);
    }
    //parse UCI "fen command"
    else {
        //make sure "fen" command is available within command string
        current_char = strstr(command, "fen");

        //if no fen command is available within command string
        if (current_char == NULL) {
            parse_fen(start_position);
        }
        //found "fen" substring
        else {
            //shoft pointer to the right where next token begins
            current_char += 4;

            //init chess board with positioin from FEN string
            parse_fen(current_char);
        }
    }


    //parse moves after position
    current_char = strstr(command, "moves");

    //moves available
    if (current_char != NULL) {
        //shift pointer to the right where next token begins
        current_char += 6;

        //loop over moves within a move string
        while (*current_char) {
            //parse next move
            int move = parse_move(current_char);

            // if no more
            if (move == 0)
                break;

            //make move on chess board
            make_move(move, all_moves);

            //move current character pointer to the end  of curremnt move
            while (*current_char && *current_char != ' ') current_char++;

            //go to the next move
            current_char++;
        }
    }
    //print board
    print_board();
}


/*
 *  Example UCI commands to make engine search for the best move
 *
 *  // fixed depth search
 *  go depth 6
 */


//parse UCI "go" command
void parse_go(char *command) {
    //init depth
    int depth = -1;

    //init char pointer to the current depth argument
    char *current_depth = NULL;

    //handle fixed depth search
    if (current_depth = strstr(command, "depth")) {
        depth = atoi(current_depth + 6);
    }

    //different time controls placeholder
    else
        depth = 6;

    //search position
    search_position(depth);
    //printf("depth: %d\n", depth);
}

/*
 *  GUI -> isready
 *  Engine -> readyok
 *  GUI -> ucinewgame
 */

//main UCI loop
void uci_loop() {
    //reset STDIN & STDOUT buffers
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    //define user / GUI input bufffer
    char input[2000];

    //print engine info
    printf("id name BBC\n");
    printf("id name Oleksandr Holobokov\n");
    printf("uciok\n");

    //main loop
    while (1) {
        //reset user / GUI input
        memset(input, 0, sizeof(input));

        //make sure output reaches the GUI
        fflush(stdout);

        //get user / GUI input
        if (!fgets(input, 2000, stdin)) {
            continue;
        }
        //make sure input is available
        if (input[0] == '\n') {
            continue;
        }
        //parse UCI "isready" command
        if (strncmp(input, "isready", 7) == 0) {
            printf("readyok\n");
            continue;
        }
        //parse UCI "position" command
        else if (strncmp(input, "position", 8) == 0) {
            //call parse position function
            parse_position(input);
        }

        //parse UCI "ucinewgame" command
        else if (strncmp(input, "ucinewgame", 10) == 0) {
            //call parse position function
            parse_position("position startpos");
        }

        //parse UCI "go" command
        else if (strncmp(input, "go", 2) == 0) {
            //call parse go function
            parse_go(input);
        }

        //parse UCI "quite" command
        else if (strncmp(input, "quit", 4) == 0) {
            //quite game
            break;
        }

        //parse UCI "uci" command
        if (strncmp(input, "uci", 3) == 0) {
            //print engine info
            printf("id name BBC\n");
            printf("id name Oleksandr Holobokov\n");
            printf("uciok\n");
        }
    }
}


//material score
/*
 *  P = 100 = P
 *  N = 300 = P * 3
 *  B = 350 = P * 3 + P * 0,5
 *  R = 500 = P * 5
 *  Q = 1000 = P * 10
 *  K = 10000 = P * 100
 *
 */

int material_score[12] = {
    100, // white pawn score
    300, //
    350, //
    500, //
    1000, //
    10000, // white king score
    -100, // black pawn score
    -300, //
    -350, //
    -500, //
    -1000, //
    -10000, //black king score
};

// pawn positional score
const int pawn_score[64] =
{
    90, 90, 90, 90, 90, 90, 90, 90,
    30, 30, 30, 40, 40, 30, 30, 30,
    20, 20, 20, 30, 30, 30, 20, 20,
    10, 10, 10, 20, 20, 10, 10, 10,
    5, 5, 10, 20, 20, 5, 5, 5,
    0, 0, 0, 5, 5, 0, 0, 0,
    0, 0, 0, -10, -10, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

// knight positional score
const int knight_score[64] =
{
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 10, 10, 0, 0, -5,
    -5, 5, 20, 20, 20, 20, 5, -5,
    -5, 10, 20, 30, 30, 20, 10, -5,
    -5, 10, 20, 30, 30, 20, 10, -5,
    -5, 5, 20, 10, 10, 20, 5, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, -10, 0, 0, 0, 0, -10, -5
};

// bishop positional score
const int bishop_score[64] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 10, 10, 0, 0, 0,
    0, 0, 10, 20, 20, 10, 0, 0,
    0, 0, 10, 20, 20, 10, 0, 0,
    0, 10, 0, 0, 0, 0, 10, 0,
    0, 30, 0, 0, 0, 0, 30, 0,
    0, 0, -10, 0, 0, -10, 0, 0

};

// rook positional score
const int rook_score[64] =
{
    50, 50, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50,
    0, 0, 10, 20, 20, 10, 0, 0,
    0, 0, 10, 20, 20, 10, 0, 0,
    0, 0, 10, 20, 20, 10, 0, 0,
    0, 0, 10, 20, 20, 10, 0, 0,
    0, 0, 10, 20, 20, 10, 0, 0,
    0, 0, 0, 20, 20, 0, 0, 0

};

// king positional score
const int king_score[64] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 5, 5, 5, 5, 0, 0,
    0, 5, 5, 10, 10, 5, 5, 0,
    0, 5, 10, 20, 20, 10, 5, 0,
    0, 5, 10, 20, 20, 10, 5, 0,
    0, 0, 5, 10, 10, 5, 0, 0,
    0, 5, 5, -5, -5, 0, 5, 0,
    0, 0, 5, 0, -15, 0, 10, 0
};

// mirror positional score tables for opposite side
const int mirror_score[128] =
{
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

//position evaluation
int evaluate() {
    //static evaluation score
    int score = 0;

    //current pieces bitboard copies
    U64 bitboard;

    //init piece & square
    int piece, square;

    //loop over piece bitboards
    for (int bb_piece = P; bb_piece <= k; bb_piece++) {
        //init piece bitboard copy
        bitboard = bitboards[bb_piece];

        //loop over pieces within a bitboard
        while (bitboard) {
            //init piece
            piece = bb_piece;

            //init square
            square = get_ls1b_index(bitboard);

            //score material weights
            score += material_score[piece];

            //score position piece scores
            switch (piece)
            {
                // evaluate white pieces
                case P: score += pawn_score[square]; break;
                case N: score += knight_score[square]; break;
                case B: score += bishop_score[square]; break;
                case R: score += rook_score[square]; break;
                case K: score += king_score[square]; break;

                // evaluate black pieces
                case p: score -= pawn_score[mirror_score[square]]; break;
                case n: score -= knight_score[mirror_score[square]]; break;
                case b: score -= bishop_score[mirror_score[square]]; break;
                case r: score -= rook_score[mirror_score[square]]; break;
                case k: score -= king_score[mirror_score[square]]; break;
            }

            //pop ls1b
            pop_bit(bitboard, square);
        }
    }

    //return final evaluation based on side
    return (side == white) ? score : -score;
}

//half move counter
int ply;

//best move
int best_move;

//Search
//search positoion for the best move
void search_position(int depth) {
    //find best move within a given position
    int score = negamax(-50000, 50000, depth);

    if (best_move) {
        printf("info score cp %d depth %d nodes %ld\n", score, depth, nodes);

        //best move placeholder
        printf("bestmove ");
        print_move(best_move);
        printf("\n");
    }
}

//quiescence search
int quiescence(int alpha, int beta) {

    nodes++;

    //evaluate position
    int evaluation = evaluate();

    //fail-hard beta cutoff
    if (evaluation >= beta) {
        //node (move) fails high
        return beta;
    }

    //found a better move
    if (evaluation > alpha) {
        //Principal variation (PV) node (move)
        alpha = evaluation;
    }

    //create move list instance
    moves move_list[1];

    //generate moves
    generate_moves(move_list);

    //loop oer moves within a movelist
    for (int count = 0; count < move_list->count; count++) {
        //preserve board state
        copy_board();

        //increment ply
        ply++;

        //make sure to make only legal moves
        if (make_move(move_list->moves[count], only_captures) == 0) {
            ply--;

            //contitnue to next move
            continue;
        }

        //score current move
        int score = -quiescence(-beta, -alpha);

        //decrement ply
        ply--;

        //take move back
        take_back();



        //fail-hard beta cutoff
        if (score >= beta) {
            //node (move) fails high
            return beta;
        }

        //found a better move
        if (score > alpha) {
            //Principal variation (PV) node (move)
            alpha = score;
        }
    }

    //node (move) fails low
    return alpha;
}

//negamax alpha beta search
int negamax(int alpha, int beta, int depth) {

    //recursion stop condition
    if (depth == 0) {
        //run quiescence search

        return quiescence(alpha, beta);
    }

    //increment nodes count
    nodes++;

    //is king in check
    int in_check = is_square_attacked((side == white) ? get_ls1b_index(bitboards[K]) : get_ls1b_index(bitboards[k]),
                                      side ^ 1);

    //legal moves counter
    int legal_moves = 0;

    //best move so far
    int best_sofar;

    //old value of alpha
    int old_alpha = alpha;

    //create move list instance
    moves move_list[1];

    //generate moves
    generate_moves(move_list);

    //loop oer moves within a movelist
    for (int count = 0; count < move_list->count; count++) {
        //preserve board state
        copy_board();

        //increment ply
        ply++;

        //make sure to make only legal moves
        if (make_move(move_list->moves[count], all_moves) == 0) {
            ply--;

            //contitnue to next move
            continue;
        }

        //increment legal moves
        legal_moves++;

        //score current move
        int score = -negamax(-beta, -alpha, depth - 1);

        //decrement ply
        ply--;

        //take move back
        take_back();


        //fail-hard beta cutoff
        if (score >= beta) {
            //node (move) fails high
            return beta;
        }

        //found a better move
        if (score > alpha) {
            //Principal variation (PV) node (move)
            alpha = score;

            // if root move
            if (ply == 0) {
                best_sofar = move_list->moves[count];
            }
        }
    }

    // we dont have any legal moves to make in the current position
    if (legal_moves == 0) {
        //king is in check
        if (in_check) {
            //return mating score (assuming closest distance to mating position)
            return -49000 + ply;
        }

        //king is not in check
        else {
            //return stalemate score
            return 0;
        }
    }

    if (old_alpha != alpha) {
        //init best move
        best_move = best_sofar;
    }

    //node (move) fails low
    return alpha;
}
