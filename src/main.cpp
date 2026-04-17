#include <iostream>
#include <cassert>
#include <format>

#include "gamestate.hpp"
#include "bitboard.hpp"
#include "types.hpp"
#include "tables.hpp"
#include "movegen.hpp"
#include "engine.hpp"

// temporary
#include <chrono>

int main()
{

    Engine *engine = new Engine();

    GameState *gamestate = &engine->gamestate;
    
    gamestate->loadFromFen("rnbqkbnr/ppppp1pp/8/4P3/5pP1/8/PPPP1P1P/RNBQKBNR b KQkq g3 0 3"); 
    std::cout << getPrintableBoardState(gamestate->state) << std::endl;

    // for (int i = 0; i < 6; i++) {
    //     print_bb(gamestate->state.pieces[WHITE][i]);
    // }

    std::cout << "\n\n" << std::endl;

    // for (int i = 0; i < 6; i++) {
    //     print_bb(gamestate->state.pieces[BLACK][i]);
    // }


    gamestate->make(MoveGeneration::convertToMove(g3, f4));
    std::cout << getPrintableBoardState(gamestate->state) << std::endl;

    // std::cout << engine->eval() << std::endl;

    return 0;
}