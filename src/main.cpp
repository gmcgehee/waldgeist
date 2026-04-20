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

    gamestate->loadFromFen("8/2PP4/8/1Kb1N3/8/7k/5p2/8 w - - 0 1"); // failing on black pawn promotions
    // gamestate->loadDefaultBoard(); // accurate up to perft 6
    std::cout << "Bitboards Original Rep: \n";
    std::cout << getPrintableBoardState(gamestate->state) << '\n';
    std::cout << "\nMailbox Original Rep: \n";
    std::cout << getPrintableBoardState(gamestate->mailbox) << '\n';

    // Move castling_move = 0b1100111100111010;

    // Undo undo;
    // gamestate->make(castling_move, undo);
    // gamestate->unmake(castling_move, undo);

    // std::cout << '\n' << getPrintableBoardState(gamestate->state) << '\n';
    // return 0;

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