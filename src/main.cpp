#include <iostream>
#include <cassert>


#include "bitboard.hpp"
#include "utilities.hpp"
#include "types.hpp"
#include "gamestate.hpp"
#include "tables.hpp"
#include "movegen.hpp"

int main()
{

    //GameState *game = new GameState();

    print_bb(KNIGHT_MOVES[b3]);


    Move move = MoveGeneration::convertToMove((Square)c8, (Square)c7, TO_NONE, NONSPECIAL);

    std::cout << move << std::endl;
    // Should be: 0100110010111010
    // Actually : 0100110010111010

    return 0;
    
}