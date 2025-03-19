#include <iostream>
#include "BitboardClass.h"
#include "Pieces.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

//created new branch <dev> for the OOP version od project


int main() {

    init_leapers_attacks();

    // for (int square = 0; square < 64; square++) {
    //     //print_bitboard(pawn_attacks[white][square]);
    // }
    //
    // for (int square = 0; square < 64; square++) {
    //     //print_bitboard(knight_attacks[square]);
    // }

    //init occupancy bitboard
    // U64 block = 0ULL;
    // set_bit(block, d7);
    // set_bit(block, d3);
    // set_bit(block, b4);
    // set_bit(block, h4);
    //print_bitboard(block);

    //print_bitboard(rook_attacks_on_fly(d4, block));

    //std::cout << count_bits(block) << std::endl;
    ///std::cout << index_first_bit(block) << ", " << square_to_coordinate[index_first_bit(block)] << std::endl;

    //print_bitboard(not_ab_file);
    Bitboard attack_mask;
    attack_mask.setBitboard(mask_rook_attacks(e4));
    Bitboard occupancy;
    occupancy.setBitboard(attack_mask.set_occupancy(4095, attack_mask.count_bits()));

    occupancy.print_bitboard();

    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.