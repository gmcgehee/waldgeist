#include <vector>
#include "bitboard.hpp"
#include "types.hpp"

class MoveGeneration
{

public:
    std::vector<Bitboard> generatePawnMoves(Bitboard &pState, Side to_play, Square en_passant_square);
    std::vector<Bitboard> generateBishopMoves(Bitboard &bState, Side to_play);
    std::vector<Bitboard> generateKnightMoves(Bitboard &nState, Side to_play);
    std::vector<Bitboard> generateRookMoves(Bitboard &rState, Side to_play);
    std::vector<Bitboard> generateQueenMoves(Bitboard &qState, Side to_play);
    std::vector<Bitboard> generateKingMoves(Bitboard &kState, Side to_play, u8 castling_rights);
};
