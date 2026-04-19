#pragma once


#include <set>
#include <vector>
#include <bit>

#include "types.hpp"
#include "bitboard.hpp"
#include "gamestate.hpp"
#include "movegen.hpp"
#include <vector>
#include <algorithm>

bool hasDuplicates(std::vector<Move> move_list)
{
    std::sort(move_list.begin(), move_list.end());
    for (size_t i = 1; i < move_list.size(); ++i)
    {
        if (move_list[i] == move_list[i - 1])
            return true;
    }
    return false;
}

// gpt generated
void print_bits(uint16_t v, int bits)
{
    for (int i = bits - 1; i >= 0; --i)
        putchar((v >> i) & 1 ? '1' : '0');
}

void print_move(uint16_t m, int idx)
{
    printf("Move %d: %u %u %u %u\n",
           idx,
           (m >> 14) & 0x3, // flag (2 bits)
           (m >> 12) & 0x3, // promotion (2 bits)
           (m >> 6) & 0x3F, // origin (6 bits)
           m & 0x3F         // destination (6 bits)
    );
}

void print_move_bin(uint16_t m, int idx)
{
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

//  Temporary for debug DEFINITELY SLOWS THINGS DOWN
void check_for_disparities(BoardState old_state, BoardState curr_state, Mailbox old_mailbox, Mailbox curr_mailbox) {
    for (int i = 0; i < KING + 1; i++)
    {
        if (old_state.pieces[0][i] != curr_state.pieces[0][i])
            throw "white pieces didn't restore properly";
    }

    for (int i = 0; i < KING + 1; i++)
    {
        if (old_state.pieces[1][i] != curr_state.pieces[1][i])
            throw "Black pieces didn't restore properly";
    }

    if (old_state.castlingRights != curr_state.castlingRights)
        throw "castling rights didn't restore properly";
    if (old_state.fullMoves != curr_state.fullMoves)
        throw "fullMoves didn't restore properly";
    if (old_state.halfMoves != curr_state.halfMoves)
        throw "halfMoves didn't restore properly";
    if (old_state.sideToPlay != curr_state.sideToPlay)
        throw "sideToPlay didn't restore properly";
    if (old_state.enPassantSquare != curr_state.enPassantSquare)
        throw "en passant square didn't restore properly";

    for (int i = 0; i < 64; i++)
    {
        if (curr_mailbox[i].piece_type != old_mailbox[i].piece_type)
        {
            throw "mailbox didn't restore properly";
        }
    }
}