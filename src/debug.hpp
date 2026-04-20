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
bool check_for_disparities(BoardState old_state, BoardState curr_state, Mailbox old_mailbox, Mailbox curr_mailbox)
{
    for (int i = 0; i < KING + 1; i++)
    {
        if (old_state.pieces[0][i] != curr_state.pieces[0][i])
        {
            std::cout << "white pieces didn't restore properly\n";
            return true;
        }
    }

    for (int i = 0; i < KING + 1; i++)
    {
        if (old_state.pieces[1][i] != curr_state.pieces[1][i])
        {
            std::cout << "Black pieces didn't restore properly\n";
            return true;
        }
    }

    if (old_state.castlingRights != curr_state.castlingRights)
    {
        std::cout << "castling rights didn't restore properly";
        return true;
    }

    if (old_state.fullMoves != curr_state.fullMoves)
    {
        std::cout << "fullMoves didn't restore properly";
        return true;
    }
    if (old_state.halfMoves != curr_state.halfMoves)
    {
        std::cout << "halfMoves didn't restore properly";
        return true;
    }
    if (old_state.sideToPlay != curr_state.sideToPlay)
    {
        std::cout << "sideToPlay didn't restore properly";
        return true;
    }
    if (old_state.enPassantSquare != curr_state.enPassantSquare)
    {
        std::cout << "en passant square didn't restore properly";
        return true;
    }
    for (int i = 0; i < 64; i++)
    {
        if (curr_mailbox[i].piece_type != old_mailbox[i].piece_type)
        {
            std::cout << "mailbox didn't restore properly";
            return true;
        }
    }
    return false;
}

bool hasDuplicates(MoveList move_list)
{
    for (int i = 0; i < move_list.count - 1; i++)
    {
        const Move left_move = move_list.moves[i];
        for (int j = i + 1; j < move_list.count; j++)
        {
            Move right_move = move_list.moves[j];
            if (left_move == right_move)
            {
                print_move(left_move, i);
                std::cout << std::format("Left move {}: ", i) << (int)left_move << '\n';
                std::cout << std::format("Right move {}: ", j) << (int)right_move << '\n';
                return true;
            }
        }
    }
    return false;
}
