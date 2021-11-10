#include <cassert>
#include "machine.hpp"

#define test_that(message) \
  std::cerr << "\e[34m" << ( message ) << "\e[0m" << std::endl;
#define assert_message(message, assertion) \
{ \
  bool result = ( assertion ); \
  std::cerr << "  " << ( result ? "\e[32m" : "\e[31m" ) << ( message ) << "\e[0m" << std::endl; \
}

machine::chain_adapter make_chain_adapter()
{
  std::function< std::string(std::vector<machine::word>) > sha3 = [](std::vector<machine::word> memory) -> std::string
  {
    return 0;
  };

  std::function< uint64_t(std::string) > get_balance = [](std::string address) -> uint64_t
  {
    return 0;
  };

  std::function< std::string(std::string) > get_code_hash = [](std::string address) -> std::string
  {
    return "";
  };

  std::function< machine::big_word(uint64_t) > get_block_hash = [](uint64_t block_num) -> machine::big_word
  {
    return {};
  };

  std::function< std::vector<machine::word>(std::string) > get_code_at_addr = [](std::string address) -> std::vector<machine::word>
  {
    return {};
  };

  std::function< std::string(std::vector<machine::word>, machine::big_word) > contract_create = [](std::vector<machine::word> memory, machine::big_word value) -> std::string
  {
    return {};
  };

  std::function< std::vector<machine::word>(std::string, uint64_t, machine::big_word, std::vector<machine::word>) > contract_call = [](std::string address, uint64_t energy, machine::big_word value, std::vector<machine::word> args) -> std::vector<machine::word>
  {
    return {};
  };

  std::function< std::vector<machine::word>(std::string, uint64_t, machine::big_word, std::vector<machine::word>) > contract_callcode = [](std::string address, uint64_t energy, machine::big_word value, std::vector<machine::word> args) -> std::vector<machine::word>
  {
    return {};
  };

  std::function< std::vector<machine::word>(std::string, uint64_t, std::vector<machine::word>) > contract_delegatecall = [](std::string address, uint64_t energy, std::vector<machine::word> args) -> std::vector<machine::word>
  {
    return {};
  };

  std::function< std::vector<machine::word>(std::string, uint64_t, std::vector<machine::word>) > contract_staticcall = [](std::string address, uint64_t energy, std::vector<machine::word> args) -> std::vector<machine::word>
  {
    return {};
  };

  std::function< std::string(std::vector<machine::word>, machine::big_word, std::string) > contract_create2 = [](std::vector<machine::word> memory, machine::big_word value, std::string salt) -> std::string
  {
    return {};
  };

  std::function< bool(std::vector<machine::word>) > revert = [](std::vector<machine::word> memory) -> bool
  {
    return {};
  };

  std::function< machine::big_word(machine::big_word) > get_storage = [](machine::big_word) -> machine::big_word
  {
    return 0;
  };

  std::function< bool(machine::big_word, machine::big_word) > set_storage = [](machine::big_word, machine::big_word value) -> bool
  {
    return false;
  };

  std::function< bool(std::vector<machine::word>) > contract_return = [](std::vector<machine::word> memory) -> bool
  {
    return {};
  };

  std::function< bool(std::string) > self_destruct = [](std::string address) -> bool
  {
    return {};
  };

  std::function< std::vector<machine::word>(std::string) > get_input_data = [](std::string address) -> std::vector<machine::word>
  {
    return {};
  };

  machine::chain_adapter adapter = {
    sha3,
    get_balance,
    get_code_hash,
    get_block_hash,
    get_code_at_addr,
    contract_create,
    contract_call,
    contract_callcode,
    contract_delegatecall,
    contract_staticcall,
    contract_create2,
    revert,
    get_storage,
    set_storage,
    contract_return,
    self_destruct,
    get_input_data
  };

  return adapter;
}

int main(int argc, char** argv)
{
  test_that("machine runs and halts")
  {
    std::vector<machine::word> input = {0x00};
    machine::context ctx = {true, 0x00};
    machine::message msg = {};
    machine::chain_adapter adapter = make_chain_adapter();
    machine::machine m(ctx, input, msg, adapter);
    assert_message( "machine should start running", m.get_state() == machine::machine_state::running );
    while (m.is_running())
      m.step();
    assert_message( "machine should stop when done executing", m.get_state() == machine::machine_state::stopped );
    assert_message( "stack has correct length", m.stack_length() == 0 );
  }

  test_that("machine halts when nothing else to do")
  {
    std::vector<machine::word> input = {};
    machine::context ctx = {true, 0x00};
    machine::message msg = {};
    machine::chain_adapter adapter = make_chain_adapter();
    machine::machine m(ctx, input, msg, adapter);
    while (m.is_running())
      m.step();
    assert_message( "machine should stop when done executing", m.get_state() == machine::machine_state::stopped );
    assert_message( "stack has correct length", m.stack_length() == 0 );
  }

  test_that("machine returns a timestamp")
  {
    std::vector<machine::word> input = {0x42, 0x00};
    machine::context ctx = {true, 0x5c477758};
    machine::message msg = {};
    machine::chain_adapter adapter = make_chain_adapter();
    machine::machine m(ctx, input, msg, adapter);
    while (m.is_running())
      m.step();
    assert_message( "machine should stop when done executing", m.get_state() == machine::machine_state::stopped );
    assert_message( "stack has correct length", m.stack_length() == 1 );
    assert_message( "top of stack has correct value", m.peek_word() == 0x5c477758 );
  }

  test_that("machine adds unsigned 8-bit values")
  {
    std::vector<machine::word> input = {0x60, 0x02, 0x60, 0x03, 0x01, 0x00};
    machine::context ctx = {true, 0x00};
    machine::message msg = {};
    machine::chain_adapter adapter = make_chain_adapter();
    machine::machine m(ctx, input, msg, adapter);
    while (m.is_running())
      m.step();
    assert_message( "machine should stop when done executing", m.get_state() == machine::machine_state::stopped );
    assert_message( "stack has correct length", m.stack_length() == 1 );
    assert_message( "top of stack has correct value", m.peek_word() == 5 );
  }

  test_that("machine can store and load a value to/from memory")
  {
    std::vector<machine::word> input = {0x60, 0x01, 0x60, 0x08, 0x52, 0x60, 0x08, 0x51, 0x00};
    machine::context ctx = {true, 0x00};
    machine::message msg = {};
    machine::chain_adapter adapter = make_chain_adapter();
    machine::machine m(ctx, input, msg, adapter);
    while (m.is_running())
      m.step();
    assert_message( "machine should stop when done executing", m.get_state() == machine::machine_state::stopped );
    assert_message( "stack has correct length", m.stack_length() == 1 );
    assert_message( "top of stack has correct value", m.peek_word() == 1 );
  }

  test_that("machine can get a wallet's balance via the chain adapter")
  {
    std::vector<machine::word> input = {0x31, 0x00};
    machine::context ctx = {true, 0x00};
    machine::message msg = {};
    machine::chain_adapter adapter = make_chain_adapter();
    adapter.get_balance = [](std::string wallet_name) -> uint64_t
    {
      return 2;
    };
    machine::machine m(ctx, input, msg, adapter);
    m.push_string("alice");
    while (m.is_running())
      m.step();
    assert_message( "machine should stop when done executing", m.get_state() == machine::machine_state::stopped );
    assert_message( "stack has correct length", m.stack_length() == 1 );
    assert_message( "top of stack has correct value", m.peek_word() == 2 );
  }

  test_that("do nothing contract evaluates")
  {
    std::vector<machine::word> input = {0x60, 0x80, 0x60, 0x40, 0x52, 0x34, 0x80, 0x15, 0x60, 0x0f, 0x57, 0x60, 0x00, 0x80, 0xfd, 0x5b, 0x50, 0x60, 0x6d, 0x80, 0x60, 0x1d, 0x60, 0x00, 0x39, 0x60, 0x00, 0xf3, 0xfe, 0x60, 0x80, 0x60, 0x40, 0x52, 0x34, 0x80, 0x15, 0x60, 0x0f, 0x57, 0x60, 0x00, 0x80, 0xfd, 0x5b, 0x50, 0x60, 0x04, 0x36, 0x10, 0x60, 0x28, 0x57, 0x60, 0x00, 0x35, 0x60, 0xe0, 0x1c, 0x80, 0x63, 0x2f, 0x57, 0x6f, 0x20, 0x14, 0x60, 0x2d, 0x57, 0x5b, 0x60, 0x00, 0x80, 0xfd, 0x5b, 0x60, 0x33, 0x60, 0x35, 0x56, 0x5b, 0x00, 0x5b, 0x56, 0xfe, 0xa2, 0x64, 0x69, 0x70, 0x66, 0x73, 0x58, 0x22, 0x12, 0x20, 0x97, 0x5b, 0xdb, 0x69, 0x49, 0x44, 0x25, 0x38, 0xb9, 0x0c, 0x14, 0xc4, 0x20, 0xb0, 0xa2, 0xf5, 0x47, 0xf0, 0x43, 0x7e, 0x85, 0xa7, 0x3d, 0xa1, 0x2f, 0xaf, 0x4e, 0x68, 0x34, 0xf0, 0x81, 0x72, 0x64, 0x73, 0x6f, 0x6c, 0x63, 0x43, 0x00, 0x08, 0x07, 0x00, 0x33};
    machine::context ctx = {true, 0x00};
    machine::message msg = {};
    machine::chain_adapter adapter = make_chain_adapter();
    machine::machine m(ctx, input, msg, adapter);
    assert_message( "machine should start running", m.get_state() == machine::machine_state::running );

    std::string line;
    while (m.is_running())
    {
      m.step();
      while ( std::getline(m.get_logger(), line) )
        std::cerr << "\e[36m" << "LOG: " << line << "\e[0m" << std::endl;
      std::cout << m.to_json() << std::endl;
    }
    while ( std::getline(m.get_logger(), line) )
      std::cerr << "\e[36m" << "LOG: " << line << "\e[0m" << std::endl;
    std::cout << m.to_json() << std::endl;

    assert_message( "machine should stop when done executing", m.get_state() == machine::machine_state::stopped );
    assert_message( "stack has correct length", m.stack_length() == 0 );
  }

  test_that("storage contract evaluates")
  {
    std::vector<machine::word> input = {0x60, 0x80, 0x60, 0x40, 0x52, 0x34, 0x80, 0x15, 0x61, 0x00, 0x10, 0x57, 0x60, 0x00, 0x80, 0xfd, 0x5b, 0x50, 0x61, 0x01, 0x50, 0x80, 0x61, 0x00, 0x20, 0x60, 0x00, 0x39, 0x60, 0x00, 0xf3, 0xfe, 0x60, 0x80, 0x60, 0x40, 0x52, 0x34, 0x80, 0x15, 0x61, 0x00, 0x10, 0x57, 0x60, 0x00, 0x80, 0xfd, 0x5b, 0x50, 0x60, 0x04, 0x36, 0x10, 0x61, 0x00, 0x36, 0x57, 0x60, 0x00, 0x35, 0x60, 0xe0, 0x1c, 0x80, 0x63, 0x2e, 0x64, 0xce, 0xc1, 0x14, 0x61, 0x00, 0x3b, 0x57, 0x80, 0x63, 0x60, 0x57, 0x36, 0x1d, 0x14, 0x61, 0x00, 0x59, 0x57, 0x5b, 0x60, 0x00, 0x80, 0xfd, 0x5b, 0x61, 0x00, 0x43, 0x61, 0x00, 0x75, 0x56, 0x5b, 0x60, 0x40, 0x51, 0x61, 0x00, 0x50, 0x91, 0x90, 0x61, 0x00, 0xd9, 0x56, 0x5b, 0x60, 0x40, 0x51, 0x80, 0x91, 0x03, 0x90, 0xf3, 0x5b, 0x61, 0x00, 0x73, 0x60, 0x04, 0x80, 0x36, 0x03, 0x81, 0x01, 0x90, 0x61, 0x00, 0x6e, 0x91, 0x90, 0x61, 0x00, 0x9d, 0x56, 0x5b, 0x61, 0x00, 0x7e, 0x56, 0x5b, 0x00, 0x5b, 0x60, 0x00, 0x80, 0x54, 0x90, 0x50, 0x90, 0x56, 0x5b, 0x80, 0x60, 0x00, 0x81, 0x90, 0x55, 0x50, 0x50, 0x56, 0x5b, 0x60, 0x00, 0x81, 0x35, 0x90, 0x50, 0x61, 0x00, 0x97, 0x81, 0x61, 0x01, 0x03, 0x56, 0x5b, 0x92, 0x91, 0x50, 0x50, 0x56, 0x5b, 0x60, 0x00, 0x60, 0x20, 0x82, 0x84, 0x03, 0x12, 0x15, 0x61, 0x00, 0xb3, 0x57, 0x61, 0x00, 0xb2, 0x61, 0x00, 0xfe, 0x56, 0x5b, 0x5b, 0x60, 0x00, 0x61, 0x00, 0xc1, 0x84, 0x82, 0x85, 0x01, 0x61, 0x00, 0x88, 0x56, 0x5b, 0x91, 0x50, 0x50, 0x92, 0x91, 0x50, 0x50, 0x56, 0x5b, 0x61, 0x00, 0xd3, 0x81, 0x61, 0x00, 0xf4, 0x56, 0x5b, 0x82, 0x52, 0x50, 0x50, 0x56, 0x5b, 0x60, 0x00, 0x60, 0x20, 0x82, 0x01, 0x90, 0x50, 0x61, 0x00, 0xee, 0x60, 0x00, 0x83, 0x01, 0x84, 0x61, 0x00, 0xca, 0x56, 0x5b, 0x92, 0x91, 0x50, 0x50, 0x56, 0x5b, 0x60, 0x00, 0x81, 0x90, 0x50, 0x91, 0x90, 0x50, 0x56, 0x5b, 0x60, 0x00, 0x80, 0xfd, 0x5b, 0x61, 0x01, 0x0c, 0x81, 0x61, 0x00, 0xf4, 0x56, 0x5b, 0x81, 0x14, 0x61, 0x01, 0x17, 0x57, 0x60, 0x00, 0x80, 0xfd, 0x5b, 0x50, 0x56, 0xfe, 0xa2, 0x64, 0x69, 0x70, 0x66, 0x73, 0x58, 0x22, 0x12, 0x20, 0x40, 0x4e, 0x37, 0xf4, 0x87, 0xa8, 0x9a, 0x93, 0x2d, 0xca, 0x5e, 0x77, 0xfa, 0xaf, 0x6c, 0xa2, 0xde, 0x3b, 0x99, 0x1f, 0x93, 0xd2, 0x30, 0x60, 0x4b, 0x1b, 0x8d, 0xaa, 0xef, 0x64, 0x76, 0x62, 0x64, 0x73, 0x6f, 0x6c, 0x63, 0x43, 0x00, 0x08, 0x07, 0x00, 0x33};
    machine::context ctx = {true, 0x00};
    machine::message msg = {};
    machine::chain_adapter adapter = make_chain_adapter();
    machine::machine m(ctx, input, msg, adapter);
    assert_message( "machine should start running", m.get_state() == machine::machine_state::running );

    std::string line;
    while (m.is_running())
    {
      m.step();
      while ( std::getline(m.get_logger(), line) )
        std::cerr << "\e[36m" << "LOG: " << line << "\e[0m" << std::endl;
      std::cout << m.to_json() << std::endl;
    }
    while ( std::getline(m.get_logger(), line) )
      std::cerr << "\e[36m" << "LOG: " << line << "\e[0m" << std::endl;
    std::cout << m.to_json() << std::endl;

    assert_message( "machine should stop when done executing", m.get_state() == machine::machine_state::stopped );
    assert_message( "stack has correct length", m.stack_length() == 0 );
  }

  return 0;
}
