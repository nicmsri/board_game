/*

compiling procedures for SFML:
g++ -c file_name.cpp
g++ file_name.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
./sfml-app

*/

#include <SFML/Graphics.hpp>
#include <algorithm> //std::swap
#include <cassert>
#include <chrono> // std::chrono::milliseconds
#include <iostream>
#include <math.h> // pow(), fabs()
#include <random>
#include <string>
#include <thread> // std::this_thread::sleep_for
#include <time.h> // time()
#include <vector>

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////

struct Pos {
  int x; // column
  int y; // row
};

auto operator+(const Pos &l, const Pos &r) {
  return Pos{l.x + r.x, l.y + r.y};
};

auto operator==(const Pos &l, const Pos &r) {
  return l.x == r.x && l.y == r.y;
};

bool operator<(const Pos &l, const Pos &r) { return l.x < r.x && l.y < r.y; };

bool operator>(const Pos &l, const Pos &r) { return l.x > r.x && l.y > r.y; };

///////////////////////////////////////////////////
// susceptible, infected, recovered, temporary, block
///////////////////////////////////////////////////

enum class State { S, I, R, T, B };

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////

struct Condition {
  float S;
  float I;
  float R;
  float N;
  float B;
  float G;
};

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////

class Board {
  int n_;
  float f_;
  float range_;
  Condition board_condition_;
  int I0_;
  std::vector<State> grid_;

public:
  Board(const int &n, const float &f, const float &range,
        const Condition &board_condition, const int &I0)
      : n_{n}, f_{f}, range_{range},
        board_condition_{board_condition}, I0_{I0} {
    // board initialization ////////////////////////
    for (int y = 0; y != n; ++y) {
      for (int x = 0; x != n; ++x) {
        grid_.push_back(State::T);
      }
    }
    // generate_initial_infected();
  };

  auto state(const Pos &pos) const { return grid_[pos.x + n_ * pos.y]; };
  void change_state(const Pos &pos, const State &state) {
    auto &state_t = grid_[pos.x + n_ * pos.y];
    assert(state_t != State::B);
    state_t = state;
  };
  void change_board_condition_(const Condition &board_condition) {
    board_condition_ = board_condition;
  };

  void swap(const Pos &pos0, const Pos &pos1) {
    auto foo = state(pos0);
    change_state(pos0, state(pos1));
    change_state(pos1, foo);
  };

  void generate_initial_infected() {
    std::default_random_engine gen(std::time(0));
    std::uniform_int_distribution<> dist(0, n_ - 1);
    int counter = 0;
    while (counter != I0_) {
      Pos pos = Pos{dist(gen), dist(gen)};
      if (state(pos) == State::S) {
        ++counter;
        change_state(pos, State::I);
      }
    }
  };

  auto set_N_() {
    int counter = 0;
    for (int y = 0; y != n_; ++y) {
      for (int x = 0; x != n_; ++x) {
        if (state(Pos{x, y}) == State::B) {
          ++counter;
        }
      }
    }
    board_condition_.N -= counter;
  }

  auto change_f_(auto const f) { f_ = f; };

  auto get_n_() const { return n_; }
  auto get_N_() const { return board_condition_.N; }
  auto get_B_() const { return board_condition_.B; }
  auto get_G_() const { return board_condition_.G; }
  auto get_f_() const { return f_; }
  auto get_I0_() const { return I0_; }
  auto get_range_() const { return range_; }
  auto get_board_condition_() const { return board_condition_; }
  auto get_grid_() const { return grid_; }
};

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////

Condition evolve(Condition &condition0) {
  const auto S0 = condition0.S;
  const auto I0 = condition0.I;
  const auto R0 = condition0.R;
  const auto B = condition0.B;
  const auto G = condition0.G;

  Condition condition = condition0;
  auto &S = condition.S;
  auto &I = condition.I;
  auto &R = condition.R;

  // apply SIR evolution rules //////////////////////

  auto foo = S0 - B * I0 * S0;
  if (foo > 0) {
    S = foo;
  } else {
    S = 0.f;
  }

  foo = I0 + (B * I0 * S0 - G * I0);
  if (foo > 0) {
    I = foo;
  } else {
    I = 0.f;
  }

  foo = R0 + (G * I0);
  if (foo > 0) {
    R = foo;
  } else {
    R = 0.f;
  }

  // the algorithm has a percentage error of 0.1%
  if (fabs(S + I + R - 1.f) > 1.f / 1000.f) {
    std::cout << "error\n";
  }

  return condition;
}

//////////////////////////////////////////////////
// counts infected neighbours ////////////////////
//////////////////////////////////////////////////

int infected_neig(const Board &board, const Pos &pos) {
  int count = 0; // neighbours counter
  const auto n = board.get_n_();

  auto check = [&](auto const &move) {
    if (!(move == Pos{0, 0}) && (pos + move) > Pos{-1, -1} &&
        (pos + move) < Pos{n, n} && (board.state(pos + move) == State::I)) {
      ++count;
    };
    return;
  };

  // count neighbours in a radius of (range) /////
  const auto range = board.get_range_();
  for (int dx = -1 * range; dx <= range; ++dx) {
    for (int dy = -1 * range; dy <= range; ++dy) {
      if (static_cast<float>(sqrt(dx * dx + dy * dy)) - range < 0.001) {

        check(Pos{dx, dy});
      }
    }
  }

  return count;
}

struct Parameter {
  int interval1;
  int interval2;
  int interval3;
  float factor0;
  float factor1;
  float factor2;
  float factor3;
};

/////////////////////////////////////////////////
// worlds generation ////////////////////////////
/////////////////////////////////////////////////

auto world_generation(const Board &board0, const Parameter &parameter) {
  Board board = board0;
  const auto num = board.get_n_();

  std::default_random_engine gen(std::time(0));
  std::uniform_int_distribution<> dist(0, num - 1);

  const int maximum_earth = 4;
  const int minimum_earth = 3;
  std::uniform_int_distribution<> earth_percentage_dist(minimum_earth,
                                                        maximum_earth);
  const float earth_percentage =
      static_cast<float>(earth_percentage_dist(gen)) / 10.f;
  const float total_earth = static_cast<float>(num * num) * earth_percentage;

  const int total_ocean = num * num - static_cast<int>(total_earth);
  const float ocean_probability = 0.11;

  const int precision = 1000;
  std::uniform_int_distribution<> chance(1, precision);

  auto neig = [&](auto &pos, auto state_var) {
    int neig = 0;
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        Pos move = Pos{dx, dy};
        if (!(move == Pos{0, 0}) && (pos + move) > Pos{-1, -1} &&
            (pos + move) < Pos{num, num} &&
            board.state(pos + move) == state_var) {
          ++neig;
        }
      }
    }
    return neig;
  };

  int counter_ocean = 0;

  int interval1 = parameter.interval1;
  int interval2 = parameter.interval2;
  int interval3 = parameter.interval3;
  float factor0 = parameter.factor0;
  float factor1 = parameter.factor1;
  float factor2 = parameter.factor2;
  float factor3 = parameter.factor3;

  while (counter_ocean != total_ocean) {
    Pos pos = Pos{dist(gen), dist(gen)};
    int count_neig = neig(pos, State::B);

    if (count_neig == 0) {
      // float f0 = static_cast<float>( exp( -1 *
      // pow(counter_ocean, 1.f/static_cast<float>(counter_ocean)) ) / (exp(1))
      // );
      if (board.state(pos) == State::T &&
          (static_cast<float>(chance(gen)) <
           static_cast<float>(precision) * ocean_probability * factor0)) {
        board.change_state(pos, State::B);
        ++counter_ocean;
      }
    } else if (count_neig > 0 && count_neig <= interval1) {
      if (board.state(pos) == State::T &&
          (static_cast<float>(chance(gen)) <
           static_cast<float>(precision * interval1) * factor1 *
               ocean_probability)) {
        board.change_state(pos, State::B);
        ++counter_ocean;
      }
    } else if (count_neig > interval1 && count_neig <= interval2) {
      if (board.state(pos) == State::T &&
          (static_cast<float>(chance(gen)) <
           static_cast<float>(precision * interval2) * factor2 *
               ocean_probability)) {
        board.change_state(pos, State::B);
        ++counter_ocean;
      }
    } else if (count_neig > interval2 && count_neig <= interval3) {
      if (board.state(pos) == State::T &&
          (static_cast<float>(chance(gen)) <
           static_cast<float>(precision * interval3) * factor3 *
               ocean_probability)) {
        board.change_state(pos, State::B);
        ++counter_ocean;
      }
    } else {
      if (board.state(pos) == State::T) {
        board.change_state(pos, State::B);
        ++counter_ocean;
      }
    }
  }

  for (int y = 0; y != num; ++y) {
    for (int x = 0; x != num; ++x) {
      Pos pos = Pos{x, y};
      if (board.state(pos) == State::T) {
        board.change_state(pos, State::S);
      }
    }
  }

  /////////////////////////////////////////////////////////

  board.generate_initial_infected();

  auto condition = board0.get_board_condition_();
  const float N = condition.N - static_cast<float>(counter_ocean);
  const auto I0 = static_cast<float>(board.get_I0_());
  condition.N = N;
  condition.S = (N - I0) / N;
  condition.I = I0 / N;
  board.change_board_condition_(condition);

  return board;
};

///////////////////////////////////////////////////
/*
Important Note:
the evolve function applies the changes of the infection
in a new Board (board) but does the checks for the changes
with the old Board (board0) so that when a change has been made
it will only be effective after every other cells change
has been calculated.
*/
///////////////////////////////////////////////////

Board evolve(const Board &board0) {
  const auto n = board0.get_n_();
  auto N = board0.get_N_();
  const auto B = board0.get_B_();
  const auto G = board0.get_G_();
  const auto f = board0.get_f_();
  Board board = board0;
  std::default_random_engine gen(std::time(0)); // creates a generator

  float S_count = 0.f;
  float I_count = 0.f;
  float R_count = 0.f;
  float T_count = 0.f;
  float B_count = 0.f;

  // infection and recover rules /////////////////
  constexpr float precision = 1000.f;
  std::uniform_int_distribution<> uniform_dist(1, precision);

  for (int x = 0; x != n; x++) {
    for (int y = 0; y != n; y++) {
      Pos pos{x, y};
      switch (board0.state(pos)) {
      case (State::S):
        ++S_count;
        for (int i = 0; i != infected_neig(board0, pos); i++) {
          if (uniform_dist(gen) < B * precision) {
            board.change_state(pos, State::I);
          }
        }
        break;
      case (State::I):
        ++I_count;
        if (static_cast<float>(uniform_dist(gen)) < G * precision) {
          board.change_state(pos, State::R);
        }
        break;
      case (State::R):
        ++R_count;
        break;
      case (State::T):
        ++T_count;
        break;
        ++B_count;
      case (State::B):
        break;
      }
    }
  }

  // random movement ///////////////////////////
  const auto movers = static_cast<int>(f * N);
  std::uniform_int_distribution<> uniform_dist2(0, n - 1);
  std::uniform_int_distribution<> uniform_dist3(0, movers - 1);
  std::vector<Pos> mover;

  int counter = 0;
  while (counter != movers) {
    int x = uniform_dist2(gen);
    int y = uniform_dist2(gen);
    Pos pos = Pos{x, y};
    if (board.state(pos) != State::B && board.state(pos) != State::T) {
      mover.push_back(pos);
      ++counter;
    }
  }

  for (int i = 0; i != movers; ++i) {
    int j = uniform_dist3(gen);
    board.swap(mover[i], mover[j]);
    std::swap(mover[i], mover[j]);
  }

  // board_condition evolution ///////////////////
  auto condition = board0.get_board_condition_();
  condition.S = S_count / N;
  condition.I = I_count / N;
  condition.R = R_count / N;
  board.change_board_condition_(condition);

  // return ///////////////////////////////////////
  return board;
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////

void print(const Board &board) {
  const auto grid = board.get_grid_();
  const int n = board.get_n_();
  char S_char = ' ';
  char I_char = 'X'; // 64 = @,48 = 0
  char R_char = '0';
  char T_char = '-';
  char B_char = '|';

  for (int y = 0; y != n; ++y) {
    for (int x = 0; x != n; ++x) {
      switch (board.state(Pos{x, y})) {
      case (State::S):
        std::cout << S_char;
        break;
      case (State::I):
        std::cout << I_char;
        break;
      case (State::R):
        std::cout << R_char;
        break;
      case (State::T):
        std::cout << T_char;
        break;
      case (State::B):
        std::cout << B_char;
        break;
      }
      if ((x + 1) % n == 0) {
        std::cout << '\n';
      }
    }
  }
  std::cout << "\n";
}

void reset_environment(const Board &board) {
  const auto n = board.get_n_();

  std::cout << "\033c";
  for (int i = 0; i != n; i++) {
    std::cout << '_';
  };
  std::cout << '\n';
}

void wait(const int &delay, const auto &delta) {
  if (delay - delta > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay - delta));
  }
  // std::cin.get();
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////

int main() {
  const int n = 150;
  float B;     // beta, probability to get infected
  float G;     // gamma, probability to recover from infection
  float f;     // fraction of people that moves
  float range; // infection's range
  int I0;
  Parameter parameter;
  float N;
  float quarantine_f = 0.1;
  
  auto init = [&](std::string &c) mutable {
    if (c == "r") {
      B = 0.40;
      G = 0.25;
      f = 0.80;
      range = 1.f; 
      I0 = 2;
      parameter = Parameter{3, 6, 7, 1.f, 1.f, 1.f, 1.f};
      N = static_cast<float>(n * n);
    } else if (c == "u") {
      B = 0.60;    
      G = 0.25;    
      f = 0.20;    
      range = 2.f; 
      I0 = 3;
      parameter = Parameter{3, 6, 7, 1.f, 1.f, 1.f, 1.f};
      N = static_cast<float>(n * n);
    } else if (c == "o") {
      B = 0.45;    
      G = 0.35;    
      f = 0.70;    
      range = 1.f; 
      I0 = 2;
      parameter = Parameter{2, 5, 7, 0.01, 0.1, 1.f, 1.f};
      N = static_cast<float>(n * n);
    }
  };

  std::cout
      << "Welcome in The Board Game! \nIn order to continue please select one "
         "of the following options: \n--> r <--   r for realistic, our best "
         "model! \n --> u <--   u for uncivilised, high infection rate, low "
         "healing rate, low moving fraction. \n--> c <--   c for civilised, low "
         "infecion rate, high healing rate, high moving fraction. \n Please "
         "press one the following keys r, u, c... \n";
  std::string c;
  std::cin >> c;
  while (!(c == "r" || c == "u" || c == "c")) {
    std::cout << c << "\nPlease press one the following keys r, u, c . . . \n";
    std::cin >> c;
  }
  init(c);

  Condition condition{0.f, 0.f, 0.f, N, B, G};
  Board board(n, f, range, condition, I0);
  board = world_generation(board, parameter);
  condition = board.get_board_condition_();

  // output //////////////////////////////////////

  // board_window dimension, cell dimension maximization ///
  const int board_window_dim_max = 768;
  const int cell_dim = board_window_dim_max / (n + 4);
  const float float_cell_dim = static_cast<float>(cell_dim);
  const float thickness = 0.001 * n * float_cell_dim;
  const int margin = 2 * cell_dim;
  const int window_dim = cell_dim * n + 2 * margin;

  // graph dimension /////////////////////////////

  const int graph_width = 400;
  const int graph_height = 300;
  const float float_graph_width = static_cast<float>(graph_width);
  const float float_graph_height = static_cast<float>(graph_height);

  // windows render //////////////////////////////////////

  ////////////////////////////////////////////////////////////////
  sf::RenderWindow board_window(sf::VideoMode(window_dim, window_dim),
                                "Board evolution", sf::Style::Default);
  board_window.setVerticalSyncEnabled(true); // refresh window same as monitor
  board_window.setPosition(sf::Vector2i(0, 0));

  /////////////////////////////////////////////////////////////////
  sf::RenderWindow board_graph_window(sf::VideoMode(graph_width, graph_height),
                                      "Board Graph", sf::Style::Default);
  board_graph_window.setVerticalSyncEnabled(true);
  board_graph_window.setPosition(sf::Vector2i(700, 0));
  std::vector<sf::Vertex> board_S_vertices;
  std::vector<sf::Vertex> board_I_vertices;
  std::vector<sf::Vertex> board_R_vertices;

  ///////////////////////////////////////////////////////////////
  sf::RenderWindow SIR_graph_window(sf::VideoMode(graph_width, graph_height),
                                    "SIR Graph", sf::Style::Default);
  SIR_graph_window.setVerticalSyncEnabled(true);
  SIR_graph_window.setPosition(sf::Vector2i(700, 400));
  std::vector<sf::Vertex> SIR_S_vertices;
  std::vector<sf::Vertex> SIR_I_vertices;
  std::vector<sf::Vertex> SIR_R_vertices;

  // draw and evolve ////////////////////////////////////////////////

  int delay = 200; // in milliseconds
  float time = 0.f;
  
  auto info = [=](auto i_B, auto i_G, auto i_range, auto i_f, auto i_delay,
                 auto i_time, auto added) {
    std::cout << "Try and press . . .\n--> Q <--   Activate Quarantine! \n--> M <--   Increase Delay of 200ms up to 1000ms! \n--> L <--   Decrease Delay of 200ms!"
    
    << "\nB = " << i_B << "\nG = " << i_G << "\nrange = " << i_range
              << "\nf = " << i_f << "\ndelay = " << i_delay
              << "\ntime = " << i_time << added << "\n\033c";
  };
	
	std::string added;
	int Qcount = 0;
	
  while (board_window.isOpen() && board_graph_window.isOpen() &&
         SIR_graph_window.isOpen()) {
    // time managment /////////////////////////////////////
    auto start_time = std::chrono::high_resolution_clock::now();
    info(board.get_B_(), board.get_G_(), board.get_range_(), board.get_f_(), delay, time, added);
    ++time;

    // events //////////////////////////////////////////////
    sf::Event event;

    while (board_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        board_window.close();
      }
      if (Qcount % 2 == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        board.change_f_(quarantine_f);
    		added = "\nQuarantine activated ! ! !";
    		++Qcount;
      } else if (Qcount % 2 == 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        board.change_f_(f);
    		added = "\nQuarantine deactivated ! ! !";
    		++Qcount;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
        if (delay <= 1000) {
          delay += delay;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        if (delay > 0) {
          delay -= delay;
        }
      }
    }

    while (board_graph_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        board_graph_window.close();
      }
      if (Qcount % 2 == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        board.change_f_(quarantine_f);
    		added = "\nQuarantine activated ! ! !";
    		++Qcount;
      } else if (Qcount % 2 == 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        board.change_f_(f);
    		added = "\nQuarantine deactivated ! ! !";
    		++Qcount;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
        if (delay <= 1000) {
          delay += delay;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        if (delay > 0) {
          delay -= delay;
        }
      }
    }

    while (SIR_graph_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        SIR_graph_window.close();
      }
      if (Qcount % 2 == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        board.change_f_(quarantine_f);
    		added = "\nQuarantine activated ! ! !";
    		++Qcount;
      } else if (Qcount % 2 == 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        board.change_f_(f);
    		added = "\nQuarantine deactivated ! ! !";
    		++Qcount;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
        if (delay <= 1000) {
          delay += delay;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        if (delay > 0) {
          delay -= delay;
        }
      }
    }

    const float max_time = 400.f;
    float speed = 4.f;

    // board_graph_window actions ////////////////////////////
    {
      const auto &board_condition = board.get_board_condition_();
      
    	if (Qcount % 2 == 0 && board_condition.I > 0.001 && c == "r") {
        board.change_f_(quarantine_f);
    		added = "\nQuarantine activated ! ! !";
    		++Qcount;
    	}
    	
      auto time_axis = [=]() {
        float result = (time * float_graph_width * speed) / max_time;
        return result;
      };
      auto y_axis = [=](auto y) {
        float result = float_graph_height - y * float_graph_height;
        return result;
      };
      if (time * speed - max_time < 0.1) {
        board_graph_window.clear();

        board_S_vertices.push_back(
            sf::Vertex(sf::Vector2f(time_axis(), y_axis(board_condition.S)),
                       sf::Color::Green));
        board_I_vertices.push_back(
            sf::Vertex(sf::Vector2f(time_axis(), y_axis(board_condition.I)),
                       sf::Color::Red));
        board_R_vertices.push_back(
            sf::Vertex(sf::Vector2f(time_axis(), y_axis(board_condition.R)),
                       sf::Color::Blue));

        board_graph_window.draw(&board_S_vertices[0], board_S_vertices.size(),
                                sf::Lines);
        board_graph_window.draw(&board_I_vertices[0], board_I_vertices.size(),
                                sf::Lines);
        board_graph_window.draw(&board_R_vertices[0], board_R_vertices.size(),
                                sf::Lines);
        board_graph_window.display();
      } /*else {
                      int foo = static_cast<int>(time * speed - max_time);
                      board_graph_window.draw(&board_S_vertices[foo],
           board_S_vertices.size(), sf::Lines);
                      board_graph_window.draw(&board_I_vertices[foo],
           board_I_vertices.size(), sf::Lines);
                      board_graph_window.draw(&board_R_vertices[foo],
           board_R_vertices.size(), sf::Lines);
              }*/
    }

    // board_window actions /////////////////////////////////////////
    {
      board_window.clear();

      for (int y = 0; y != n; y++) {
        for (int x = 0; x != n; x++) {
          sf::RectangleShape rectangle(sf::Vector2f(cell_dim, cell_dim));
          rectangle.setOutlineThickness(thickness);
          switch (board.state(Pos{x, y})) {
          case (State::S):
            rectangle.setFillColor(sf::Color(255, 127, 80));
            rectangle.setOutlineColor(sf::Color(230, 230, 230));
            break;
          case (State::I):
            rectangle.setFillColor(sf::Color::Red);
            rectangle.setOutlineColor(sf::Color(230, 230, 230));
            break;
          case (State::R):
            rectangle.setFillColor(sf::Color::Yellow);
            rectangle.setOutlineColor(sf::Color(230, 230, 230));
            break;
          case (State::T):
            rectangle.setFillColor(sf::Color::Blue);
            rectangle.setOutlineColor(sf::Color::Blue);
            break;
          case (State::B):
            rectangle.setFillColor(sf::Color::Black);
            rectangle.setOutlineColor(sf::Color::Black);
            break;
          }
          rectangle.setPosition(x * cell_dim + margin, y * cell_dim + margin);
          board_window.draw(rectangle);
        }
      }
      board_window.display();

      // reset_environment(board);
      // print(board);

      board = evolve(board);
    }

    // SIR_graph_window actions /////////////////////////////////////////
    {
      SIR_graph_window.clear();

      auto time_axis = [=](auto float_i) {
        float result = (time * speed + float_i) * float_graph_width / max_time;
        return result;
      };
      auto y_axis = [=](auto y) {
        float result = float_graph_height - y * float_graph_height;
        return result;
      };
      if (time * speed - max_time < 0.1) {
        for (int i = 0; i < speed + 0.1; ++i) {
          float float_i = static_cast<float>(i);
          SIR_S_vertices.push_back(
              sf::Vertex(sf::Vector2f(time_axis(float_i), y_axis(condition.S)),
                         sf::Color::Green));
          SIR_I_vertices.push_back(
              sf::Vertex(sf::Vector2f(time_axis(float_i), y_axis(condition.I)),
                         sf::Color::Red));
          SIR_R_vertices.push_back(
              sf::Vertex(sf::Vector2f(time_axis(float_i), y_axis(condition.R)),
                         sf::Color::Blue));
          condition = evolve(condition);
        }

        SIR_graph_window.draw(&SIR_S_vertices[0], SIR_S_vertices.size(),
                              sf::Lines);
        SIR_graph_window.draw(&SIR_I_vertices[0], SIR_I_vertices.size(),
                              sf::Lines);
        SIR_graph_window.draw(&SIR_R_vertices[0], SIR_R_vertices.size(),
                              sf::Lines);
        SIR_graph_window.display();
      } /* else {
                      int foo = static_cast<int>(time * speed - max_time);
                      int size = SIR_S_vertices.size();
                      for (int i = 0; i != foo; ++i) {
                                                      std::swap(SIR_S_vertices[size
      - i], SIR_S_vertices[i]); std::swap(SIR_I_vertices[size - i],
      SIR_I_vertices[i]); std::swap(SIR_R_vertices[size - i],
      SIR_R_vertices[i]);
                      }
                      SIR_graph_window.draw(&SIR_S_vertices[0],
      SIR_S_vertices.size() - foo, sf::Lines);
              SIR_graph_window.draw(&SIR_I_vertices[0], SIR_I_vertices.size() -
      foo, sf::Lines); SIR_graph_window.draw(&SIR_R_vertices[0],
      SIR_R_vertices.size() - foo, sf::Lines);
      }*/
    }

    // time elapsed and delay ////////////////////////////////

    auto end_time = std::chrono::high_resolution_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
                     start_time - end_time)
                     .count();
    wait(delay, delta);
  }
}
