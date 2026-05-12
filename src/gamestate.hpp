#pragma once

#include "types.hpp"

class GameState
{

private:
    Move localConvertToMove(Square destination, Square origin, PieceType promotion_piece = PAWN, SpecialMoveFlag flag = NONSPECIAL) const;
    
public:
    BoardState state;
    Mailbox mailbox{};

    GameState(std::string fen = "");

    void loadDefaultBoard();

    // GPT generated
    void resetBoardState();

    void loadFromFen(std::string const fen);

    Bitboard getSideState(Side side) const;

    Bitboard getFullState() const;

    Piece getPieceAt(Square square) const;

    void setPieceAt(Square square, Piece piece);

    void unsetPieceAt(Square square);

    bool isSquareThreatened(Square square, Side bySide) const;

    /// @brief  Moves one piece
    /// @param move A 16-bit integer. Bits 0-5 hold origin, 6-11 hold destination, and 12-16 include special move flags and promotion piece type (not in that order).
    bool make(Move move, Undo &undo);

    void unmake(Move move, Undo undo);

    void make_null();

    void unmake_null(Square original_en_passant_square);

    void UCI_make(std::string str_move);

    Move UCI_convert_to_move(std::string str_move) const;
};