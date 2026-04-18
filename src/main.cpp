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

    gamestate->loadDefaultBoard();
    std::cout << getPrintableBoardState(gamestate->state) << std::endl;

    unsigned long long move_count = engine->perft(2);

    return 0;
}