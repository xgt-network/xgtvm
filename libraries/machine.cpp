#include "machine.hpp"

void machine::add_word(std::unique_ptr<word> i)
{
  words.push_back(std::move(i));
}

void machine::add_listener(std::unique_ptr<listener> l)
{
  listeners.push_back(std::move(l));
}

void machine::step()
{
  if (pc >= words.size() - 1)
  {
    state = stopped_machine_state;
    return;
  }
  std::unique_ptr<word>& item = words[pc];

  // Allow to skip evaluation by throwing exception.
  // TODO: Verify this behavior.
  try
  {
    for (auto& listener : listeners)
      listener->pre_word(*this, *item);
    item->evaluate(*this);
    for (auto& listener : listeners)
      listener->post_word(*this, *item);
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

// int machine::get_pc()
// {
//   return pc;
// }

// word machine::get_word(int i)
// {
//   return words[i];
// }

// void machine:increment_pc()
// {
//   pc++;
// }
