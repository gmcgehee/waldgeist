#include <iostream>
#include <cassert>
#include <format>

#include "gamestate.hpp"
#include "bitboard.hpp"
#include "types.hpp"
#include "tables.hpp"
#include "movegen.hpp"
#include "engine.hpp"

#include "debug.hpp"

// temporary
#include <chrono>


int main()
{

    Engine *engine = new Engine();

    GameState *gamestate = &engine->gamestate;

    // gamestate->loadFromFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    gamestate->loadDefaultBoard(); //accurate up to perft 6
    // std::cout << getPrintableBoardState(gamestate->state) << std::endl;

    int perft_depth = 4;
    auto start = std::chrono::high_resolution_clock::now();


    unsigned long long move_count;
    int trial_count = 1;
    for (int i = 0; i < trial_count; i++)
        move_count = engine->perft(perft_depth);
    
    auto end = std::chrono::high_resolution_clock::now();


    std::cout << "Move count at PERFT " << perft_depth << ": " << move_count << std::endl;

    auto duration = duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << "Total Elapsed time: " << duration.count() << "ns\n";
    std::cout << "Average time per trial: " << duration.count() / trial_count << " ns\n";


    return 0;
}