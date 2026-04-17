#pragma once

#include <string>
#include <set>

#include "types.hpp"
#include "bitboard.hpp"

class GameState
{

public:
    BoardState state;
    std::array<Piece, 64> mailbox{};
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
        for (int side = 0; side < 2; ++side)
        {
            for (int pt = 0; pt < 6; ++pt)
            {
                state.pieces[side][pt] = 0ULL;
            }
        }

        state.castlingRights = 0U;
        state.enPassantSquare = 0U;

        state.halfMoves = 0U;
        state.fullMoves = 0U;

        state.sideToPlay = Side::WHITE;

        for (int i = 0; i < 64; i++)
        {
            mailbox[i] = Piece{EMPTY, WHITE, NULL};
        }
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

                else if (std::isalpha(static_cast<unsigned char>(c)))
                {
                    Square sq = (8 * curr_row + curr_file);

                    Side side = std::islower(static_cast<unsigned char>(c)) ? BLACK : WHITE;
                    PieceType pieceType;

                    switch (std::tolower(static_cast<unsigned char>(c)))
                    {
                    case 'p':
                        pieceType = PAWN;
                        break;
                    case 'n':
                        pieceType = KNIGHT;
                        break;
                    case 'b':
                        pieceType = BISHOP;
                        break;
                    case 'r':
                        pieceType = ROOK;
                        break;
                    case 'q':
                        pieceType = QUEEN;
                        break;
                    case 'k':
                        pieceType = KING;
                        break;
                    default:
                        std::cout << "C: " << c << std::endl;
                        throw std::invalid_argument("Unexpected value in FEN field 'pieces'.");
                    }

                    Bitboard *piecetype_to_modify = &state.pieces[side][pieceType];
                    mailbox[sq] = Piece{pieceType, side, piecetype_to_modify};

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
        Bitboard result = 0ULL;

        for (int pt = 0; pt < 6; ++pt)
        {
            result |= state.pieces[side][pt];
        }

        return result;
    }

    Bitboard getFullState()
    {
        return getSideState(BLACK) & getSideState(WHITE);
    }

    inline Piece getPieceAt(Square square)
    {
        return mailbox[square];
    }

    inline void setPieceAt(Square square, Piece piece)
    {
        mailbox[square] = piece;
        *piece.piece_bb |= 1ULL << square;
    }

    inline void unsetPieceAt(Square square)
    {
        Piece piece = getPieceAt(square);
        if (piece.piece_type == EMPTY)
            return;
        mailbox[square] = Piece{EMPTY, WHITE, NULL};
        unset_bit(*piece.piece_bb, square);
    }

    bool isSquareThreatened(Square square)
    {
        // how am I even going to do this?
        return false;
    }

    /// @brief  Moves one piece
    /// @param move A 16-bit integer. Bits 0-5 hold origin, 6-11 hold destination, and 12-16 include special move flags and promotion piece type (not in that order).
    Undo make(Move move)
    {

        // I somehow need to check if the king will continue to be in check after making a move, but without making the move

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

        Undo undo;

        // DECONSTRUCT THE MOVE
        Square destination = move & 0x3F;
        Square origin = (move >> 6) & 0x3F;
        u8 promotion_piece = ((move >> 12) & 0x3); // adds 2 because 0 means knight, but in the enum it means empty
        u8 flag = (move >> 14);

        // Get pieces at squares in order to determine captures
        Piece piece_on_origin = getPieceAt(origin);
        if (piece_on_origin.piece_type == EMPTY)
            throw "`make()` attempted to move empty piece square.";
        Side us = piece_on_origin.color;

        Piece piece_on_destination = getPieceAt(destination);

        // Modify non-piece BoardState elements

        if (piece_on_destination.piece_type != EMPTY)
            state.halfMoves = 0;
        state.fullMoves += state.sideToPlay == BLACK ? 1 : 0; // every time Black plays, fullMoves increases by 1
        state.sideToPlay = state.sideToPlay == WHITE ? BLACK : WHITE;

        // Pawn specialties

        // If there's a pawn moving onto the en passant square, it is guaranteed to be capturing.
        // Due to the rules of chess, it's not possible for another pawn to move onto the en passant square except for a capture.
        if (piece_on_origin.piece_type == PAWN)
        {
            state.halfMoves = 0;
            if (destination == state.enPassantSquare)
            {
                int direction = us == WHITE ? -1 : 1;
                unsetPieceAt(destination + 8 * direction);
            }
        }

        state.enPassantSquare = 0;

        unsetPieceAt(origin); // is this always done? I believe so...
        setPieceAt(destination, piece_on_origin);

        // Modify the game state in a unique manner in special circumstances
        switch (flag)
        {
        case NONSPECIAL:
            break;
        case PROMOTION:
            switch (promotion_piece)
            {
            case KNIGHT:
                setPieceAt(destination, Piece{KNIGHT, us, &state.pieces[us][KNIGHT]});
                break;
            case BISHOP:
                setPieceAt(destination, Piece{BISHOP, us, &state.pieces[us][BISHOP]});
                break;
            case QUEEN:
                setPieceAt(destination, Piece{QUEEN, us, &state.pieces[us][QUEEN]});
                break;
            default:
                break;
            }
            break;
        case EN_PASSANT:
            // this needs to unset the piece one above or one below the en passant square. One above if us == WHITE, opposite otherwise
            {
                int direction = us == WHITE ? -1 : 1;
                state.enPassantSquare = destination + (8 * direction);
            }
            break;
        case CASTLING:
            // needs logic to test if the squares in-between are in check and remove castling rights
            /*
             Steps:
             1. determine black/white
             2. determine kingside/queenside
            */
            {
                Square king_square = pop_lsb(state.pieces[us][KING]);

                int direction = us == WHITE ? -1 : 1;
                switch (destination - origin)
                {
                case 2: // if the king is moving two to the left, e.g. queenside
                    if (isSquareThreatened(king_square) or isSquareThreatened(king_square - 1) or isSquareThreatened(king_square - 2))
                        unmake(move, undo);
                    break;
                case -2: // if the king is moving two to the right, e.g. kingside
                    if (isSquareThreatened(king_square) or isSquareThreatened(king_square + 1) or isSquareThreatened(king_square + 2))
                        unmake(move, undo);
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }

        if (isSquareThreatened(pop_lsb(state.pieces[us][KING])))
        {
            unmake(move, undo);
        }

        return undo;
    }

    void unmake(Move move, Undo undo)
    {
    }
};
