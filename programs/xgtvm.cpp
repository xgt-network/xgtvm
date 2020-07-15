#include "machine.hpp"

int main(int argc, word *argv[])
{
  machine m("\x60\x03\x60\x05\x01\x00");
  while (m.is_running())
    m.step();
  std::cout << m.to_json() << std::endl;

  return 0;
}
