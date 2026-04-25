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

    gamestate->loadFromFen("4k3/8/8/8/8/8/p7/6K1 b - - 0 1"); // failing on pawn promotions where pawncount > 1 on a side
    // gamestate->loadDefaultBoard(); // accurate up to perft 6
    
    // print_bb(ROOK_PEXT_TABLES[a5][37]);
    
    std::cout << "Original State: \n";
    std::cout << getPrintableBoardState(gamestate->state) << '\n';
    // std::cout << "\nMailbox Original Rep: \n";
    // std::cout << getPrintableBoardState(gamestate->mailbox) << '\n';

    // std::cout << "Bitboards Qcastling Rep: \n";
    // std::cout << getPrintableBoardState(gamestate->state) << '\n';
    // std::cout << "\nMailbox QCastling Rep: \n";
    // std::cout << getPrintableBoardState(gamestate->mailbox) << '\n';


    int perft_depth = 2;
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