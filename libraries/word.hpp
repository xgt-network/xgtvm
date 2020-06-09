#pragma once

class machine;

#include "machine.hpp"

class word
{
  public:
  virtual ~word() {}
  virtual void evaluate(machine& a) = 0;
};

class noop_word : public word
{
  public:
  ~noop_word() {}
  void evaluate(machine& m)
  {
    std::cout << "noop_word::evaluate" << std::endl;
  }
};

class push_word : public word
{
  public:
  ~push_word() {}
  void evaluate(machine& m)
  {
    // word& next_word = m.get_word(m.get_pc() + 1);
    std::cout << "push_word::evaluate" << std::endl;
  }
};
