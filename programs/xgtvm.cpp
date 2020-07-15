#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <algorithm>
#include <iterator>
#include "machine.hpp"

// Set by --help
static int help_flag;
static int eval_flag;
static char* eval_string;
static struct option long_options[] = {
  {"help", no_argument, &help_flag, 1},
  {"eval", required_argument, 0, 'e'},
  {0, 0, 0, 0}
};

word to_hex(std::string str)
{
  return std::stoi(str, 0, 16);
}

std::string process_eval_input(const std::string& str)
{
  char delim = ' ';
  std::size_t current, previous = 0;
  std::string token;
  std::ostringstream s;
  current = str.find(delim);
  while (current != std::string::npos) {
    token = str.substr(previous, current - previous);
    s << to_hex(token);
    previous = current + 1;
    current = str.find(delim, previous);
  }
  token = str.substr(previous, current - previous);
  s << to_hex(token);
  return s.str();
}

int main(int argc, word *argv[])
{
  int c;
  for (;;)
  {
    int option_index = 0;
    c = getopt_long(argc, argv, "he:", long_options, &option_index);
    if (c == -1)
      break;
    switch(c)
    {
      case 'h':
        help_flag = 1;
        break;
      case 'e':
        eval_flag = 1;
        eval_string = optarg;
        break;
    }
  }

  // TODO: Add a help message
  if (help_flag)
  {}
  if (eval_flag)
  {
    std::string input = process_eval_input(std::string(eval_string));
    machine m(input);
    while (m.is_running())
      m.step();
    std::cout << m.to_json() << std::endl;
  }

  return 0;
}
