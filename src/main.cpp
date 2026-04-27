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

int main(int argc, char **argv)
{
    UCI();
    return 0;
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

gamestate->loadDefaultBoard();
// gamestate->loadFromFen("8/3k1n2/1ppp4/8/8/8/1PP2R2/1K6 w - - 0 1");
std::cout << "Original State: \n";
std::cout << getPrintableBoardState(gamestate->state) << '\n';

std::pair<float, Move> best_pair = engine->alpha_beta(8, -__FLT_MAX__, __FLT_MAX__);

std::cout << MoveGeneration::moveToString(best_pair.second) << "  " << best_pair.first << '\n';

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