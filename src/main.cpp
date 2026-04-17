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

    gamestate->loadDefaultBoard();
    gamestate->getPieceAt(c2);
    gamestate->unsetPieceAt(c2);
    std::cout << getPrintableBoardState(gamestate->state) << std::endl;
    gamestate->getPieceAt(c2);



    std::cout << engine->eval() << std::endl;

    return 0;
}