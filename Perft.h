//
// Created by Alexandr on 23/06/2025.
//

#ifndef PERFT_H
#define PERFT_H

// leaf nodes (number od positions reached during the test of the move generator at given depth)
extern long nodes;

int get_time_ms();
void perft_driver(int depth);
void perft_test(int depth);


#endif //PERFT_H
