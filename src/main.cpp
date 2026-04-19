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
    gamestate->loadDefaultBoard(); // accurate up to perft 6
    std::cout << getPrintableBoardState(gamestate->state) << '\n';

    // Move castling_move = 0b1100111100111010;

    // Undo undo;
    // gamestate->make(castling_move, undo);
    // gamestate->unmake(castling_move, undo);

    // std::cout << '\n' << getPrintableBoardState(gamestate->state) << '\n';
    // return 0;

    int perft_depth = 7;
    auto start = std::chrono::steady_clock::now();

    unsigned long long node_count = 0;
    int trial_count = 1;
    for (int i = 0; i < trial_count; i++)
        node_count = engine->perft(perft_depth);

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