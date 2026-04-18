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

// gpt generated
void print_bits(uint16_t v, int bits) {
    for (int i = bits - 1; i >= 0; --i)
        putchar((v >> i) & 1 ? '1' : '0');
}

void print_move(uint16_t m, int idx) {
    printf("Move %d: %u %u %u %u\n",
        idx,
        (m >> 14) & 0x3,   // flag (2 bits)
        (m >> 12) & 0x3,   // promotion (2 bits)
        (m >> 6)  & 0x3F,  // origin (6 bits)
        m & 0x3F           // destination (6 bits)
    );
}

void print_move_bin(uint16_t m, int idx) {
    printf("Move %d: ", idx);

    print_bits((m >> 14) & 0x3, 2);
    putchar(' ');
    print_bits((m >> 12) & 0x3, 2);
    putchar(' ');
    print_bits((m >> 6) & 0x3F, 6);
    putchar(' ');
    print_bits(m & 0x3F, 6);

    putchar('\n');
}

int main()
{

    Engine *engine = new Engine();

    GameState *gamestate = &engine->gamestate;

    gamestate->loadFromFen("r3k2r/1P1p1pP1/8/2PpPp2/3P4/8/PP3PP1/R3K2R w KQkq - 0 1");

    std::cout << getPrintableBoardState(gamestate->state) << std::endl;

    std::cout << "\n"
              << std::endl;

    std::vector<Move> pawn_captures = MoveGeneration::generatePawnCaptures(gamestate->getSideState(BLACK), gamestate->state.pieces[WHITE][PAWN], WHITE);
    std::vector<Move> pawn_pushes = MoveGeneration::generatePawnPushes(~(gamestate->getSideState(BLACK) | gamestate->getSideState(WHITE)), gamestate->state.pieces[WHITE][PAWN], WHITE);

    printf("Pawn Captures: \n");
    for (int i = 0; i < pawn_captures.size(); i++)
    {
        print_move(pawn_captures[i], i);
        print_move_bin(pawn_captures[i], i);
    }

    printf("Pawn Pushes: \n");
    for (int i = 0; i < pawn_pushes.size(); i++)
    {
        print_move(pawn_pushes[i], i);
        print_move_bin(pawn_pushes[i], i);
    }

    /*
    Pawn capture 0: 01 01 110110 111111
    Pawn capture 1: 01 10 110110 111111
    Pawn capture 2: 0111110110111111
    Pawn capture 3: 0100110110111111
    Pawn capture 4: 0101110001111000
    Pawn capture 5: 0110110001111000
    Pawn capture 6: 0111110001111000
    Pawn capture 7: 0100110001111000
    Pawn push 0: 0000001000010000
    Pawn push 1: 0000001001010001
    Pawn push 2: 0000001101010101
    Pawn push 3: 0000001110010110
    Pawn push 4: 0000100010101010
    Pawn push 5: 0000100100101100
    Pawn push 6: 0000110001111001
    Pawn push 7: 0000110110111110
    Pawn push 8: 1111001000011000
    Pawn push 9: 1111001001011001
    Pawn push 10: 1111001101011101
    Pawn push 11: 1111001110011110
    */

    return 0;
}