#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <sstream>

typedef char word;
typedef int address;

enum opcode {
  exit_opcode = 0x00,
  add_opcode = 0x01,
  push_opcode = 0x60
};

enum machine_state {
  stopped_machine_state,
  running_machine_state
};

class machine
{
  address pc;
  std::deque<word> stack;
  machine_state state;
  std::string code;

  public:
  machine(std::string c) : pc(0), code(c), state(running_machine_state) {}
  void step();
  bool is_running();
  std::string to_json();
};
