#include <iostream>
#include <cstdint>

using u8 = uint8_t;
using u16 = uint16_t;
using i64 = uint64_t;

struct BoardState
{

    i64 wPawn;
    i64 wKnight;
    i64 wBishop;
    i64 wRook;
    i64 wQueen;
    i64 wKing;
    i64 bPawn;
    i64 bKnight;
    i64 bBishop;
    i64 bRook;
    i64 bQueen;
    i64 bKing;

    u8 castlingRights; // ok... nice

    u8 halfMoves; // one byte can hold up to 255, which is easily in a uint8
                  // but this means 50 plays per side

    u16 fullMoves; // 16 bits is enough; games can go on for very long, but not 2 ^ 32 - 1 = 4294967295 moves

    bool sideToPlay; // white is 0, black is 1

    u8 enPassantSquare; // number 0-63 will be every move
};

class GameState
{

    BoardState state;

public:
    GameState(std::string fen = "")
    {
        if (fen != "")
        {
            state = loadFromFen(fen);
        }

        else {
            
        }
    }

    void setupDefaultBoard()
    {
        // default FEN : rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    }

    BoardState loadFromFen(std::string fen)
    {

        // White pieces are uppercase, black are lowercase
        // Empty space is denoted with a number 1-8
        // Sample FEN: rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1
        //             where each piece is                       _ to play _ castling rights _ en passant square _ halfmove counter _ fullmove counter

        BoardState board;

        for (int i = 0; i < fen.length(); i++)
        {
            char char1 = fen[i];
            std::cout << char1 << std::endl;
        }

        return board;
    }

    std::string exportFen()
    {
    }

    void printBoardState(BoardState state)
    {
    }
};

// we can set up default values for just white and lshift by 8 * rows (5) for pawns and 8 * rows (7) for other pieces

i64 emptyBoard = 0x0000000000000000ULL;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
*/

i64 wPawnDefault = 0xFF00ULL;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
1 1 1 1 1 1 1 1
0 0 0 0 0 0 0 0
*/

i64 wKnightDefault;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 1 0 0 0 0 1 0
*/

i64 wBishopDefault;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 1 0 0 1 0 0
*/

i64 wRookDefault;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
1 0 0 0 0 0 0 1
*/

i64 wQueenDefault;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 1 0 0 0 0
*/

i64 wKingDefault;

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 1 0 0 0
*/