#include <iostream>
#include <cstdint>
#include <map>

#ifndef BITBOARD_H
#define BITBOARD_H

using u8 = uint8_t;
using u16 = uint16_t;
using i64 = uint64_t;

struct BoardState
{

    // Bitboard of 1s and 0s showing every current position of pieces

    i64 wPawn = 0ULL;
    i64 wKnight = 0ULL;
    i64 wBishop = 0ULL;
    i64 wRook = 0ULL;
    i64 wQueen = 0ULL;
    i64 wKing = 0ULL;

    i64 bPawn = 0ULL;
    i64 bKnight = 0ULL;
    i64 bBishop = 0ULL;
    i64 bRook = 0ULL;
    i64 bQueen = 0ULL;
    i64 bKing = 0ULL;

    u8 castlingRights = 0U; // ok... nice

    u8 enPassantSquare = 0U; // number 0-63; 0 is the 0th bit, meaning LSB

    u8 halfMoves = 0U; // one byte can hold up to 255, which is easily in a uint8
                       // but this means 50 plays per side

    u16 fullMoves = 0U; // 16 bits is enough; games can go on for very long, but not 2 ^ 32 - 1 = 4294967295 moves

    bool sideToPlay = false; // white is 0, black is 1

};

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

        else
        {
            // do I need to do anything here?
        }
    }

    static u8 squareToString(std::string square) {
        // Bit 0 is a8 on a chess board


        /*
        
        ▼ bit 63
           8 0 0 0 0 0 0 0 0
           7 0 0 0 0 0 0 0 0
           6 0 0 0 0 0 0 0 0
           5 0 0 0 0 0 0 0 0
           4 0 0 0 0 0 0 0 0
           3 0 0 0 0 0 0 0 0
           2 0 0 0 0 0 0 0 0
           1 0 0 0 0 0 0 0 0
                        ^ bit 0 
             a b c d e f g h
        */

        // a8 = 63
        // h8 = 0
        // number = 63 - ()

        std::map<char, u8> letter_value 
        {
            {'a', 1},
            {'b', 1},
            {'c', 1},
            {'d', 1},
            {'e', 1},
            {'f', 1},
            {'g', 1},
            {'h', 1},
        };

        u8 bit_number;

        return bit_number;

    }

    void setupDefaultBoard()
    {
        // default FEN : rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
        // Convenience method
        state = loadFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    static BoardState loadFromFen(std::string const &fen)
    {

        // White pieces are uppercase, black are lowercase
        // Empty space is denoted with a number 1-8
        // Sample FEN: rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1
        //             where each piece is                       _ to play _ castling rights _ en passant square _ halfmove counter _ fullmove counter

        BoardState board;

        u8 curr_row = 0;
        u8 curr_file = 0;

        u8 fieldnumber = 0;

        for (char c : fen)
        {

            if (c == ' ')
            {
                fieldnumber += 1;
                continue;
            }

            if (fieldnumber == 0)
            {
                if (std::isdigit(static_cast<unsigned char>(c)))
                {
                    curr_file += (int)c;
                }
                else if (c == '/')
                {

                    curr_row += 1;
                    curr_file = 0;
                }

                else if (std::isalpha(c))
                {

                    // is this the most efficient/best practice way to do this? row 0 file 0 is lshift 63
                    u8 shamt = 63 - ((curr_row * 8) + curr_file);
                    i64 *piecetype_to_modify = nullptr;

                    switch (c)
                    {
                    case 'r':
                        piecetype_to_modify = &board.bRook;
                        break;
                    case 'n':
                        piecetype_to_modify = &board.bKnight;
                        break;
                    case 'b':
                        piecetype_to_modify = &board.bBishop;
                        break;
                    case 'q':
                        piecetype_to_modify = &board.bQueen;
                        break;
                    case 'k':
                        piecetype_to_modify = &board.bKing;
                        break;
                    case 'p':
                        piecetype_to_modify = &board.bPawn;
                        break;
                    case 'R':
                        piecetype_to_modify = &board.wRook;
                        break;
                    case 'N':
                        piecetype_to_modify = &board.wKnight;
                        break;
                    case 'B':
                        piecetype_to_modify = &board.wBishop;
                        break;
                    case 'Q':
                        piecetype_to_modify = &board.wQueen;
                        break;
                    case 'K':
                        piecetype_to_modify = &board.wKing;
                        break;
                    case 'P':
                        piecetype_to_modify = &board.wPawn;
                        break;
                    default:
                        std::cout << "C: " << c << std::endl;
                        throw std::invalid_argument("Unexpected value in FEN field 'pieces'.");
                        break;
                    }

                    *piecetype_to_modify |= (1ULL << shamt);
                    curr_file += 1;
                }
            }
            else if (fieldnumber == 1)
            {

                if (c == 'b')
                {
                    board.sideToPlay = true;
                }
                else if (c == 'w')
                {
                    board.sideToPlay = false;
                }
                else
                {
                    std::cout << "C: " << c << std::endl;
                    throw std::invalid_argument("Unexpected value in FEN field 'side to play'.");
                }

                continue;
            }
            else if (fieldnumber == 2) {
                // right nibble keeps track of castling rights (white kingside, white queenside, black kingside, black queenside)

                if (c == 'K') {
                    board.castlingRights |= (1 << 3);   // is the speed difference worthwhile? should I precompute these values? it would look less readable.
                } else if (c == 'Q') {
                    board.castlingRights |= (1 << 2);
                } else if (c == 'k') {
                    board.castlingRights |= (1 << 1);
                } else if (c == 'q') {
                    board.castlingRights |= 1;
                } else {
                    std::cout << "C: " << c << std::endl;
                    throw std::invalid_argument("Unexpected value in FEN field 'castling rights'.");
                }

            } else if (fieldnumber == 3) {

            }
        }

        return board;
    }

    std::string exportFen()
    {
        return "";
    }

    void printBoardState(BoardState state)
    {
    }
};

#endif