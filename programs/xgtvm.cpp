#include <vector>
#include <iostream>
#include <sstream>
#include <deque>

class instruction;

enum opcode {
  null_opcode = 0x00,
  noop_opcode = 0x01,
  push_opcode = 0x02,
  add_opcode = 0x03,
  display_opcode = 0x04
};

enum machine_state {
  stopped_machine_state,
  running_machine_state
};

class listener;

// TODO: Notify listeners
class machine
{
  int pc;
  std::deque<char> stack;
  machine_state state;
  std::string code;
  // TODO: Use this
  char current_instruction;
  std::vector< std::unique_ptr<instruction> > instructions;
  std::vector< std::unique_ptr<listener> > listeners;

  public:
  machine() : pc(0), code(), state(running_machine_state) {}
  machine(std::string c) : pc(0), code(c), state(running_machine_state) {}
  void register_instruction(std::unique_ptr<instruction> w);
  void register_listener(std::unique_ptr<listener> l);
  void step();
  // TODO: Use these
  bool is_running();
  void stop();
  char get_instruction(int i);
  int get_pc();
  void stack_push(char c);
  char stack_peek();
  void stack_pop();
  void jump(int i);
};

class listener
{
  public:
  virtual ~listener() {}
  virtual void pre_instruction(machine& a) = 0;
  virtual void post_instruction(machine& a) = 0;
};

class instruction
{
  public:
  virtual ~instruction() {}
  virtual void evaluate(machine& a) = 0;
};

class noop_instruction : public instruction
{
  public:
  ~noop_instruction() {}
  void evaluate(machine& m)
  {
    if (m.get_instruction(m.get_pc()) != noop_opcode) return;
    std::cout << "noop_instruction::evaluate" << std::endl;
  }
};

class push_instruction : public instruction
{
  public:
  ~push_instruction() {}
  void evaluate(machine& m)
  {
    if (m.get_instruction(m.get_pc()) != push_opcode) return;
    std::cout << "push_instruction::evaluate" << std::endl;
    char next_instruction = m.get_instruction(m.get_pc() + 1);
    m.stack_push(next_instruction);
    m.jump(m.get_pc() + 1);
  }
};

class add_instruction : public instruction
{
  public:
  ~add_instruction() {}
  void evaluate(machine& m)
  {
    if (m.get_instruction(m.get_pc()) != add_opcode) return;
    std::cout << "add_instruction::evaluate" << std::endl;
    char a = m.stack_peek();
    m.stack_pop();
    char b = m.stack_peek();
    m.stack_pop();
    m.stack_push(a + b);
  }
};

class display_instruction : public instruction
{
  public:
  ~display_instruction() {}
  void evaluate(machine& m)
  {
    if (m.get_instruction(m.get_pc()) != display_opcode) return;
    std::cout << "add_instruction::evaluate" << std::endl;
    char v = m.stack_peek();
    m.stack_pop();
    std::cout << "DISPLAY: " << (int)v << std::endl;
  }
};

class stub_listener : public listener
{
  public:
  ~stub_listener() {}
  void pre_instruction(machine& a)
  {
    std::cout << "stub_listener::pre_instruction" << std::endl;
  }
  void post_instruction(machine& a)
  {
    std::cout << "stub_listener::post_instruction" << std::endl;
  }
};

void machine::register_instruction(std::unique_ptr<instruction> w)
{
  instructions.push_back(std::move(w));
}

void machine::register_listener(std::unique_ptr<listener> l)
{
  listeners.push_back(std::move(l));
}

void machine::step()
{
  if (pc >= code.size())
  {
    state = stopped_machine_state;
    return;
  }
  current_instruction = code[pc];

  // Allow to skip evaluation by throwing exception.
  // TODO: Verify this behavior.
  try
  {
    for (auto& listener : listeners)
      listener->pre_instruction(*this);
    for (auto& instruction : instructions)
      instruction->evaluate(*this);
    for (auto& listener : listeners)
      listener->post_instruction(*this);
  }
  catch (std::exception& e)
  {
  }

  pc++;
}

bool machine::is_running()
{
  return state == running_machine_state;
}

void machine::stop()
{
  state = stopped_machine_state;
}

char machine::get_instruction(int i)
{
  return code[i];
}

int machine::get_pc()
{
  return pc;
}

void machine::stack_push(char c)
{
  stack.push_front(c);
}

char machine::stack_peek()
{
  return stack[0];
}

void machine::stack_pop()
{
  stack.pop_front();
}

void machine::jump(int i)
{
  pc = i;
}

int main(int argc, char *argv[])
{
  std::cout << "-----" << std::endl;
  {
    // machine m("\x01\x00");
    machine m("\x02\x01\x02\x01\x03\x04");
    m.register_listener(std::move(std::unique_ptr<listener>(new stub_listener())));
    m.register_instruction(std::move(std::unique_ptr<instruction>(new noop_instruction())));
    m.register_instruction(std::move(std::unique_ptr<instruction>(new push_instruction())));
    m.register_instruction(std::move(std::unique_ptr<instruction>(new add_instruction())));
    m.register_instruction(std::move(std::unique_ptr<instruction>(new display_instruction())));
    while (m.is_running())
      m.step();
  }

  return 0;
}
