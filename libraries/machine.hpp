#include <cstdint>
#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <sstream>
#include <boost/optional.hpp>
#include <boost/multiprecision/cpp_int.hpp>

namespace machine
{

typedef uint8_t word;
typedef boost::multiprecision::uint256_t big_word;
typedef boost::multiprecision::int256_t signed_big_word;

enum opcode
{
  stop_opcode = 0x00,
  add_opcode = 0x01,
  mul_opcode = 0x02,
  sub_opcode = 0x03,
  div_opcode = 0x04,
  sdiv_opcode = 0x05,
  mod_opcode = 0x06,
  smod_opcode = 0x07,
  addmod_opcode = 0x08,
  mulmod_opcode = 0x09,
  exp_opcode = 0x0a,
  signextend_opcode = 0x0b,
  lt_opcode = 0x10,
  gt_opcode = 0x11,
  slt_opcode = 0x12,
  sgt_opcode = 0x13,
  eq_opcode = 0x14,
  iszero_opcode = 0x15,
  and_opcode = 0x16,
  or_opcode = 0x17,
  xor_opcode = 0x18,
  not_opcode = 0x19,
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
  return_opcode = 0xf3,
  // TODO: Gap...
};

enum class machine_state
{
  stopped,
  running,
  error
};

struct context
{
  bool is_debug;
  uint64_t block_timestamp;
};

class machine
{
  size_t pc;
  std::deque<big_word> stack;
  machine_state state;
  std::vector<word> code;
  std::vector<word> memory;
  std::vector<word> return_value;
  context ctx;
  boost::optional<std::string> error_message;
  std::stringstream logger;

  void push_word(big_word v);
  big_word pop_word();
  void log(std::string output);

  public:
  machine(context ctx, std::vector<word> v)
    : ctx(ctx), pc(0), code(v), state(machine_state::running)
  {
  }

  big_word peek_word();
  void print_stack();
  size_t stack_length();
  void step();
  bool is_running();
  machine_state get_state();
  std::stringstream& get_logger();
  std::string to_json();
};

}
