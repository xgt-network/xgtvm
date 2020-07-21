#include "machine.hpp"

template <typename T>
word get_byte(T x, word n)
{
  return ( x >> (8 * n) ) & 0xFF;
}

template <typename T>
T set_byte(T x, word n, word v)
{
  T mask = v << (8 * n);
  return (x & ~mask) | (v << (8 * n));
}

template <typename T>
T make_t(std::vector<int> bytes)
{
  T x = 0;
  for (int i = 0; i < bytes.size(); i++)
    x = set_byte(x, i, bytes[i]);
  return x;
}

void machine::push_word(word v)
{
  stack.push_front(v);
}

word machine::pop_word()
{
  word a = stack.front();
  stack.pop_front();
  return a;
}

void machine::push_big_word(big_word v)
{
  push_word(get_byte(v, 0));
  push_word(get_byte(v, 1));
  push_word(get_byte(v, 2));
  push_word(get_byte(v, 3));
}

big_word machine::pop_big_word()
{
  word a = pop_word();
  word b = pop_word();
  word c = pop_word();
  word d = pop_word();
  std::vector<int> words = { a, b, c, d };
  return make_t<uint64_t>(words);
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
  word a, b, c, d, e;
  big_word va, vb, vc;
  switch (o)
  {
    case stop_opcode:
      std::cerr << "stop" << std::endl;
      state = stopped_machine_state;
      break;
    case add_opcode:
      std::cerr << "add" << std::endl;
      a = pop_word();
      b = pop_word();
      c = a + b;
      push_word(c);
      break;
    case mul_opcode:
      std::cerr << "mul" << std::endl;
      a = pop_word();
      b = pop_word();
      c = a * b;
      push_word(c);
      break;
    case sub_opcode:
      std::cerr << "sub" << std::endl;
      a = pop_word();
      b = pop_word();
      c = a - b;
      push_word(c);
      break;
    case div_opcode:
      std::cerr << "div" << std::endl;
      a = pop_word();
      b = pop_word();
      c = a / b;
      push_word(c);
      break;
    case sdiv_opcode:
      std::cerr << "sdiv" << std::endl;
      va = pop_big_word();
      vb = pop_big_word();
      vc = a / b;
      push_big_word(vc);
      break;
    case mod_opcode:
      std::cerr << "mod" << std::endl;
      a = pop_word();
      b = pop_word();
      c = a % b;
      push_word(c);
      stack.push_front(c);
      break;
    case smod_opcode:
      std::cerr << "smod" << std::endl;
      va = pop_big_word();
      vb = pop_big_word();
      vc = va % vb;
      push_big_word(vc);
      break;
    case lt_opcode:
      std::cerr << "lt" << std::endl;
      a = pop_word();
      b = pop_word();
      c = a < b;
      push_word(c);
      break;
    case gt_opcode:
      std::cerr << "gt" << std::endl;
      a = pop_word();
      b = pop_word();
      c = a > b;
      push_word(c);
      break;
    case timestamp_opcode:
      std::cerr << "timestamp" << std::endl;
      push_big_word(ctx.block_timestamp);
      break;
    case jumpi_opcode:
      std::cerr << "jumpi" << std::endl;
      print_stack();
      a = pop_word();
      b = pop_word();
      if (b != 0)
        if (code[a] == jumpdest_opcode)
          pc = a;
      break;
    case jumpdest_opcode:
      std::cerr << "jumpdest" << std::endl;
      break;
    case push1_opcode:
      std::cerr << "push1" << std::endl;
      a = code[pc];
      pc++;
      push_word(a);
      break;
    case push2_opcode:
      std::cerr << "push2" << std::endl;
      a = code[pc];
      pc++;
      b = code[pc];
      pc++;
      push_word(a);
      push_word(b);
      break;
    case push3_opcode:
      std::cerr << "push3" << std::endl;
      a = code[pc];
      pc++;
      b = code[pc];
      pc++;
      c = code[pc];
      pc++;
      push_word(a);
      push_word(b);
      push_word(c);
      break;
    case push4_opcode:
      std::cerr << "push4" << std::endl;
      a = code[pc];
      pc++;
      b = code[pc];
      pc++;
      c = code[pc];
      pc++;
      d = code[pc];
      pc++;
      push_word(a);
      push_word(b);
      push_word(c);
      push_word(d);
      break;
    case dup1_opcode:
      a = stack.front();
      push_word(a);
      break;
    case swap1_opcode:
      a = pop_word();
      b = pop_word();
      push_word(a);
      push_word(b);
      break;
    case swap2_opcode:
      a = pop_word();
      b = pop_word();
      c = pop_word();
      push_word(a);
      push_word(b);
      push_word(c);
      break;
    case swap3_opcode:
      a = pop_word();
      b = pop_word();
      c = pop_word();
      d = pop_word();
      push_word(a);
      push_word(c);
      push_word(b);
      push_word(d);
      break;
    case swap4_opcode:
      a = pop_word();
      b = pop_word();
      c = pop_word();
      d = pop_word();
      e = pop_word();
      push_word(a);
      push_word(d);
      push_word(c);
      push_word(b);
      push_word(e);
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
