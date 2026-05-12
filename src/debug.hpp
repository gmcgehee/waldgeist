#pragma once

#include "types.hpp"

void print_bits(uint16_t v, int bits);

void print_move(uint16_t m, int idx);

void print_move_bin(uint16_t m, int idx);

bool check_for_disparities(BoardState old_state, BoardState curr_state, Mailbox old_mailbox, Mailbox curr_mailbox);

bool hasDuplicates(MoveList move_list);