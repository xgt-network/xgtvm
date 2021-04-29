#include <cstdint>
#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <sstream>
#include <boost/optional.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <typeinfo>

namespace machine
{

typedef uint8_t word;
typedef boost::multiprecision::uint256_t big_word;
typedef boost::multiprecision::int256_t signed_big_word;

enum opcode
{
  // ARITHMETIC
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
  signextend_opcode = 0x0b, // TODO
  lt_opcode = 0x10,
  gt_opcode = 0x11,
  slt_opcode = 0x12,
  sgt_opcode = 0x13,
  eq_opcode = 0x14,
  iszero_opcode = 0x15,

  // BITWISE
  and_opcode = 0x16,
  or_opcode = 0x17,
  xor_opcode = 0x18,
  not_opcode = 0x19,
  byte_opcode = 0x1A, // TODO
  shl_opcode = 0x1B,
  shr_opcode = 0x1C,
  sar_opcode = 0x1D,

  sha3_opcode = 0x20, // TODO
  address_opcode = 0x30, // TODO
  balance_opcode = 0x31, // TODO
  origin_opcode = 0x32, // TODO
  caller_opcode = 0x33, // TODO
  callvalue_opcode = 0x34, // TODO
  calldataload_opcode = 0x35, // TODO
  calldatasize_opcode = 0x36, // TODO
  calldatacopy_opcode = 0x37, // TODO
  codesize_opcode = 0x38, // TODO
  codecopy_opcode = 0x39, // TODO
  gasprice_opcode = 0x3A, // TODO energyprice?
  extcodesize_opcode = 0x3B, // TODO
  extcodecopy_opcode = 0x3C, // TODO
  returndatasize_opcode = 0x3D, // TODO
  returndatacopy_opcode = 0x3E, // TODO
  extcodehash_opcode = 0x3F, // TODO
  blockhash_opcode = 0x40, // TODO
  coinbase_opcode = 0x41, // TODO
  timestamp_opcode = 0x42, // TODO
  number_opcode = 0x43, // TODO
  difficulty_opcode = 0x44, // TODO
  gaslimit_opcode = 0x45, // TODO energylimit?
  pop_opcode = 0x50, // TODO
  mload_opcode = 0x51, // TODO
  mstore_opcode = 0x52, // TODO
  mstore8_opcode = 0x53, // TODO
  sload_opcode = 0x54, // TODO
  sstore_opcode = 0x55, // TODO
  jump_opcode = 0x56,
  jumpi_opcode = 0x57, 
  pc_opcode = 0x58,
  msize_opcode = 0x59, // TODO
  gas_opcode = 0x5A, // TODO energy?
  jumpdest_opcode = 0x5B, // TODO

  // PUSH
  push1_opcode = 0x60,
  push2_opcode = 0x61,
  push3_opcode = 0x62,
  push4_opcode = 0x63,
  push5_opcode = 0x64,
  push6_opcode = 0x65,
  push7_opcode = 0x66,
  push8_opcode = 0x67,
  push9_opcode = 0x68,
  push10_opcode = 0x69,
  push11_opcode = 0x6A,
  push12_opcode = 0x6B,
  push13_opcode = 0x6C,
  push14_opcode = 0x6D,
  push15_opcode = 0x6E,
  push16_opcode = 0x6F,
  push17_opcode = 0x70,
  push18_opcode = 0x71,
  push19_opcode = 0x72,
  push20_opcode = 0x73,
  push21_opcode = 0x74,
  push22_opcode = 0x75,
  push23_opcode = 0x76,
  push24_opcode = 0x77,
  push25_opcode = 0x78,
  push26_opcode = 0x79,
  push27_opcode = 0x7A,
  push28_opcode = 0x7B,
  push29_opcode = 0x7C,
  push30_opcode = 0x7D,
  push31_opcode = 0x7E,
  push32_opcode = 0x7F,

  // DUP
  dup1_opcode = 0x80,
  dup2_opcode = 0x81,
  dup3_opcode = 0x82,
  dup4_opcode = 0x83,
  dup5_opcode = 0x84,
  dup6_opcode = 0x85,
  dup7_opcode = 0x86,
  dup8_opcode = 0x87,
  dup9_opcode = 0x88,
  dup10_opcode = 0x89,
  dup11_opcode = 0x8A,
  dup12_opcode = 0x8B,
  dup13_opcode = 0x8C,
  dup14_opcode = 0x8D,
  dup15_opcode = 0x8E,
  dup16_opcode = 0x8F,

  // SWAP
  swap1_opcode = 0x90,
  swap2_opcode = 0x91,
  swap3_opcode = 0x92,
  swap4_opcode = 0x93,
  swap5_opcode = 0x94,
  swap6_opcode = 0x95,
  swap7_opcode = 0x96,
  swap8_opcode = 0x97,
  swap9_opcode = 0x98,
  swap10_opcode = 0x99,
  swap11_opcode = 0x9A,
  swap12_opcode = 0x9B,
  swap13_opcode = 0x9C,
  swap14_opcode = 0x9D,
  swap15_opcode = 0x9E,
  swap16_opcode = 0x9F,

  // LOG
  log0_opcode = 0xA0, // TODO
  log1_opcode = 0xA1, // TODO
  log2_opcode = 0xA2, // TODO
  log3_opcode = 0xA3, // TODO
  log4_opcode = 0xA4, // TODO

  // Generic push/dup/swap opcodes
  push_opcode = 0xB0, // TODO
  dup_opcode = 0xB1, // TODO
  swap_opcode = 0xB2, // TODO

  create_opcode = 0xF0, // TODO
  call_opcode = 0xF1, // TODO
  callcode_opcode = 0xF2, // TODO
  return_opcode = 0xF3,
  delegatecall_opcode = 0xF4, // TODO
  create2_opcode = 0xF5, // TODO
  staticcall_opcode = 0xFA, // TODO
  revert_opcode = 0xFD, // TODO
  selfdestruct_opcode = 0xFF, // TODO
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
