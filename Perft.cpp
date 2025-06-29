//
// Created by Alexandr on 23/06/2025.
//
#include <iostream>
#include "Bitboard.h"
#include "Move.h"
#include "Perft.h"
#include <string.h>

#ifdef WIN64
#include <windows.h>
#else
#include <sys/time.h>
#endif


//get time in ms
int get_time_ms() {
#ifdef WIN64
    return GetTickCount();
#else
    struct timeval time_value;
    gettimeofday(&time_value, NULL);
    return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
#endif
}

// leaf nodes (number od positions reached during the test of the move generator at given depth)
long nodes;

//perft driver (performance driver)
void perft_driver(int depth) {
    //reccursion escape condition
    if (depth == 0) {
        //increment nodes count
        nodes++;
        return;
    }

    //create move list instance
    moves move_list[1];
    //generate moves
    generate_moves(move_list);

    //loop over generated moves
    for (int move_count = 0; move_count < move_list->count; move_count++) {
        //preserve board state
        copy_board();

        //make move
        if (!make_move(move_list->moves[move_count], all_moves)) {
            //skip to the next move
            continue;
        }

        //call perft driver recursively
        perft_driver(depth - 1);

        //take back
        take_back();
    }
}

//perft test
void perft_test(int depth) {
    printf("\n   Performance test\n\n");


    //create move list instance
    moves move_list[1];
    //generate moves
    generate_moves(move_list);

    long start = get_time_ms();

    //loop over generated moves
    for (int move_count = 0; move_count < move_list->count; move_count++) {
        //preserve board state
        copy_board();

        //make move
        if (!make_move(move_list->moves[move_count], all_moves)) {
            //skip to the next move
            continue;
        }

        //cummulative nodes
        long cumulative_nodes = nodes;


        //call perft driver recursively
        perft_driver(depth - 1);

        //old nodes
        long old_nodes = nodes - cumulative_nodes;

        //take back
        take_back();

        // print move
        printf("     move: %s%s%c  nodes: %ld\n", square_to_coordinates[get_move_source(move_list->moves[move_count])],
                                                 square_to_coordinates[get_move_target(move_list->moves[move_count])],
                                                 get_move_promoted(move_list->moves[move_count]) ? promoted_pieces[get_move_promoted(move_list->moves[move_count])] : ' ',
                                                 old_nodes);
    }

    printf("\n   Depth: %d", depth);
    printf("\n   Nodes: %ld", nodes);
    printf("\n   Time: %ld", get_time_ms() - start);
}