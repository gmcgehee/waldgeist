#pragma once

#include "types.hpp"
#include "bitboard.hpp"
#include <string>

class GameState
{

public:
    BoardState state;

    GameState(std::string fen = "")
    {
        if (fen != "")
        {
            state = loadFromFen(fen);
        }
    }

    void setupDefaultBoard()
    {
        // Convenience method
        // default FEN : rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
        state = loadFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    Bitboard getSideState(Side side)
    {
        if (side == BLACK)
        {
            return state.bPawn | state.bKnight | state.bBishop | state.bRook | state.bQueen | state.bKing;
        }
        else { // (side == WHITE) 
            return state.wPawn | state.wKnight | state.wBishop | state.wRook | state.wQueen | state.wKing;
        }
    }
};
