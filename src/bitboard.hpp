#pragma once

#include <string>

#include "types.hpp"

constexpr Bitboard FILE_A = 0x0101010101010101ULL;
constexpr Bitboard FILE_B = 0x0202020202020202ULL;
constexpr Bitboard FILE_C = 0x0404040404040404ULL;
constexpr Bitboard FILE_D = 0x0808080808080808ULL;
constexpr Bitboard FILE_E = 0x1010101010101010ULL;
constexpr Bitboard FILE_F = 0x2020202020202020ULL;
constexpr Bitboard FILE_G = 0x4040404040404040ULL;
constexpr Bitboard FILE_H = 0x8080808080808080ULL;

constexpr Bitboard RANK_1 = 0x00000000000000FFULL;
constexpr Bitboard RANK_2 = 0x000000000000FF00ULL;
constexpr Bitboard RANK_3 = 0x0000000000FF0000ULL;
constexpr Bitboard RANK_4 = 0x00000000FF000000ULL;
constexpr Bitboard RANK_5 = 0x000000FF00000000ULL;
constexpr Bitboard RANK_6 = 0x0000FF0000000000ULL;
constexpr Bitboard RANK_7 = 0x00FF000000000000ULL;
constexpr Bitboard RANK_8 = 0xFF00000000000000ULL;

Bitboard north(Bitboard bb);

Bitboard south(Bitboard bb);

Bitboard west(Bitboard bb);

Bitboard east(Bitboard bb);

Bitboard northw(Bitboard bb);

Bitboard northe(Bitboard bb);

Bitboard southw(Bitboard bb);

Bitboard southe(Bitboard bb);

bool get_bit(Bitboard &bb, Square sq);

void set_bit(Bitboard &bb, Square sq);

void unset_bit(Bitboard &bb, Square sq);

Square pop_lsb(Bitboard &bb);

Square inverse_square(Square sq);

void print_bb(Bitboard bb);

/**
 * @brief Returns the index of a given square. a1 = 0, h1 = 7, a8 = 56, h8 = 63
 * @param square
 * @returns
 */

int squareToIndex(std::string square);

std::string indexToSquare(int index);

std::string ____getPieceOnSquare(BoardState state, Square sq);

std::string exportFen(BoardState state);

std::string getPrintableBoardState(std::array<Piece, 64> mailbox);

std::string getPrintableBoardState(BoardState state);

BoardState loadFromFen(std::string const fen);

Square get_destination_square(Move move);

Square get_origin_square(Move move);

u8 get_flag(Move move);