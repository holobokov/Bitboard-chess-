#include <iostream>
#include "bbc.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {

    init_leapers_attacks();

    for (int square = 0; square < 64; square++) {
        //print_bitboard(pawn_attacks[white][square]);
    }

    for (int square = 0; square < 64; square++) {
        //print_bitboard(knight_attacks[square]);
    }

    print_bitboard(mask_bishop_attacks(d4));

    //print_bitboard(not_ab_file);

    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.