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

/* TODO:
    - Troubleshoot GameState class not being recognized in movegen params
    - Modify PSTs and MVV-LVA tables/how they're called
    - Look around for other TODOs that have been labeled

*/

int main(int argc, char **argv)
{
    // UCI();
    // return 0;
    // if (argc > 1) {

    // std::string arg1 = argv[1];

    //     if (arg1 == "uci")
    //     {
    //         UCI();
    //         return 0;
    //     }
    // }

    Engine *engine = new Engine();
    GameState *gamestate = &engine->gamestate;

    // gamestate->loadDefaultBoard();
    gamestate->loadFromFen("r1bqkb1r/ppp1nppp/2n5/3pP3/5P2/2N5/PPP1P1PP/R1BQKBNR w KQkq d6 0 5");
    std::cout << "Original State: \n";
    std::cout << getPrintableBoardState(gamestate->state) << '\n';

    // return 0;

    // int perft_depth = 6;
    auto start = std::chrono::steady_clock::now();

    // unsigned long long node_count = engine->perft(perft_depth);
    // 44.7439 s without NMP
    // 30.8287 s with NMP
    // with compiler optimizations:
    // without nmp : 7.02989 s (low 7 to low 9)
    // with    nmp : 5.22629 s (high 4 to high 5)
    std::pair<float, Move> best_pair = engine->alpha_beta(6, -100000, 100000);

    auto end = std::chrono::steady_clock::now();

    auto duration = end - start;
    double seconds = std::chrono::duration<double>(duration).count();
    double nanoseconds = std::chrono::duration<double, std::nano>(duration).count();

    // std::cout << "Move count at PERFT " << perft_depth << ": " << node_count << '\n';
    std::cout << MoveGeneration::moveToString(best_pair.second) << "  " << best_pair.first << '\n';
    std::cout << "Total elapsed time: " << seconds << " s\n";
    // std::cout << "Average time per node: " << (nanoseconds / node_count) << " ns\n";
    // std::cout << "Nodes per second: " << (node_count / seconds) << " nodes/s\n";

    return 0;
}