#include <vector>
#include "bitboard.hpp"
#include "types.hpp"

class MoveGeneration
{

public:
    std::vector<Bitboard> generatePawnMoves(Bitboard &empty, Bitboard &p_state, Side to_play, Square en_passant_square);
    std::vector<Bitboard> generateBishopMoves(Bitboard &empty, Bitboard &b_state, Side to_play);
    std::vector<Bitboard> generateKnightMoves(Bitboard &empty, Bitboard &n_state, Side to_play);
    std::vector<Bitboard> generateRookMoves(Bitboard &empty, Bitboard &r_state, Side to_play);
    std::vector<Bitboard> generateQueenMoves(Bitboard &empty, Bitboard &q_state, Side to_play);
    std::vector<Bitboard> generateKingMoves(Bitboard &empty, Bitboard &k_state, Side to_play, u8 castling_rights);
};
