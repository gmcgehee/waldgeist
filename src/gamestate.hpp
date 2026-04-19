#pragma once

#include <string>
#include <set>
#include <immintrin.h>

#include "types.hpp"
#include "bitboard.hpp"
#include "tables.hpp"
#include "../precalculation/bishop/bishop_pext_tables.hpp"
#include "../precalculation/bishop/bishop_ray_masks.hpp"
#include "../precalculation/rook/rook_pext_tables.hpp"
#include "../precalculation/rook/rook_ray_masks.hpp"

#include "debug.hpp"

class GameState
{

public:
    BoardState state;
    Mailbox mailbox{};

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
        return getSideState(BLACK) | getSideState(WHITE);
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

    bool isSquareThreatened(Square square, Side bySide)
    {
        Bitboard occ = getSideState(WHITE) | getSideState(BLACK);
        Bitboard threatened_squares = 0ULL; // will not be all threatened squares (like with knight and pawn moves) but just the relevant ones

        // Pawn-threatened
        if (PAWN_ATTACKS[bySide == WHITE ? BLACK : WHITE][square] & state.pieces[bySide][PAWN])
            return true;

        // Knight-threatened
        if (KNIGHT_MOVES[square] & state.pieces[bySide][KNIGHT])
            return true;

        // Bishop-threatened
        int bishop_pext_index = (int)_pext_u64(occ, BISHOP_RAY_MASKS[square]);
        Bitboard bishop_attacks = BISHOP_PEXT_TABLES[square][bishop_pext_index];

        if (bishop_attacks & state.pieces[bySide][BISHOP])
            return true; // checks if there are any bishops on those diagonals

        // Rook-threatened
        int rook_pext_index = (int)_pext_u64(occ, ROOK_RAY_MASKS[square]);
        Bitboard rook_attacks = ROOK_PEXT_TABLES[square][rook_pext_index];

        if (rook_attacks & state.pieces[bySide][ROOK])
            return true;

        // Queen-threatened
        if ((rook_attacks | bishop_attacks) & state.pieces[bySide][QUEEN])
            return true;

        // King-threatened
        if (KING_MOVES[square] & state.pieces[bySide][KING])
            return true;

        return false;
    }

    /// @brief  Moves one piece
    /// @param move A 16-bit integer. Bits 0-5 hold origin, 6-11 hold destination, and 12-16 include special move flags and promotion piece type (not in that order).
    Undo make(Move move)
    {

        // Temporary for debug
        // std::cout << "\nBitboards: \n"
        //           << getPrintableBoardState(state) << std::endl;
        // std::cout << "\nMailbox: \n"
        //           << getPrintableBoardState(mailbox) << std::endl;

        // DECONSTRUCT THE MOVE
        Square destination = move & 0x3F;
        Square origin = (move >> 6) & 0x3F;
        u8 promotion_piece = ((move >> 12) & 0x3) + 1; // adds 1 because 0 means knight, but in the enum it means pawn
        u8 flag = (move >> 14);

        // Get pieces at squares in order to determine captures
        Piece piece_on_origin = getPieceAt(origin);
        if (piece_on_origin.piece_type == EMPTY)
        {
            printf("\n");
            std::cout << "Failed to find piece on square " << indexToSquare(origin) << std::endl;
            std::cout << exportFen(state) << std::endl;
            std::cout << "\nBitboards: \n"
                      << getPrintableBoardState(state) << std::endl;
            std::cout << "\nMailbox: \n"
                      << getPrintableBoardState(mailbox) << std::endl;

            throw "`make()` attempted to move empty piece square.";
        }

        Side us = piece_on_origin.color; // is it better to do this, or to get state.sideToPlay  ?
        Side them = us == WHITE ? BLACK : WHITE;

        Piece piece_on_destination = getPieceAt(destination);

        Undo undo = {
            move,                  // Move move;
            piece_on_destination,  // Piece captured;
            state.enPassantSquare, // Square en_passant_square;
            state.castlingRights,  // u8 castling_rights;
            state.halfMoves        // u8 halfmoves;
        };

        // DESTINATION PIECE

        undo.captured = getPieceAt(destination);

        if (piece_on_destination.piece_type != EMPTY)
            state.halfMoves = 0;
        state.fullMoves += us == BLACK ? 1 : 0; // every time Black plays, fullMoves increases by 1
        state.sideToPlay = state.sideToPlay == WHITE ? BLACK : WHITE;

        if (piece_on_destination.piece_type == ROOK)
        {
            // castlingRights = 0000 - KQkq

            switch (destination)
            {
            case (a1):
                if (piece_on_origin.color == WHITE)
                    state.castlingRights &= 0b0111;
                break;
            case (h1):
                if (piece_on_origin.color == WHITE)
                    state.castlingRights &= 0b1011;
                break;
            case (a8):
                if (piece_on_origin.color == BLACK)
                    state.castlingRights &= 0b1101;
                break;
            case (h8):
                if (piece_on_origin.color == BLACK)
                    state.castlingRights &= 0b1110;
                break;
            default:
                break;
            }

            state.castlingRights &= 0b1111;
        }

        // ORIGIN PIECE

        if (piece_on_origin.piece_type == ROOK)
        {
            switch (origin)
            {
            case (a1):
                if (piece_on_origin.color == WHITE)
                    state.castlingRights &= 0b0111;
                break;
            case (h1):
                if (piece_on_origin.color == WHITE)
                    state.castlingRights &= 0b1011;
                break;
            case (a8):
                if (piece_on_origin.color == BLACK)
                    state.castlingRights &= 0b1101;
                break;
            case (h8):
                if (piece_on_origin.color == BLACK)
                    state.castlingRights &= 0b1110;
                break;
            default:
                break;
            }
        }

        else if (piece_on_origin.piece_type == KING)
        {
            state.castlingRights &= 0b0011 << (2 * us);
        }

        // Pawn specialties

        // If there's a pawn moving onto the en passant square, it is guaranteed to be capturing.
        // Due to the rules of chess, it's not possible for another pawn to move onto the en passant square except for a capture.
        else if (piece_on_origin.piece_type == PAWN)
        {
            state.halfMoves = 0;
            if (destination == state.enPassantSquare)
            {
                int direction = us == WHITE ? -1 : 1;
                Square piece_captured_square = destination + 8 * direction;
                Piece piece_captured = getPieceAt(piece_captured_square);
                undo.captured = piece_captured;
                unsetPieceAt(piece_captured_square);
            }
            else
            {
                undo.captured = getPieceAt(destination);
            }
        }
        else
        {
            unsetPieceAt(destination);
        }

        state.enPassantSquare = 0;

        unsetPieceAt(origin); // is this always done? I believe so...

        // Modify the game state in a unique manner in special circumstances
        switch (flag)
        {
        case NONSPECIAL:
            setPieceAt(destination, piece_on_origin);
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
            case ROOK:
                setPieceAt(destination, Piece{ROOK, us, &state.pieces[us][ROOK]});
                break;
            case QUEEN:
                setPieceAt(destination, Piece{QUEEN, us, &state.pieces[us][QUEEN]});
                break;
            default:
                break;
            }
            break;
        case PAWN_DOUBLE_PUSH:
            // this needs to unset the piece one above or one below the en passant square. One above if us == WHITE, opposite otherwise
            setPieceAt(destination, piece_on_origin);
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
                Square king_square = __builtin_ctzll(state.pieces[us][KING]);

                // int direction = us == WHITE ? -1 : 1;
                switch (destination - origin)
                {
                case 2: // if the king is moving two to the left, e.g. queenside
                    if (isSquareThreatened(king_square, them) or isSquareThreatened(king_square - 1, them) or isSquareThreatened(king_square - 2, them))
                        unmake(move, undo);
                    else
                    {
                        // king should already be on destination just need to move the rook
                        Square rook_square = destination - 2;
                        unsetPieceAt(rook_square);
                        setPieceAt(rook_square, Piece{ROOK, us, &state.pieces[us][ROOK]});
                    }
                    break;
                case -2: // if the king is moving two to the right, e.g. kingside
                    if (isSquareThreatened(king_square, them) or isSquareThreatened(king_square + 1, them) or isSquareThreatened(king_square + 2, them))
                        unmake(move, undo);
                    else
                    {
                        // king should already be on destination just need to move the rook
                        Square rook_square = destination + 1;
                        unsetPieceAt(rook_square);
                        setPieceAt(rook_square, Piece{ROOK, us, &state.pieces[us][ROOK]});
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }

        if (isSquareThreatened(__builtin_ctzll(state.pieces[us][KING]), them))
        {
            unmake(move, undo);
        }

        return undo; // the trouble: this could return an undo and then unmake even though I've already unmaked.
    }

    void unmake(Move move, Undo undo)
    {

        // Temporary for debug
        // std::cout << "\nBitboards: \n"
        //           << getPrintableBoardState(state) << std::endl;
        // std::cout << "\nMailbox: \n"
        //           << getPrintableBoardState(mailbox) << std::endl;

        if (undo.castling_rights & 0xF0) return;
        // 0b11110000
        // 0b0000KQkq

        // DECONSTRUCT THE MOVE
        // origin and destination are swapped
        Square origin = move & 0x3F;
        Square destination = (move >> 6) & 0x3F;
        u8 promotion_piece = ((move >> 12) & 0x3) + 1; // adds 1 because 0 in Move means knight, but 0 in enum means pawn, which is obviously not a promotion piece
        u8 flag = (move >> 14);

        // Get pieces at squares in order to determine captures
        Piece piece_on_origin = getPieceAt(origin);
        if (piece_on_origin.piece_type == EMPTY)
        {
            printf("\n");
            std::cout << "Failed to find piece on square " << indexToSquare(origin) << std::endl;
            print_move(move, 0);
            std::cout << exportFen(state) << std::endl;
            std::cout << "\nBitboards: \n"
                      << getPrintableBoardState(state) << std::endl;
            std::cout << "\nMailbox: \n"
                      << getPrintableBoardState(mailbox) << std::endl;
            throw "`unmake()` attempted to move empty piece square.";
        }

        Side us = piece_on_origin.color;
        Side them = us == WHITE ? BLACK : WHITE;

        // ALWAYS DONE
        unsetPieceAt(origin);

        // Reset captured piece
        if (origin == state.enPassantSquare && piece_on_origin.piece_type == PAWN)
        {
            int direction = us == WHITE ? -1 : 1;
            setPieceAt(origin + 8 * direction, undo.captured);
        }
        else
        {
            if (undo.captured.piece_type != EMPTY)
                setPieceAt(origin, undo.captured);
        }

        state.enPassantSquare = undo.en_passant_square;
        state.castlingRights = undo.castling_rights;
        state.sideToPlay = state.sideToPlay == WHITE ? BLACK : WHITE;
        state.fullMoves -= state.sideToPlay == BLACK ? 1 : 0;

        // Modify the game state in a unique manner in special circumstances
        switch (flag)
        {
        case NONSPECIAL:
            setPieceAt(destination, piece_on_origin);
            break;
        case PROMOTION:
            setPieceAt(destination, Piece{PAWN, us, &state.pieces[us][PAWN]});
            break;
        case PAWN_DOUBLE_PUSH:
        {
            setPieceAt(destination, piece_on_origin);
            break;
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
                setPieceAt(destination, Piece{KING, us, &state.pieces[us][KING]});
                int direction = origin - destination;

                switch (destination - origin)
                {
                case -2: // if the king has to move back twice, the rook is currently to his left 1 and needs to be moved right twice
                {
                    Square rook_square = origin - 1;
                    unsetPieceAt(rook_square);
                    setPieceAt(destination + 2, Piece{ROOK, us, &state.pieces[us][ROOK]});
                }
                break;
                case 2: // if the king has to move forward twice, the rook is currently to his right 1 and needs to be moved left 3 times
                {
                    Square rook_square = origin + 1;
                    unsetPieceAt(rook_square);
                    setPieceAt(destination - 3, Piece{ROOK, us, &state.pieces[us][ROOK]});
                }
                break;
                default:
                    break;
                }
            }

            break;
        default:
            break;
        }
    }
};
