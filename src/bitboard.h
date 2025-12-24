#include <iostream>
#include <cstdint>
#include <map>
#include <string>
#include <cctype>

#ifndef BITBOARD_H
#define BITBOARD_H

using u8 = uint8_t;
using u16 = uint16_t;
using Bitboard = uint64_t;

struct BoardState
{

    // Bitboard of 1s and 0s showing every current position of pieces

    Bitboard wPawn = 0ULL;
    Bitboard wKnight = 0ULL;
    Bitboard wBishop = 0ULL;
    Bitboard wRook = 0ULL;
    Bitboard wQueen = 0ULL;
    Bitboard wKing = 0ULL;

    Bitboard bPawn = 0ULL;
    Bitboard bKnight = 0ULL;
    Bitboard bBishop = 0ULL;
    Bitboard bRook = 0ULL;
    Bitboard bQueen = 0ULL;
    Bitboard bKing = 0ULL;

    u8 castlingRights = 0U; // ok... nice

    u8 enPassantSquare = 0U; // number 0-63; 0 is the 0th bit, meaning LSB

    u8 halfMoves = 0U; // one byte can hold up to 255, which is easily in a uint8
                       // but this means 50 plays per side

    u16 fullMoves = 0U; // 16 bits is enough; games can go on for very long, but not 2 ** 32 - 1 = 4294967295 moves

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
        // index += 1;

        int fileNum = index % 8;
        std::string fileString(1, 'a' + fileNum);
        square.append(fileString);

        int row = (index - fileNum) / 8 + 1;
        std::string rowString = std::to_string(row);
        square.append(rowString);

        return square;
    }

    void setupDefaultBoard()
    {
        // default FEN : rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
        // Convenience method
        state = loadFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    BoardState loadFromFen(std::string const &fen)
    {

        // White pieces are uppercase, black are lowercase
        // Empty space is denoted with a number 1-8
        // Sample FEN: rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1
        //             where each piece is                       _ to play _ castling rights _ en passant square _ halfmove counter _ fullmove counter

        BoardState board;

        u8 curr_row = 0;
        u8 curr_file = 0;

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

                    curr_row += 1;
                    curr_file = 0;
                }

                else if (std::isalpha(c))
                {

                    // is this the most efficient/best practice way to do this? row 0 file 0 is lshift 63
                    u8 shamt = 63 - (8 * curr_row + curr_file);

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

    std::string exportFen(BoardState state)
    {
        Bitboard blackBoard = state.bBishop | state.bKnight | state.bPawn | state.bQueen | state.bRook | state.bKing;
        Bitboard whiteBoard = state.wBishop | state.wKnight | state.wPawn | state.wQueen | state.wRook | state.wKing;
        Bitboard fullBoard = blackBoard | whiteBoard;

        std::string fen = "";
        int blankCount = 0;

        for (int i = 63; i >= 0; i--)
        {

            Bitboard val = 1ULL << i;
            std::string toAppend;

            if ((i + 1) % 8 == 0 && i < 63) // this is lowkey unreadable
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
                    blankCount = 0;
                }
            }
            else
            {
                blankCount += 1;
                continue;
            }

            if (val & state.bBishop)
            {
                toAppend = "b";
            }
            else if (val & state.bKing)
            {
                toAppend = "k";
            }
            else if (val & state.bKnight)
            {
                toAppend = "n";
            }
            else if (val & state.bPawn)
            {
                toAppend = "p";
            }
            else if (val & state.bQueen)
            {
                toAppend = "q";
            }
            else if (val & state.bRook)
            {
                toAppend = "r";
            }
            else if (val & state.wBishop)
            {
                toAppend = "B";
            }
            else if (val & state.wKing)
            {
                toAppend = "K";
            }
            else if (val & state.wKnight)
            {
                toAppend = "N";
            }
            else if (val & state.wPawn)
            {
                toAppend = "P";
            }
            else if (val & state.wQueen)
            {
                toAppend = "Q";
            }
            else if (val & state.wRook)
            {
                toAppend = "R";
            }
            else
            {
                // what do do if the current square is blank
            }
            fen.append(toAppend);
        }

        if (blankCount > 0)
            fen.append(std::to_string(blankCount)); // edge case where there are empty squares on the bottom row

        // Append side to play to FEN
        fen.append(" ");
        std::string to_play = (state.sideToPlay) ? "b" : "w"; // remember, false means white is playing
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
};

#endif