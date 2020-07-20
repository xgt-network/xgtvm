#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <algorithm>
#include <iterator>
#include "machine.hpp"
#include "unistd.h"

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

std::vector<word> process_eval_input(const std::string& str)
{
  char delim = ' ';
  std::size_t current, previous = 0;
  std::string token;
  std::vector<word> tokens;
  current = str.find(delim);
  while (current != std::string::npos) {
    tokens.push_back( to_hex( str.substr(previous, current - previous) ) );
    previous = current + 1;
    current = str.find(delim, previous);
  }
  token = str.substr(previous, current - previous);
  tokens.push_back( to_hex(token) );
  return tokens;
}

int main(int argc, char** argv)
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
    context ctx = {0x5c477758};
    machine m(ctx, process_eval_input(std::string(eval_string)));
    while (m.is_running())
    {
      m.step();
      sleep(0.2);
    }
    std::cout << m.to_json() << std::endl;
  }

  return 0;
}
