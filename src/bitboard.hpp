
#ifndef BITBOARD_H
#define BITBOARD_H

#include <iostream>
#include <cstdint>
#include <map>
#include <string>
#include <cctype>
#include "types.hpp"

constexpr Bitboard FILE_A = 0x0101010101010101ULL;
constexpr Bitboard FILE_B = 0x0202020202020202ULL;
constexpr Bitboard FILE_C = 0x0404040404040404ULL;
constexpr Bitboard FILE_D = 0x0808080808080808ULL;
constexpr Bitboard FILE_E = 0x1010101010101010ULL;
constexpr Bitboard FILE_F = 0x2020202020202020ULL;
constexpr Bitboard FILE_G = 0x4040404040404040ULL;
constexpr Bitboard FILE_H = 0x8080808080808080ULL;

constexpr Bitboard RANK_1 = 0x00000000000000FFULL;
constexpr Bitboard RANK_2 = 0x000000000000FF00ULL;
constexpr Bitboard RANK_3 = 0x0000000000FF0000ULL;
constexpr Bitboard RANK_4 = 0x00000000FF000000ULL;
constexpr Bitboard RANK_5 = 0x000000FF00000000ULL;
constexpr Bitboard RANK_6 = 0x0000FF0000000000ULL;
constexpr Bitboard RANK_7 = 0x00FF000000000000ULL;
constexpr Bitboard RANK_8 = 0xFF00000000000000ULL;

inline Bitboard north(Bitboard bb)
{
    return (bb & ~RANK_8) << 8;
}

inline Bitboard south(Bitboard bb)
{
    return (bb & ~RANK_1) >> 8;
}

inline Bitboard west(Bitboard bb)
{
    return (bb & ~FILE_A) >> 1;
}

inline Bitboard east(Bitboard bb)
{
    return (bb & ~FILE_H) << 1;
}

inline Bitboard northw(Bitboard bb)
{
    return (bb & ~FILE_A & ~RANK_8) << 7;
}

inline Bitboard northe(Bitboard bb)
{
    return (bb & ~FILE_H & ~RANK_8) << 9;
}

inline Bitboard southw(Bitboard bb)
{
    return (bb & ~FILE_A & ~RANK_1) >> 9;
}

inline Bitboard southe(Bitboard bb)
{
    return (bb & ~FILE_H & ~RANK_1) >> 7;
}

inline bool get_bit(Bitboard &bb, Square sq)
{
    return bb & (1ULL << sq);
}

inline void set_bit(Bitboard &bb, Square sq) // reference to bitboard because we need to change it
{
    get_bit(bb, sq) ? 0 : bb ^= (1ULL << sq);
}

inline int pop_lsb(Bitboard &bb)
{
    int index = __builtin_ctzll(bb);
    bb &= bb - 1;
    return index;
}

void print_bb(Bitboard bb)
{

    for (int rank = 7; rank >= 0; rank--)
    {
        std::cout << "\n " << std::to_string(rank + 1);

        for (int file = 0; file < 8; file++)
        {
            int index = 8 * rank + file;

            std::cout << ' ' << std::to_string(get_bit(bb, index));
        }
    }

    std::cout << "\n   a b c d e f g h\n";
}

/**
 * @brief Returns the index of a given square. a1 = 0, h1 = 7, a8 = 56, h8 = 63
 * @param square
 * @returns
 */

int squareToIndex(std::string square)
{
    /*
              index 63 ▼
       8 0 0 0 0 0 0 0 0
       7 0 0 0 0 0 0 0 0
       6 0 0 0 0 0 0 0 0
       5 0 0 0 0 0 0 0 0
       4 0 0 0 0 0 0 0 0
       3 0 0 0 0 0 0 0 0
       2 0 0 0 0 0 0 0 0
       1 0 0 0 0 0 0 0 0
         ^ index 0
         a b c d e f g h
    */

    char fileChar = square[0];
    int file = fileChar - 'a'; // gives 0 - 7 value  (GPT idea)

    int row = square[1] - '0'; // somehow this converts the types. Genuinely no idea how. Subtract a char from a char you get an int. Nice.

    int bit_number = 8 * (row - 1) + file;

    return bit_number;
}

std::string indexToSquare(int index)
{
    std::string square = "";

    int fileNum = index % 8;
    std::string fileString(1, 'a' + fileNum);
    square.append(fileString);

    int row = (index - fileNum) / 8 + 1;
    std::string rowString = std::to_string(row);
    square.append(rowString);

    return square;
}

std::string getPieceOnSquare(BoardState state, int index)
{
    Bitboard mask = 1ULL << index;

    if (state.wPawn & mask)
        return "P";
    else if (state.wKnight & mask)
        return "N";
    else if (state.wBishop & mask)
        return "B";
    else if (state.wRook & mask)
        return "R";
    else if (state.wQueen & mask)
        return "Q";
    else if (state.wKing & mask)
        return "K";
    else if (state.bPawn & mask)
        return "p";
    else if (state.bKnight & mask)
        return "n";
    else if (state.bBishop & mask)
        return "b";
    else if (state.bRook & mask)
        return "r";
    else if (state.bQueen & mask)
        return "q";
    else if (state.bKing & mask)
        return "k";
    else
        return "";
}

std::string exportFen(BoardState state)
{
    Bitboard blackBoard = state.bBishop | state.bKnight | state.bPawn | state.bQueen | state.bRook | state.bKing;
    Bitboard whiteBoard = state.wBishop | state.wKnight | state.wPawn | state.wQueen | state.wRook | state.wKing;
    Bitboard fullBoard = blackBoard | whiteBoard;

    std::string fen = "";
    int blankCount = 0;

    for (int row = 7; row >= 0; row--) // the problem is that it starts at 63; fen goes from 56->63, 48->55, etc.
    {
        for (int file = 0; file < 8; file++)

        {
            int index = 8 * row + file;
            Bitboard val = 1ULL << index;
            std::string toAppend;

            if (file == 0 && row < 7)
            {

                if (blankCount > 0)
                {
                    fen.append(std::to_string(blankCount)); // if we have several blank
                    blankCount = 0;
                }

                fen.append("/");
            }

            if (val & fullBoard)
            {
                if (blankCount > 0)
                {
                    fen.append(std::to_string(blankCount)); // if we have several blank
                    // file += blankCount - 1; // to account for automatically incrementing by 1
                    blankCount = 0;
                }

                toAppend = getPieceOnSquare(state, index);
                fen.append(toAppend);
            }
            else
            {
                blankCount += 1;
                continue;
            }
        }
    }
    if (blankCount > 0)
        fen.append(std::to_string(blankCount)); // edge case where there are empty squares on the bottom row

    // Append side to play to FEN
    fen.append(" ");
    std::string to_play = (bool)(state.sideToPlay) ? "b" : "w"; // remember, false means white is playing
    fen.append(to_play);
    fen.append(" ");

    // Append castling rights to FEN
    if (state.castlingRights > 63)
        fen.append("-");

    else
    {
        if (state.castlingRights & 8)
            fen.append("K");

        if (state.castlingRights & 4)
            fen.append("Q");

        if (state.castlingRights & 2)
            fen.append("k");

        if (state.castlingRights & 1)
            fen.append("q");
    }

    fen.append(" ");

    // En passant square
    std::string enPassantSquare;

    if (state.enPassantSquare <= 63)
        enPassantSquare = indexToSquare(state.enPassantSquare);
    else
        enPassantSquare = "-";

    fen.append(enPassantSquare);

    fen.append(" ");

    std::string halfmoveCounter = std::to_string(state.halfMoves);
    fen.append(halfmoveCounter);

    fen.append(" ");

    std::string fullmoveCounter = std::to_string(state.fullMoves);
    fen.append(fullmoveCounter);

    return fen;
}

std::string getPrintableBoardState(BoardState state)
{
    std::string fen = exportFen(state);
    std::string printableBoard;

    for (int i = 0; i <= fen.length(); i++)
    {
        char c = fen[i];

        if (c == ' ')
        {
            return printableBoard;
        }
        else if (c == '/')
        {
            printableBoard.append("\n");
        }
        else if (std::isdigit(c))
        {
            for (int j = 0; j < c - '0'; j++)
            {
                printableBoard.append(" .");
            }
        }
        else if (std::isalpha(c))
        {
            std::string to_append(1, c);
            to_append = " " + to_append;
            printableBoard.append(to_append);
        }
        else
        {
            // if it isn't a /, ' ', digit, or otherwise--what could it be?

            // throw error
            throw std::invalid_argument("Internal FEN conversion output invalid character.");
            return printableBoard;
        }
    }

    return printableBoard;
}

BoardState loadFromFen(std::string const &fen)
    {

        // White pieces are uppercase, black are lowercase
        // Empty space is denoted with a number 1-8
        // Sample FEN: rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1
        //             where each piece is                       _ to play _ castling rights _ en passant square _ halfmove counter _ fullmove counter

        BoardState board;

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

                    // is this the most efficient/best practice way to do this? row 0 file 0 is lshift 63
                    u8 shamt = (8 * curr_row + curr_file); // the first piece, at a8, is lshamt 56

                    Bitboard *piecetype_to_modify = nullptr;

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
                    board.sideToPlay = Side::BLACK;
                }
                else if (c == 'w')
                {
                    board.sideToPlay = Side::WHITE;
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
                    board.castlingRights |= (1 << 3); // is the speed difference worthwhile? should I precompute these values? it would look less readable.
                }
                else if (c == 'Q')
                {
                    board.castlingRights |= (1 << 2);
                }
                else if (c == 'k')
                {
                    board.castlingRights |= (1 << 1);
                }
                else if (c == 'q')
                {
                    board.castlingRights |= 1;
                }
                else if (c == '-')
                {
                    board.castlingRights = 128;
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
                    board.enPassantSquare = 128; // TEMPORARY: if there is no en passant square, have some number too big to be on the board
                    continue;
                }

                else if (std::isalpha(c))
                {
                    std::string square;
                    square += c;
                    square += fen[i + 1]; // the cleanest way to add two chars to a string
                    int index = squareToIndex(square);

                    board.enPassantSquare = index; // this will be 63 if the square is a8 and 0 if it is h1
                }
            }
            else if (fieldnumber == 4)
            {
                if (fen[i + 1] == ' ')
                {
                    board.halfMoves = c - '0'; // again, this converts c to an int
                }
                else
                {
                    std::string halfMoveCounter;
                    halfMoveCounter += c;
                    halfMoveCounter += fen[i + 1];
                    board.halfMoves = std::stoi(halfMoveCounter);
                    i++; // necessary to make it skip
                }
            }
            else if (fieldnumber == 5)
            {
                std::string fullMoveCounter = fen.substr(i, fen.length() - i);
                board.fullMoves = std::stoi(fullMoveCounter);
                break; // do not run after field 5
            }
        }

        return board;
    }

#endif