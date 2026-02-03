#include <vector>
#include "movegen.hpp"
#include "bitboard.hpp"
#include "types.hpp"

std::vector<Bitboard> MoveGeneration::generatePawnMoves(Bitboard &empty, Bitboard &p_state, Side to_play, Square en_passant_square)
{

    std::vector<Move> move_list = {};

    
    
}

std::vector<Bitboard> MoveGeneration::generateBishopMoves(Bitboard &empty, Bitboard &bState, Side to_play)
{
}

std::vector<Bitboard> MoveGeneration::generateKnightMoves(Bitboard &empty, Bitboard &nState, Side to_play)
{
}

std::vector<Bitboard> MoveGeneration::generateRookMoves(Bitboard &empty, Bitboard &rState, Side to_play)
{
}

std::vector<Bitboard> MoveGeneration::generateQueenMoves(Bitboard &empty, Bitboard &qState, Side to_play)
{
}

std::vector<Bitboard> MoveGeneration::generateKingMoves(Bitboard &empty, Bitboard &kState, Side to_play, u8 castling_rights)
{
}
