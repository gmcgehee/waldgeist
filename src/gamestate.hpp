#pragma once

#include <string>
#include <set>

#include "types.hpp"
#include "bitboard.hpp"

class GameState
{

public:
    BoardState state;
    std::array<Piece, 64> mailbox;
    std::set<Square> attacked_squares;

    GameState(std::string fen = "")
    {
        if (fen != "")
        {
            loadFromFen(fen);
        }
    }

    void loadDefaultBoard()
    {
        // Convenience method
        loadFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    // GPT generated
    void resetBoardState()
    {
        state.wPawn = 0ULL;
        state.wKnight = 0ULL;
        state.wBishop = 0ULL;
        state.wRook = 0ULL;
        state.wQueen = 0ULL;
        state.wKing = 0ULL;

        state.bPawn = 0ULL;
        state.bKnight = 0ULL;
        state.bBishop = 0ULL;
        state.bRook = 0ULL;
        state.bQueen = 0ULL;
        state.bKing = 0ULL;

        state.castlingRights = 0U;
        state.enPassantSquare = 0U;

        state.halfMoves = 0U;
        state.fullMoves = 0U;

        state.sideToPlay = Side::WHITE;
    }

    void loadFromFen(std::string const fen)
    {
        // NOTE: Move this to GameState class

        // White pieces are uppercase, black are lowercase
        // Empty space is denoted with a number 1-8
        // Sample FEN: rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1
        //             where each piece is                       _ to play _ castling rights _ en passant square _ halfmove counter _ fullmove counter

        resetBoardState();

        u8 curr_row = 7;  // row 8
        u8 curr_file = 0; // file a

        u8 fieldnumber = 0;

        for (int i = 0; i < fen.length(); i++)
        {

            char c = fen[i];

            if (c == ' ')
            {
                fieldnumber += 1;
                continue;
            }

            if (fieldnumber == 0)
            {
                if (std::isdigit(static_cast<unsigned char>(c)))
                {
                    curr_file += c - '0'; // was that it
                }

                else if (c == '/')
                {

                    curr_row -= 1;
                    curr_file = 0;
                }

                else if (std::isalpha(c))
                {

                    // is this the most efficient/best practice way to do this?
                    Square sq = (8 * curr_row + curr_file); // the first piece, at a8, is lshamt 56

                    Bitboard *piecetype_to_modify = nullptr;

                    switch (c)
                    {
                    case 'r':
                        piecetype_to_modify = &state.bRook;
                        mailbox[sq] = Piece{PieceType::ROOK, Side::BLACK};
                        break;
                    case 'n':
                        piecetype_to_modify = &state.bKnight;
                        mailbox[sq] = Piece{PieceType::KNIGHT, Side::BLACK};

                        break;
                    case 'b':
                        piecetype_to_modify = &state.bBishop;
                        mailbox[sq] = Piece{PieceType::BISHOP, Side::BLACK};
                        break;
                    case 'q':
                        piecetype_to_modify = &state.bQueen;
                        mailbox[sq] = Piece{PieceType::QUEEN, Side::BLACK};
                        break;
                    case 'k':
                        piecetype_to_modify = &state.bKing;
                        mailbox[sq] = Piece{PieceType::KING, Side::BLACK};
                        break;
                    case 'p':
                        piecetype_to_modify = &state.bPawn;
                        mailbox[sq] = Piece{PieceType::PAWN, Side::BLACK};
                        break;
                    case 'R':
                        piecetype_to_modify = &state.wRook;
                        mailbox[sq] = Piece{PieceType::ROOK, Side::WHITE};
                        break;
                    case 'N':
                        piecetype_to_modify = &state.wKnight;
                        mailbox[sq] = Piece{PieceType::KNIGHT, Side::WHITE};
                        break;
                    case 'B':
                        piecetype_to_modify = &state.wBishop;
                        mailbox[sq] = Piece{PieceType::BISHOP, Side::WHITE};
                        break;
                    case 'Q':
                        piecetype_to_modify = &state.wQueen;
                        mailbox[sq] = Piece{PieceType::QUEEN, Side::WHITE};
                        break;
                    case 'K':
                        piecetype_to_modify = &state.wKing;
                        mailbox[sq] = Piece{PieceType::KING, Side::WHITE};
                        break;
                    case 'P':
                        piecetype_to_modify = &state.wPawn;
                        mailbox[sq] = Piece{PieceType::PAWN, Side::WHITE};
                        break;
                    default:
                        std::cout << "C: " << c << std::endl;
                        throw std::invalid_argument("Unexpected value in FEN field 'pieces'.");
                        break;
                    }

                    *piecetype_to_modify |= (1ULL << sq);
                    curr_file += 1;
                }
            }
            else if (fieldnumber == 1)
            {

                if (c == 'b')
                {
                    state.sideToPlay = Side::BLACK;
                }
                else if (c == 'w')
                {
                    state.sideToPlay = Side::WHITE;
                }
                else
                {
                    std::cout << "C: " << c << std::endl;
                    throw std::invalid_argument("Unexpected value in FEN field 'side to play'.");
                }

                continue;
            }
            else if (fieldnumber == 2)
            {
                // right nibble keeps track of castling rights (white kingside, white queenside, black kingside, black queenside)

                if (c == 'K')
                {
                    state.castlingRights |= (1 << 3); // is the speed difference worthwhile? should I precompute these values? it would look less readable.
                }
                else if (c == 'Q')
                {
                    state.castlingRights |= (1 << 2);
                }
                else if (c == 'k')
                {
                    state.castlingRights |= (1 << 1);
                }
                else if (c == 'q')
                {
                    state.castlingRights |= 1;
                }
                else if (c == '-')
                {
                    state.castlingRights = 128;
                }
                else
                {
                    std::cout << "C: " << c << std::endl;
                    throw std::invalid_argument("Unexpected value in FEN field 'castling rights'.");
                }
            }
            else if (fieldnumber == 3)
            {

                if (c == '-')
                {
                    state.enPassantSquare = 0;
                    continue;
                }

                else if (std::isalpha(c))
                {
                    std::string square;
                    square += c;
                    square += fen[i + 1]; // the cleanest way to add two chars to a string
                    int index = squareToIndex(square);

                    state.enPassantSquare = index; // this will be 63 if the square is a8 and 0 if it is h1
                }
            }
            else if (fieldnumber == 4)
            {
                if (fen[i + 1] == ' ')
                {
                    state.halfMoves = c - '0'; // again, this converts c to an int
                }
                else
                {
                    std::string halfMoveCounter;
                    halfMoveCounter += c;
                    halfMoveCounter += fen[i + 1];
                    state.halfMoves = std::stoi(halfMoveCounter);
                    i++; // necessary to make it skip
                }
            }
            else if (fieldnumber == 5)
            {
                std::string fullMoveCounter = fen.substr(i, fen.length() - i);
                state.fullMoves = std::stoi(fullMoveCounter);
                break;
            }
        }
    }

    Bitboard getSideState(Side side)
    {
        if (side == BLACK)
        {
            return state.bPawn | state.bKnight | state.bBishop | state.bRook | state.bQueen | state.bKing;
        }
        else
        { // (side == WHITE)
            return state.wPawn | state.wKnight | state.wBishop | state.wRook | state.wQueen | state.wKing;
        }
    }

    Bitboard getFullState()
    {
        return getSideState(BLACK) & getSideState(WHITE);
    }

    Piece getPieceAt(Square square)
    {
        return mailbox[square];
    }

    bool isSquareThreatened(Square square)
    {
    }

    /// @brief  Moves one piece
    /// @param move A 16-bit integer. Bits 0-5 hold origin, 6-11 hold destination, and 12-16 include special move flags and promotion piece type (not in that order).
    void make(Move move)
    {

        /*

            What to account for:
             - castling rights changes
             - piece moving
             - en passant square
             - counters (halfmove, fullmove)
             - side to play
            In other words, keep everything in the gamestate

            Update both the mailbox and the bitboards!

        */

        Square origin = move & 0x1F;
        Square destination = (move >> 6) & 0x1F;
        u8 promotion_piece = ((move >> 12) & 0x3) + 2; // adds 2 because 0 means knight, but in the enum it means empty
        u8 flag = (move >> 14);

        Piece piece_on_origin = getPieceAt(origin);
        Piece piece_on_destination = getPieceAt(destination);

        
    }

    void unmake(Move move)
    {
    }
};
