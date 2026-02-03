#include <vector>
#include "bitboard.hpp"
#include "types.hpp"

class MoveGeneration
{

public:
    std::vector<Move> generatePawnMoves(Bitboard &empty, Bitboard &p_state, Side to_play, Square en_passant_square);
    std::vector<Move> generateBishopMoves(Bitboard &empty, Bitboard &b_state, Side to_play);
    std::vector<Move> generateKnightMoves(Bitboard &empty, Bitboard &n_state, Side to_play);
    std::vector<Move> generateRookMoves(Bitboard &empty, Bitboard &r_state, Side to_play);
    std::vector<Move> generateQueenMoves(Bitboard &empty, Bitboard &q_state, Side to_play);
    std::vector<Move> generateKingMoves(Bitboard &empty, Bitboard &k_state, Side to_play, u8 castling_rights);
};
