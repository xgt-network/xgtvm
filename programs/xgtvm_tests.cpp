#include <cassert>
#include "machine.hpp"

#define assert_message(message, assertion) \
  { \
    bool result = ( assertion ); \
    std::cerr << ( result ? "\e[32m" : "\e[31m" ) << ( message ) << "\e[0m" << std::endl; \
  }

int main(int argc, char** argv)
{
  std::vector<machine::word> input = {0x60, 0x02, 0x60, 0x03, 0x01, 0x00};
  machine::context ctx = {0x5c477758};
  machine::machine m(ctx, input);
  assert_message( "machine should start running", m.get_state() == machine::machine_state::running );
  while (m.is_running())
    m.step();
  assert_message( "machine should stop when done executing", m.get_state() == machine::machine_state::stopped );

  return 0;
}
