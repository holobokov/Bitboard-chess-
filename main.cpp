#include <iostream>
#include "Random_Number.h"
#include "Bitboard.h"
#include "Pieces.h"
#include "Move.h"
#include <string.h>
#include "Perft.h"


int main() {
    init_all();

    //debug mode variable
    int debug = 1;
    if (debug) {
        parse_fen("r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ");
        print_board();
        search_position(1);
    } else {
        //connect to the GUI
        uci_loop();
    }


    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.
