#include <iostream>
#include <cassert>
#include <format>

#include "gamestate.hpp"
#include "bitboard.hpp"
#include "types.hpp"
#include "tables.hpp"
#include "movegen.hpp"
#include "engine.hpp"

// temporary
#include <chrono>

int main()
{

    Engine *engine = new Engine();

    engine->gamestate.loadFromFen("qqqqqqqq/qqqqqqqq/qqqqqqqq/qqqqqqqq/qqqqqqqq/qqqqqqqq/qqqqqqqq/qqqqKqqq w - - 0 1");

    std::cout << engine->eval(engine->gamestate.state) << std::endl;

    return 0;
}