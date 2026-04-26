#include <iostream>
#include <cassert>
#include <format>

#include "gamestate.hpp"
#include "bitboard.hpp"
#include "types.hpp"
#include "tables.hpp"
#include "movegen.hpp"
#include "engine.hpp"
#include "uci.hpp"

#include "debug.hpp"

// temporary
#include <chrono>

int main(int argc, char** argv)
{

    if (argc > 1) {
        
        std::string arg1 = argv[1];
        
        if (arg1 == "uci") {
            UCI();
            return 0;
        }
    }

    Engine *engine = new Engine();
    GameState *gamestate = &engine->gamestate;

    // gamestate->loadDefaultBoard(); 
    gamestate->loadFromFen("8/p4pR1/1b2b2p/2rp4/5N1k/7P/PP4P1/1B5K b - - 0 1");
    std::cout << "Original State: \n";
    std::cout << getPrintableBoardState(gamestate->state) << '\n';

    Move best_move = engine->alpha_beta(5, __FLT_MAX__, __FLT_MIN__).second;

    std::cout << MoveGeneration::moveToString(best_move) << '\n';

    return 0;



    int perft_depth = 6;
    auto start = std::chrono::steady_clock::now();

    unsigned long long node_count = engine->perft(perft_depth);

    auto end = std::chrono::steady_clock::now();

    auto duration = end - start;
    double seconds = std::chrono::duration<double>(duration).count();
    double nanoseconds = std::chrono::duration<double, std::nano>(duration).count();

    std::cout << "Move count at PERFT " << perft_depth << ": " << node_count << '\n';
    std::cout << "Total elapsed time: " << seconds << " s\n";
    std::cout << "Average time per node: " << (nanoseconds / node_count) << " ns\n";
    std::cout << "Nodes per second: " << (node_count / seconds) << " nodes/s\n";

    return 0;
}