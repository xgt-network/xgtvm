#include <cstdint>
#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <sstream>

typedef uint64_t word;
typedef int address;

enum opcode {
  stop_opcode = 0x00,
  add_opcode = 0x01,
  mul_opcode = 0x02,
  sub_opcode = 0x03,
  div_opcode = 0x04,
  // TODO: sdiv 0x05
  mod_opcode = 0x06,
  // TODO: smod 0x07
  // TODO: addmod 0x08
  // TODO: mulmod 0x09
  // TODO: exp 0x0a
  // TODO: signextend 0x0b
  lt_opcode = 0x10,
  // TODO: gt 0x11
  // TODO: slt 0x12
  // TODO: sgt 0x13
  // TODO: eq 0x14
  // TODO: Gap...
  jmpi_opcode = 0x57,
  // TODO: Gap...
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
  std::vector<word> code;

  public:
  machine(std::vector<word> v) : pc(0), code(v), state(running_machine_state) {}
  void step();
  bool is_running();
  std::string to_json();
};
