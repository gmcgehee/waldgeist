#pragma once

#include <iostream>
#include <cassert>
#include <format>
#include <sstream>
#include <chrono>

#include "gamestate.hpp"
#include "bitboard.hpp"
#include "types.hpp"
#include "tables.hpp"
#include "movegen.hpp"
#include "engine.hpp"

void UCI()
{
    std::cout << "\nWaldgeist Engine\n";

    Engine *engine = new Engine();
    GameState *gamestate = &engine->gamestate;

    while (true)

    {
        std::string command;
        getline(std::cin, command);
        std::stringstream ss(command);
        std::vector<std::string> args;
        std::string temp;

        while (getline(ss, temp, ' ')) // Credit to Geeks4Geeks https://www.geeksforgeeks.org/cpp/split-string-by-space-into-vector-in-cpp-stl/
        {
            args.push_back(temp);
        }

        try
        {
            if (args.size() > 0)
            {
                if (args[0] == "quit")
                {
                    return;
                }
                else if (args[0] == "uci")
                {
                    std::cout << "uciok\n";
                }
                else if (args[0] == "isready")
                {
                    std::cout << "readyok\n";
                }
                else if (args[0] == "position")
                {
                    if (args.size() < 2)
                    {
                        continue;
                    }
                    else
                    {
                        if (args[1] == "startpos")
                        {
                            gamestate->loadDefaultBoard();
                        }
                        else if (args[1] == "fen")
                        {
                            std::string fen_string = command.substr(13, command.size() - 13);
                            std::cout << fen_string << '\n';
                            gamestate->loadFromFen(fen_string);
                        }
                    }
                }
                else if (args[0] == "go")
                {
                    if (args.size() < 2)
                    {
                        continue;
                    }
                    else
                    {
                        if (args[1] == "perft")
                        {
                            if (args.size() == 3)
                            {
                                int perft_depth = std::stoi(args[2]);
                                auto start = std::chrono::steady_clock::now();

                                unsigned long long node_count = engine->perft(perft_depth);

                                auto end = std::chrono::steady_clock::now();

                                auto duration = end - start;
                                double seconds = std::chrono::duration<double>(duration).count();
                                double nanoseconds = std::chrono::duration<double, std::nano>(duration).count();

                                std::cout << "\nMove count at PERFT " << perft_depth << ": " << node_count << '\n';
                                std::cout << "Total elapsed time: " << seconds << " s\n";
                                std::cout << "Average time per node: " << (nanoseconds / node_count) << " ns\n";
                                std::cout << "Nodes per second: " << (node_count / seconds) << " nodes/s\n\n";
                            }
                        }
                    }
                }
                else if (args[0] == "clear")
                {
                    for (int i = 0; i < 100; i++)
                        std::cout << '\n';
                }
                else
                {
                    continue;
                }
            }
        }
        catch (std::exception &e)
        {
            std::cout << "\nOops! That had a bug. Please try again.\n";
        }
    }
}
