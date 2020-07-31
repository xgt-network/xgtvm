#include "machine.hpp"

namespace machine
{

union int_aliaser
{
  uint64_t u;
  int64_t i;
};

uint64_t alias_to_uint64_t(int64_t i)
{
  int_aliaser aliaser;
  aliaser.i = i;
  return aliaser.u;
}

int64_t alias_to_int64_t(uint64_t u)
{
  int_aliaser aliaser;
  aliaser.u = u;
  return aliaser.i;
}

int get_byte(uint64_t x, int n)
{
  return ( x >> (8 * n) ) & 0xFF;
}

void set_byte(uint64_t& x, int n, int v)
{
  uint64_t mask = v << (8 * n);
  x = (x & ~mask) | (v << (8 * n));
}

big_word to_big_word(word a)
{
  return a;
}

big_word to_big_word(word a, word b)
{
  big_word va = 0;
  set_byte(va, 0, a);
  set_byte(va, 1, b);
  return va;
}

big_word to_big_word(word a, word b, word c, word d)
{
  big_word va = 0;
  set_byte(va, 0, a);
  set_byte(va, 1, b);
  set_byte(va, 2, c);
  set_byte(va, 3, d);
  return va;
}

big_word to_big_word(word a, word b, word c, word d, word e, word f, word g, word h)
{
  big_word va = 0;
  set_byte(va, 0, a);
  set_byte(va, 1, b);
  set_byte(va, 2, c);
  set_byte(va, 3, d);
  set_byte(va, 4, e);
  set_byte(va, 5, f);
  set_byte(va, 6, g);
  set_byte(va, 7, h);
  return va;
}

std::string inspect(std::vector<word> words)
{
  std::stringstream ss;
  ss << "[";
  for (int i = 0; i < words.size(); i++)
  {
    // TODO: Print hex
    ss << std::to_string(words.at(i));
    if (i != words.size() - 1)
      ss << ", ";
  }
  ss << "]";
  return ss.str();
}

void machine::push_word(big_word v)
{
  stack.push_front(v);
}

big_word machine::pop_word()
{
  big_word v = stack.front();
  stack.pop_front();
  return v;
}

// TODO: Make better
// TODO: Return string instead?
void machine::print_stack()
{
  for (auto it = stack.cbegin(); it != stack.cend(); ++it)
  {
    word w = *it;
    logger << std::to_string(w);
    if (it + 1 != stack.cend())
      logger << " ";
  }
  logger << std::endl;
}

void machine::step()
{
  if (pc > code.size())
  {
    logger << "stop" << std::endl;
    state = machine_state::stopped;
    return;
  }
  word current_instruction = code[pc];
  opcode o = (opcode)current_instruction;
  pc++;

  logger << "step pc " << std::to_string(pc) << " opcode " << std::hex << o  << std::dec << std::endl;

  // Allow to skip evaluation by throwing exception.
  // TODO: Verify this behavior.
  word a, b, c, d, e;
  big_word va, vb, vc, vd, ve;
  switch (o)
  {
    case stop_opcode:
      logger << "op stop" << std::endl;
      state = machine_state::stopped;
      break;
    case add_opcode:
      logger << "op add" << std::endl;
      va = pop_word();
      vb = pop_word();
      vc = va + vb;
      push_word(vc);
      break;
    case mul_opcode:
      logger << "op mul" << std::endl;
      va = pop_word();
      vb = pop_word();
      vc = va * vb;
      push_word(vc);
      break;
    case sub_opcode:
      logger << "op sub" << std::endl;
      va = pop_word();
      vb = pop_word();
      vc = va - vb;
      push_word(vc);
      break;
    case div_opcode:
      logger << "op div" << std::endl;
      va = pop_word();
      vb = pop_word();
      vc = va / vb;
      push_word(vc);
      break;
    case sdiv_opcode:
      // TODO: Must handle up to 256-bit ints
      logger << "op sdiv" << std::endl;
      va = alias_to_int64_t( pop_word() );
      vb = alias_to_int64_t( pop_word() );
      vc = va / vb;
      push_word( alias_to_uint64_t(vc) );
      break;
    case mod_opcode:
      logger << "op mod" << std::endl;
      va = pop_word();
      vb = pop_word();
      vc = va % vb;
      push_word(vc);
      break;
    case smod_opcode:
      // TODO: Must handle up to 256-bit ints
      logger << "op smod" << std::endl;
      va = alias_to_int64_t( pop_word() );
      vb = alias_to_int64_t( pop_word() );
      vc = va % vb;
      push_word( alias_to_uint64_t(vc) );
      break;
    case lt_opcode:
      logger << "op lt" << std::endl;
      va = pop_word();
      vb = pop_word();
      vc = va < vb;
      push_word(vc);
      break;
    case gt_opcode:
      logger << "op gt" << std::endl;
      va = pop_word();
      vb = pop_word();
      vc = va > vb;
      push_word(vc);
      break;
    case timestamp_opcode:
      logger << "op timestamp" << std::endl;
      push_word(ctx.block_timestamp);
      break;
    case jumpi_opcode:
      logger << "op jumpi" << std::endl;
      print_stack();
      va = pop_word();
      vb = pop_word();
      if (vb != 0)
        if (code[a] == jumpdest_opcode)
          pc = va;
      break;
    case mload_opcode:
      logger << "op mload" << std::endl;
      va = pop_word(); // offset
      if (va + 8 >= memory.size())
      {
        state = machine_state::error;
        error_message.emplace("Memory overflow");
      }
      // TODO: Verify order
      vb = to_big_word(
        memory[va + 7],
        memory[va + 6],
        memory[va + 5],
        memory[va + 4],
        memory[va + 3],
        memory[va + 2],
        memory[va + 1],
        memory[va + 0]
      );
      push_word(vb);
      break;
    case mstore_opcode:
      logger << "op mstore" << std::endl;
      va = pop_word(); // offset
      vb = pop_word(); // value
      logger << "memory before: " << inspect(memory) << std::endl;
      if (va + 8 >= memory.size())
        memory.resize(va + 8);
      // TODO: Verify order
      memory[va + 0] = get_byte(vb, 7);
      memory[va + 1] = get_byte(vb, 6);
      memory[va + 2] = get_byte(vb, 5);
      memory[va + 3] = get_byte(vb, 4);
      memory[va + 4] = get_byte(vb, 3);
      memory[va + 5] = get_byte(vb, 2);
      memory[va + 6] = get_byte(vb, 1);
      memory[va + 7] = get_byte(vb, 0);
      logger << "memory after: " << inspect(memory) << std::endl;
      break;
    case jumpdest_opcode:
      logger << "op jumpdest" << std::endl;
      break;
    case push1_opcode:
      logger << "op push1" << std::endl;
      a = code[pc];
      pc++;
      push_word(a);
      break;
    case push2_opcode:
      logger << "op push2" << std::endl;
      a = code[pc];
      pc++;
      b = code[pc];
      pc++;
      va = to_big_word(0, 0, a, b); // TODO: Verify
      push_word(va);
      break;
    case push3_opcode:
      logger << "op push3" << std::endl;
      a = code[pc];
      pc++;
      b = code[pc];
      pc++;
      c = code[pc];
      pc++;
      va = to_big_word(0, a, b, c); // TODO: Verify
      push_word(va);
      break;
    case push4_opcode:
      logger << "op push4" << std::endl;
      a = code[pc];
      pc++;
      b = code[pc];
      pc++;
      c = code[pc];
      pc++;
      d = code[pc];
      pc++;
      va = to_big_word(a, b, c, d); // TODO: Verify
      break;
    case dup1_opcode:
      logger << "op dup1" << std::endl;
      a = stack.front();
      push_word(a);
      break;
    case swap1_opcode:
      logger << "op swap1" << std::endl;
      a = pop_word();
      b = pop_word();
      push_word(a);
      push_word(b);
      break;
    case swap2_opcode:
      logger << "op swap2" << std::endl;
      a = pop_word();
      b = pop_word();
      c = pop_word();
      push_word(a);
      push_word(b);
      push_word(c);
      break;
    case swap3_opcode:
      logger << "op swap3" << std::endl;
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
      logger << "op swap4" << std::endl;
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
    case return_opcode:
      logger << "op return" << std::endl;
      a = pop_word(); // offset
      b = pop_word(); // length
      logger << std::to_string(a) << std::endl;
      logger << std::to_string(b) << std::endl;
      return_value.resize(b);
      // TODO: Bounds checking
      // TODO: Check if size needs to be capped
      // TODO: Optimize
      for (int i = 0; i < return_value.size(); i++)
        return_value[i] = memory[a + i];
      state = machine_state::stopped;
      break;
  }
}

machine_state machine::get_state()
{
  return state;
}

std::stringstream& machine::get_logger()
{
  return logger;
}

bool machine::is_running()
{
  return state == machine_state::running;
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
        s << std::to_string(w);
        if (it + 1 != stack.cend())
          s << ",";
      }
      s << "],";
      s << "\"returnValue\":" << "[";
      for (auto it = return_value.cbegin(); it != return_value.cend(); ++it)
      {
        word w = *it;
        s << std::to_string(w);
        if (it + 1 != return_value.cend())
          s << ",";
      }
      s << "],";
      if (error_message == boost::none)
        s << "\"exceptionError\":" << "null";
      else
        s << "\"exceptionError\":" << "\"" << error_message.value() << "\"";
    }
    s << "}";
  }
  s << "}";
  return s.str();
}

}
