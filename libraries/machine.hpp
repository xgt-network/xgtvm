#pragma once

#include "word.hpp"
#include "listener.hpp"

enum opcode {
  null_opcode = 0x00,
  noop_opcode = 0x01,
  push_opcode = 0x02
};

enum machine_state {
  stopped_machine_state,
  running_machine_state
};

// TODO: Notify listeners
class machine
{
  int pc;
  machine_state state;
  std::vector< std::unique_ptr<word> > words;
  std::vector< std::unique_ptr<listener> > listeners;

  public:
  machine() : words(), state(running_machine_state) {}
  void add_word(std::unique_ptr<word> item);
  void add_listener(std::unique_ptr<listener> l);
  void step();
  // TODO: Use these
  bool is_running();
  void stop();
  // int get_pc();
  // word& get_word(int i);
  // void increment_pc();
};
