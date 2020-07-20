#include "machine.hpp"

template <typename T>
int get_byte(T x, int n) {
  return ( x >> (8 * n) ) & 0xFF;
}

// TODO: Make better
// TODO: Return string instead?
void machine::print_stack()
{
  for (auto it = stack.cbegin(); it != stack.cend(); ++it)
  {
    word w = *it;
    std::cerr << std::to_string(w);
    if (it + 1 != stack.cend())
      std::cerr << " ";
  }
  std::cerr << std::endl;
}

void machine::step()
{
  if (pc > code.size())
  {
    state = stopped_machine_state;
    return;
  }
  word current_instruction = code[pc];
  opcode o = (opcode)current_instruction;
  pc++;

  std::cerr << "step " << std::to_string(pc) << " " << std::to_string(o) << std::endl;

  // Allow to skip evaluation by throwing exception.
  // TODO: Verify this behavior.
  word a, b, c;
  switch (o)
  {
    case stop_opcode:
      std::cerr << "stop" << std::endl;
      state = stopped_machine_state;
      break;
    case add_opcode:
      std::cerr << "add" << std::endl;
      a = stack.front();
      stack.pop_front();
      b = stack.front();
      stack.pop_front();
      c = a + b;
      stack.push_front(c);
      break;
    case mul_opcode:
      std::cerr << "mul" << std::endl;
      a = stack.front();
      stack.pop_front();
      b = stack.front();
      stack.pop_front();
      c = a * b;
      stack.push_front(c);
      break;
    case sub_opcode:
      std::cerr << "sub" << std::endl;
      a = stack.front();
      stack.pop_front();
      b = stack.front();
      stack.pop_front();
      c = a - b;
      stack.push_front(c);
      break;
    case div_opcode:
      std::cerr << "div" << std::endl;
      a = stack.front();
      stack.pop_front();
      b = stack.front();
      stack.pop_front();
      c = a / b;
      stack.push_front(c);
      break;
    case mod_opcode:
      std::cerr << "mod" << std::endl;
      a = stack.front();
      stack.pop_front();
      b = stack.front();
      stack.pop_front();
      c = a % b;
      stack.push_front(c);
      break;
    case lt_opcode:
      std::cerr << "lt" << std::endl;
      a = stack.front();
      stack.pop_front();
      b = stack.front();
      stack.pop_front();
      c = a < b;
      stack.push_front(c);
      break;
    case timestamp_opcode:
      std::cerr << "timestamp" << std::endl;
      stack.push_front(get_byte(ctx.block_timestamp, 0));
      stack.push_front(get_byte(ctx.block_timestamp, 1));
      stack.push_front(get_byte(ctx.block_timestamp, 2));
      stack.push_front(get_byte(ctx.block_timestamp, 3));
      break;
    case jumpi_opcode:
      std::cerr << "jumpi" << std::endl;
      print_stack();
      a = stack.front();
      stack.pop_front();
      b = stack.front();
      stack.pop_front();
      if (b != 0)
        if (code[a] == jumpdest_opcode)
          pc = a;
      break;
    case jumpdest_opcode:
      break;
    case push1_opcode:
      std::cerr << "push1" << std::endl;
      a = code[pc];
      pc++;
      stack.push_front(a);
      break;
    case dup1_opcode:
      a = stack.front();
      stack.push_front(a);
      break;
    case swap1_opcode:
      a = stack.front();
      stack.pop_front();
      b = stack.front();
      stack.pop_front();
      stack.push_front(a);
      stack.push_front(b);
      break;
    case swap2_opcode:
      a = stack.front();
      stack.pop_front();
      b = stack.front();
      stack.pop_front();
      c = stack.front();
      stack.pop_front();
      stack.push_front(a);
      stack.push_front(b);
      stack.push_front(c);
      break;
  }
}

bool machine::is_running()
{
  return state == running_machine_state;
}

std::string machine::to_json()
{
  std::stringstream s;
  s << "{";
  {
    s << "\"finalState\":" << "{";
    {
      s << "\"pc\":" << pc << ",";
      s << "\"stack\":" << "[";
      for (auto it = stack.cbegin(); it != stack.cend(); ++it)
      {
        word w = *it;
        s << "\"" << std::to_string(w) << "\"";
        if (it + 1 != stack.cend())
          s << ",";
      }
      s << "]";
    }
    s << "}";
  }
  s << "}";
  return s.str();
}
