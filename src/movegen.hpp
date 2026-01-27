#include <vector>
#include "bitboard.hpp"
#include "types.hpp"

class MoveGeneration
{

public:
    std::vector<Bitboard> generatePawnMoves(BoardState &state, Side to_play, Square en_passant_square);
    std::vector<Bitboard> generateBishopMoves(BoardState &state, Side to_play);
    std::vector<Bitboard> generateKnightMoves(BoardState &state, Side to_play);
    std::vector<Bitboard> generateRookMoves(BoardState &state, Side to_play);
    std::vector<Bitboard> generateQueenMoves(BoardState &state, Side to_play);
    std::vector<Bitboard> generateKingMoves(BoardState &state, Side to_play, u8 castling_rights);
};
