#include <cstdint>
#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <sstream>

typedef uint8_t word;
typedef uint64_t big_word;
typedef int address;

enum opcode {
  stop_opcode = 0x00,
  add_opcode = 0x01,
  mul_opcode = 0x02,
  sub_opcode = 0x03,
  div_opcode = 0x04,
  sdiv_opcode = 0x05,
  mod_opcode = 0x06,
  smod_opcode = 0x07,
  // TODO: addmod 0x08
  // TODO: mulmod 0x09
  // TODO: exp 0x0a
  // TODO: signextend 0x0b
  lt_opcode = 0x10,
  gt_opcode = 0x11,
  // TODO: slt 0x12
  // TODO: sgt 0x13
  // TODO: eq 0x14
  // TODO: iszero 0x15
  // TODO: Gap...
  timestamp_opcode = 0x42,
  // TODO: Gap...
  jumpi_opcode = 0x57,
  // TODO: Gap...
  mload_opcode = 0x51,
  mstore_opcode = 0x52,
  // TODO: Gap...
  jumpdest_opcode = 0x5b,
  // TODO: Gap...
  push1_opcode = 0x60,
  push2_opcode = 0x61,
  push3_opcode = 0x62,
  push4_opcode = 0x63,
  // TODO: Gap...
  dup1_opcode = 0x80,
  // TODO: Gap...
  swap1_opcode = 0x90,
  swap2_opcode = 0x91,
  swap3_opcode = 0x92,
  swap4_opcode = 0x93,
  // TODO: Gap...
};

enum machine_state {
  stopped_machine_state,
  running_machine_state,
  error_machine_state
};

struct context
{
  uint64_t block_timestamp;
};

class machine
{
  address pc;
  std::deque<big_word> stack;
  machine_state state;
  std::vector<word> code;
  std::vector<word> memory;
  context ctx;
  std::string error_message;

  void push_word(big_word v);
  big_word pop_word();

  public:
  machine(context ctx, std::vector<word> v)
    : ctx(ctx), pc(0), code(v), state(running_machine_state)
  {
  }

  void print_stack();
  void step();
  bool is_running();
  std::string to_json();
};
