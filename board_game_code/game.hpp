#ifndef CLASS_BOARD
#define CLASS_BOARD

#include "setup.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL DECLARATION of DEFAULT PARAMETERS /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

int n = 150;
float B = 0.40;    
float G = 0.25;    
float f = 0.80;    
float range = 1.f;
int I0 = 2;
Block_parameters block_parameters;
float N = static_cast<float>(n * n);
float quarantine_f = 0.1;
std::string simulation_type;

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

template <class T_>
auto print_and_check(T_ foo, std::string to_print, T_ bottom, T_ top) {
  std::cout << "\n" << to_print;
  std::cin >> foo;
  while (foo < bottom || foo > top) {
    std::cout << "\n" << to_print;
    std::cin >> foo;
  }
  assert(foo < bottom || foo > top);
  return foo;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

auto Type_choise(std::string &simulation_type_ref) {
  if (simulation_type_ref == "r") {
    block_parameters = Block_parameters{7, 3, 6, 7, 0.1, 0.4, 0.5, 0.8};
  } else if (simulation_type_ref == "u") {
    block_parameters = Block_parameters{7, 2, 5, 7, 0.1, 0.1, 0.7, 0.8};
  } else if (simulation_type_ref == "n") {
    block_parameters = Block_parameters{}; // block percentage is 0
  } else if (simulation_type_ref == "p") {
    B = print_and_check<float>(
        B,
        std::string{"Please, insert the probability of infection of your "
                    "virus (from 0 to 1): \n"},
        0, 1);
    G = print_and_check<float>(
        B,
        std::string{"Please, insert the probability of healing from your "
                    "virus (from 0 to 1): \n"},
        0, 1);
    f = print_and_check<float>(
        B,
        std::string{"Please, insert the fraction of moving people in your "
                    "model (from 0 to 1): \n"},
        0, 1);
    range = print_and_check<float>(
        B,
        std::string{"Please, insert the range of infection of your virus "
                    "(from 0 to (n - 1) / 2): \n"},
        0, (n - 1) / 2);
    I0 = print_and_check<int>(
        B,
        std::string{"Please, insert the number of initial infected in your "
                    "model (integer from 0 to n * n * 0.1): \n"},
        0, static_cast<int>(n * n * 0.1));

    int block_percentage = 0;
    block_percentage = print_and_check<int>(
        block_percentage,
        std::string{"Please, insert the percentage of blocks in your model "
                    "(integer from 0 to 9): \n"},
        0, 9);

    block_parameters =
        Block_parameters{block_percentage, 2, 5, 7, 0.8, 0.8, 0.7, 0.8};
  } else if (simulation_type_ref == "pa") {
    n = print_and_check<int>(
        n,
        std::string{"Please, insert the dimension (the length) of your board"
                    " (from 1 to 200): \n"},
        1, 200);
    B = print_and_check<float>(
        B,
        std::string{"Please, insert the probability of infection of your "
                    "virus (from 0 to 1): \n"},
        0.f, 1.f);
    G = print_and_check<float>(
        B,
        std::string{"Please, insert the probability of healing from your "
                    "virus (from 0 to 1): \n"},
        0.f, 1.f);
    f = print_and_check<float>(
        B,
        std::string{"Please, insert the fraction of moving people in your "
                    "model (from 0 to 1): \n"},
        0.f, 1.f);
    range = print_and_check<float>(
        B,
        std::string{"Please, insert the range of infection of your virus "
                    "(from 0 to (n - 1) / 2): \n"},
        0.f, static_cast<float>((n - 1) / 2));
    I0 = print_and_check<int>(
        B,
        std::string{"Please, insert the number of initial infected in your "
                    "model (integer from 0 to n * n * 0.1): \n"},
        0, static_cast<int>(n * n * 0.1));

    std::array<int, 4> block_parameters_1{};

    block_parameters_1[0] = print_and_check<int>(
        block_parameters_1[0],
        std::string{"Please, insert the percentage of blocks in your model "
                    "(integer from 0 to 9): \n"},
        0, 9);
    block_parameters_1[1] = print_and_check<int>(
        block_parameters_1[1],
        std::string{"Please, insert the first number to define the interval "
                    "for the generation of blocks in your model (integer "
                    "from 0 to 8): \nparameter_1 = "},
        0, 8);
    block_parameters_1[2] = print_and_check<int>(
        block_parameters_1[2],
        std::string{"Please, insert the second number to define the interval "
                    "for the generation of blocks in your model (integer "
                    "from parameter_1 to 8): \nparameter_2 = "},
        block_parameters_1[1], 8);
    block_parameters_1[3] = print_and_check<int>(
        block_parameters_1[3],
        std::string{"Please, insert the third number to define the interval "
                    "for the generation of blocks in your model (integer "
                    "from parameter_2 to 8): \nparameter_3 = "},
        block_parameters_1[2], 8);

    std::array<float, 4> block_parameters_2{};

    block_parameters_1[0] = print_and_check<float>(
        block_parameters_1[0],
        std::string{"Please, insert the probability of generating a block "
                    "between 0 and parameter_1 (decimal from 0.01 to 1): \n"},
        0.01, 1.f);
    block_parameters_1[1] = print_and_check<float>(
        block_parameters_1[1],
        std::string{
            "Please, insert the probability of generating a block between "
            "parameter_1 and parameter_2 (decimal from 0.01 to 1): \n"},
        0.01, 1.f);
    block_parameters_1[2] = print_and_check<float>(
        block_parameters_1[2],
        std::string{
            "Please, insert the probability of generating a block between "
            "parameter_2 and parameter_3 (decimal from 0.01 to 1): \n"},
        0.01, 1.f);
    block_parameters_1[3] = print_and_check<float>(
        block_parameters_1[3],
        std::string{"Please, insert the probability of generating a block "
                    "between parameter_3 and 8 (decimal from 0.01 to 1): \n"},
        0.01, 1.f);

    block_parameters = Block_parameters{
        block_parameters_1[0], block_parameters_1[1], block_parameters_1[2],
        block_parameters_1[3], block_parameters_2[0], block_parameters_2[1],
        block_parameters_2[2], block_parameters_2[3]};
  } else {
    while (!(simulation_type == "r" || simulation_type == "u" ||
             simulation_type == "n" || simulation_type == "p" ||
             simulation_type == "pa")) {
      std::cout
          << simulation_type
          << "\nPlease press one the following keys r, u, n, p, pa . . . \n";
      std::cin >> simulation_type;
    }
  }
  assert(simulation_type == "r" || simulation_type == "u" ||
         simulation_type == "n" || simulation_type == "p" ||
         simulation_type == "pa");
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

auto welcome() {
  std::cout
      << "Welcome in The Board Game! \nIn order to continue please select one "
         "of the following options: \n --> r <--   r for realistic, our best "
         "model! \n --> u <--   u for uncivilised, "
         "fewer blocks and lower moving fraction. \n --> n <--   n for "
         "nonrealistic, zero or close to zero blocks. \n --> p <--   p for "
         "personalised, choose your own parameters!\n --> pa <--  pa for "
         "personalised advanced, play with how the blocks are distributed! (we "
         "strongly recommend to read instructions before choosing this "
         "option).\nPlease "
         "press one the following keys r, u, n, p, pa... \n";

  std::cin >> simulation_type;
  Type_choise(simulation_type);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

int infected_neig(const Board &board, const Pos &pos) {
  int count = 0;

  auto check = [&](auto const &move) {
    if (!(move == Pos{0, 0}) && (pos + move) > Pos{-1, -1} &&
        (pos + move) < Pos{n, n} && (board.state(pos + move) == State::I)) {
      ++count;
    }
    return;
  };

  // count neighbours in a radius of (range) ////////////////////////////////////////////////////
  for (float dx = -1.f * range; dx < range + 0.001; ++dx) {
    for (float dy = -1.f * range; dy < range + 0.001; ++dy) {
      if (sqrt(dx * dx + dy * dy) - range < 0.001) {
        check(Pos{static_cast<int>(dx), static_cast<int>(dy)});
      }
    }
  }

  return count;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

auto neig(const Pos &pos, const State &state, const Board &board) {
  int neig = 0;
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      Pos move{dx, dy};
      if (!(move == Pos{0, 0}) && (pos + move) > Pos{-1, -1} &&
          (pos + move) < Pos{n, n} && board.state(pos + move) == state) {
        ++neig;
      }
    }
  }
  return neig;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

auto world_generation(const Board &board0,
                      const Block_parameters &block_parameters) {

  // GENERATE BLOCKS ////////////////////////////////////////////////////////////////////////////
  Board board = board0;

  std::uniform_int_distribution<> dist(0, n - 1);

  const float block_percentage = block_parameters.int_block_percentage / 10.f;
  const int total_block = static_cast<int>(n * n * block_percentage);

  const float block_probability = 0.11;

  const int precision = 1000;
  std::uniform_int_distribution<> chance(1, precision);

  int counter_block = 0;

  int interval1 = block_parameters.interval1;
  int interval2 = block_parameters.interval2;
  int interval3 = block_parameters.interval3;
  float factor0 = block_parameters.factor0;
  float factor1 = block_parameters.factor1;
  float factor2 = block_parameters.factor2;
  float factor3 = block_parameters.factor3;

  while (counter_block != total_block) {
    Pos pos = Pos{dist(gen), dist(gen)};
    int count_neig = neig(pos, State::B, board);

    if (count_neig == 0) {
      if (board.state(pos) == State::T &&
          (static_cast<float>(chance(gen)) <
           static_cast<float>(precision) * block_probability * factor0)) {
        board.change_state(pos, State::B);
        ++counter_block;
      }
    } else if (count_neig > 0 && count_neig <= interval1) {
      if (board.state(pos) == State::T &&
          (static_cast<float>(chance(gen)) <
           static_cast<float>(precision * interval1) * factor1 *
               block_probability)) {
        board.change_state(pos, State::B);
        ++counter_block;
      }
    } else if (count_neig > interval1 && count_neig <= interval2) {
      if (board.state(pos) == State::T &&
          (static_cast<float>(chance(gen)) <
           static_cast<float>(precision * interval2) * factor2 *
               block_probability)) {
        board.change_state(pos, State::B);
        ++counter_block;
      }
    } else if (count_neig > interval2 && count_neig <= interval3) {
      if (board.state(pos) == State::T &&
          (static_cast<float>(chance(gen)) <
           static_cast<float>(precision * interval3) * factor3 *
               block_probability)) {
        board.change_state(pos, State::B);
        ++counter_block;
      }
    } else {
      if (board.state(pos) == State::T) {
        board.change_state(pos, State::B);
        ++counter_block;
      }
    }
  }

  for (int y = 0; y != n; ++y) {
    for (int x = 0; x != n; ++x) {
      Pos pos = Pos{x, y};
      if (board.state(pos) == State::T) {
        board.change_state(pos, State::S);
      } else if (!(board.state(pos) == State::B)) {
        throw std::runtime_error{
            "error... why are there cells that are not Temporary nor Blocks?!"};
      }
    }
  }

  // GENERATE INITIAL INFECTED //////////////////////////////////////////////////////////////////
  board.generate_initial_infected();

  // CHANGE BOARD CONDITIONS ////////////////////////////////////////////////////////////////////
  auto condition = board0.get_board_condition_();

  N -= static_cast<float>(counter_block);
  condition.N = N;
  condition.S = (N - I0) / N;
  condition.I = I0 / N;

  board.change_board_condition_(condition);
  
  // return /////////////////////////////////////////////////////////////////////////////////////
  return board;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

Board evolve(const Board &board0) {
  Board board = board0;
  n = board.get_n_();
  f = board.get_f_();
  B = board.get_B_();
  G = board.get_G_();
  float S_count = 0.f;
  float I_count = 0.f;
  float R_count = 0.f;
  float T_count = 0.f;
  float B_count = 0.f;

  // INFECTION and RECOVER RULES ////////////////////////////////////////////////////////////////
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
      case (State::B):
        ++B_count;
        break;
      }
    }
  }

  if (T_count != 0) {
    throw std::runtime_error{"error... and a big one I'd say"};
  }
  
  // RANDOM MOVEMENT ////////////////////////////////////////////////////////////////////////////
  const auto movers = static_cast<int>(f * N);
  std::uniform_int_distribution<> uniform_dist2(0, n - 1);
  std::uniform_int_distribution<> uniform_dist3(0, movers - 1);
  std::vector<Pos> mover;

  int counter = 0;
  while (counter != movers) {
    int x = uniform_dist2(gen);
    int y = uniform_dist2(gen);
    Pos pos = Pos{x, y};

    if (board.state(pos) != State::B) {
      mover.push_back(pos);
      ++counter;
    }
  }

  for (int i = 0; i != movers; ++i) {
    int j = uniform_dist3(gen);
    board.swap(mover[i], mover[j]);
    std::swap(mover[i], mover[j]);
  }
  
  // board_condition EVOLUTION //////////////////////////////////////////////////////////////////
  auto condition = board0.get_board_condition_();
  condition.S = S_count / N;
  condition.I = I_count / N;
  condition.R = R_count / N;
  board.change_board_condition_(condition);
  
  // return /////////////////////////////////////////////////////////////////////////////////////
  return board;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

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

  // APPLY SIR RULES ////////////////////////////////////////////////////////////////////////////
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

  // the algorithm has a percentage error of 0.1% ish
  if (fabs(S + I + R - 1.f) > 1.f / 1000.f) {
    throw std::runtime_error{"error... we want our results to be precise!!!"};
  }

  return condition;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void main_execution(Board &board, Condition &condition) {
  
  // WINDOW SIZES ///////////////////////////////////////////////////////////////////////////////
  const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

  const int board_window_dim_max = desktop.height;
  const int cell_dim = board_window_dim_max / (n + 4);
  const float float_cell_dim = static_cast<float>(cell_dim);
  const float thickness = 0.001 * n * float_cell_dim;
  const int margin = 2 * cell_dim;
  const int window_dim = cell_dim * n + 2 * margin;

  const int graph_width = (desktop.width - window_dim) / 2;
  const int graph_height = (desktop.height) * 3 / 10; // 30% of desktop's heigth
  const float float_graph_width = static_cast<float>(graph_width);
  const float float_graph_height = static_cast<float>(graph_height);

  // WINDOWS RENDER /////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////
  sf::RenderWindow board_window(sf::VideoMode(window_dim, window_dim), "Board",
                                sf::Style::Default);
  board_window.setVerticalSyncEnabled(true); // refresh window same as monitor
  board_window.setPosition(sf::Vector2i(desktop.width - window_dim, 0));

  ///////////////////////////////////////////////////////////////////////////////////////////////
  sf::RenderWindow board_graph_window(sf::VideoMode(graph_width, graph_height),
                                      "Board Graph", sf::Style::Default);
  board_graph_window.setVerticalSyncEnabled(true);
  board_graph_window.setPosition(
      sf::Vector2i(graph_width, window_dim - graph_height));
  std::vector<sf::Vertex> board_S_vertices;
  std::vector<sf::Vertex> board_I_vertices;
  std::vector<sf::Vertex> board_R_vertices;

  ///////////////////////////////////////////////////////////////////////////////////////////////
  sf::RenderWindow SIR_graph_window(sf::VideoMode(graph_width, graph_height),
                                    "SIR Graph", sf::Style::Default);
  SIR_graph_window.setVerticalSyncEnabled(true);
  SIR_graph_window.setPosition(sf::Vector2i(0, window_dim - graph_height));
  std::vector<sf::Vertex> SIR_S_vertices;
  std::vector<sf::Vertex> SIR_I_vertices;
  std::vector<sf::Vertex> SIR_R_vertices;
  
  // DRAW and EVOLVE ////////////////////////////////////////////////////////////////////////////
  int delay = 400; // in milliseconds
  unsigned int iteration_time = 0;
  float time = 0.f;

  auto info = [&](auto board, auto added) mutable {
    auto i_f = board.get_f_();
    auto i_S = board.get_board_condition_().S * N;
    auto i_I = board.get_board_condition_().I * N;
    auto i_R = board.get_board_condition_().R * N;
    std::cout
        << "Board: Susceptibles --> Orange // Infectious --> Red // Recovered "
           "--> Yellow\nTheory: Susceptibles --> Green // Infectious --> Red "
           "// Recovered --> Blue\n\nTry and press . . .\n--> Q <--   Activate "
           "Quarantine!"
           "\n--> L <--   Minimize delay!"

        << "\n\nB = " << B << "\nG = " << G << "\nrange = " << range
        << "\nf = " << i_f << "\nBlock percentage = "
        << static_cast<float>(block_parameters.int_block_percentage) / 10.f
        << "\ndelay (ms) = " << delay
        << "\niteration time (ms) = " << iteration_time << "\ntime = " << time
        << "\nN = " << N << "\nS = " << i_S << "\nI = " << i_I
        << "\nR = " << i_R << added << "\n\033c";
  };

  std::string added;

  while (board_window.isOpen() && board_graph_window.isOpen() &&
         SIR_graph_window.isOpen()) {

    // time managment ///////////////////////////////////////////////////////////////////////////
    auto start_time = std::chrono::high_resolution_clock::now();
    info(board, added);
    ++time;

    // events ///////////////////////////////////////////////////////////////////////////////////
    sf::Event event;

    while (board_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        board_window.close();
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        board.change_f_(quarantine_f);
        added = "\nQuarantine activated ! ! !";
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        if (delay > 0) {
          delay = 0;
        }
      }
    }

    while (board_graph_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        board_graph_window.close();
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        board.change_f_(quarantine_f);
        added = "\nQuarantine activated ! ! !";
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        if (delay > 0) {
          delay = 0;
        }
      }
    }

    while (SIR_graph_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        SIR_graph_window.close();
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        board.change_f_(quarantine_f);
        added = "\nQuarantine activated ! ! !";
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        if (delay > 0) {
          delay = 0;
        }
      }
    }
    
  // ACTIONS ////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////
    const float max_time = 150.f;
    float speed = 1.f;

    // board_graph_window ACTIONS ///////////////////////////////////////////////////////////////
    {
      const auto &board_condition = board.get_board_condition_();

      if (board_condition.I > 0.00085 && simulation_type == "r") {
        board.change_f_(quarantine_f);
        added = "\nQuarantine activated ! ! !";
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
                       sf::Color(255, 127, 80)));
        board_I_vertices.push_back(
            sf::Vertex(sf::Vector2f(time_axis(), y_axis(board_condition.I)),
                       sf::Color::Red));
        board_R_vertices.push_back(
            sf::Vertex(sf::Vector2f(time_axis(), y_axis(board_condition.R)),
                       sf::Color::Yellow));

        board_graph_window.draw(&board_S_vertices[0], board_S_vertices.size(),
                                sf::Lines);
        board_graph_window.draw(&board_I_vertices[0], board_I_vertices.size(),
                                sf::Lines);
        board_graph_window.draw(&board_R_vertices[0], board_R_vertices.size(),
                                sf::Lines);
        board_graph_window.display();
      }
    }
  
    // board_window ACTIONS /////////////////////////////////////////////////////////////////////
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
            rectangle.setOutlineColor(sf::Color(230, 230, 230));
            break;
          case (State::B):
            rectangle.setFillColor(sf::Color::Black);
            rectangle.setOutlineColor(sf::Color(230, 230, 230));
            break;
          }
          rectangle.setPosition(x * cell_dim + margin, y * cell_dim + margin);
          board_window.draw(rectangle);
        }
      }
      
      board_window.display();
      board = evolve(board);
    }

    // SIR_graph_window ACTIONS /////////////////////////////////////////////////////////////////
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
      }
    }
    
    // TIME ELAPSED and DELAY ///////////////////////////////////////////////////////////////////
    auto end_time = std::chrono::high_resolution_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
                     end_time - start_time)
                     .count();
    if (delay - delta > 0) {
      iteration_time = delta;
      std::this_thread::sleep_for(std::chrono::milliseconds(delay - delta));
    }
  }
};

#endif
