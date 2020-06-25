#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "game.hpp"
#include "setup.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("TESTING ===> infected_neig(Board, Pos) && evolve(Board) && main "
          "board member functions") {
  n = 50;
  B = 1.f;

  State state{State::I};
  Pos pos{3, 3};

  // range = 1 /////////////////////////////////////////////////////////////////////////////////
  range = 1.f;
  Board board{n, 0.f, range, Condition{0.f, 0.f, 0.f, N, B, 0.f}, 0};

  if (range == doctest::Approx(1.f)) {
    CHECK(infected_neig(board, pos) == 0);

    for (int x = 0; x != n; ++x) {
      for (int y = 0; y != n; ++y) {
        board.change_state(Pos{x, y}, State::S);
      }
    }
    board.change_state(pos, state);

    board = evolve(board);
    CHECK(infected_neig(board, pos) == 4);

    // board.change_state(Pos{50, 50}, state); // causes crash because it's out
    // of domain of the grid
  }

  // range = 2 //////////////////////////////////////////////////////////////////////////////////
  range = 2.f;
  board = Board{n, 0.f, range, Condition{0.f, 0.f, 0.f, N, B, 0.f}, 0};

  if (range == doctest::Approx(2.f)) {
    CHECK(infected_neig(board, pos) == 0);

    for (int x = 0; x != n; ++x) {
      for (int y = 0; y != n; ++y) {
        board.change_state(Pos{x, y}, State::S);
      }
    }
    board.change_state(pos, state);

    board = evolve(board);
    CHECK(infected_neig(board, pos) == 12);
  }

  // range = 3 /////////////////////////////////////////////////////////////////////////////////
  range = 3.f;
  board = Board{n, 0.f, range, Condition{0.f, 0.f, 0.f, N, B, 0.f}, 0};

  if (range == doctest::Approx(3.f)) {
    CHECK(infected_neig(board, pos) == 0);

    for (int x = 0; x != n; ++x) {
      for (int y = 0; y != n; ++y) {
        board.change_state(Pos{x, y}, State::S);
      }
    }
    board.change_state(pos, state);

    board = evolve(board);
    CHECK(infected_neig(board, pos) == 28);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("TESTING ===> evolve(Condition)") {

  ///////////////////////////////////////////////////////////////////////////////////////////////
  B = 0.4;
  G = 0.25;
  N = 150 * 150;
  float float_N = static_cast<float>(N);
  float I0 = 2.f / float_N;
  auto S0 = 1.f - I0;

  Condition condition{S0, I0, 0.f, N, B, G};
  for (int i = 0; i != 200; ++i) {
    condition = evolve(condition);
    CHECK((condition.S + condition.I + condition.R) - 1.f ==
          doctest::Approx(0.f));
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  B = 0.9;
  G = 0.8;
  N = 783;
  float_N = static_cast<float>(N);
  I0 = 2.f / float_N;
  S0 = 1.f - I0;

  condition = Condition{S0, I0, 0.f, N, B, G};
  for (int i = 0; i != 200; ++i) {
    condition = evolve(condition);
    CHECK((condition.S + condition.I + condition.R) - 1.f ==
          doctest::Approx(0.f));
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  B = 0.f;
  G = 0.f;
  N = 1000;
  I0 = 2.f / float_N;
  S0 = 1.f - I0;

  condition = Condition{S0, I0, 0.f, N, B, G};
  for (int i = 0; i != 200; ++i) {
    condition = evolve(condition);
    CHECK((condition.S + condition.I + condition.R) - 1.f ==
          doctest::Approx(0.f));
  }
}
