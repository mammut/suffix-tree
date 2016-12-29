// Copyright (c) 2012 Adam Serafini

#include <sstream>
#include <fstream>
#include <iostream>
#include <argp.h>
#include <cstdint>
#include "SuffixTree.h"

struct arguments {
  char *args[1];                /* arg1 & arg2 */
  char *output_name;
  bool draw;
};

/**
 * Parseo de argumentos desde la linea de comandos usando la libreria
 * ARGP. No es estandar de C, por lo que puede que en Mac no compile
 * correctamente
 */
const char *argp_program_version     = "SuffixTree to DFUDS 1.0.1";
const char *argp_program_bug_address = "<juan.escalona@alumnos.inf.utfsm.cl>";
static char doc[] = "SuffixTree to DFUDS translator";
static char args_doc[] = "INPUT_FILE";
static struct argp_option options[] = {
  {"output-name", 'o', "dna", 0,       "Outout file name to create name.ascii & name.letts" },
  {"draw",        'd', 0,     0,      "Send Graphviz DOT language to stdout. It can be later viewed with  ./suffixTree input | dot -tjpg > draw.jpg" },
  { 0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = (struct arguments *)state->input;

  switch (key) {
    case 'o': arguments->output_name = arg; break;
    case 'd': arguments->draw = true; break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 1)
        argp_usage(state);
      arguments->args[state->arg_num] = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 1)
        argp_usage(state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char* argv[]) {
  struct arguments arguments;
  arguments.output_name = NULL;
  arguments.draw = false;
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  std::ifstream in(arguments.args[0], std::ifstream::in);
  if (!in.good()) {
    printf("Error: file '%s' not valid.\n", argv[1]);
    exit(2);
  }
  if ( ! arguments.draw) std::cout << "Reading file " << arguments.args[0] << std::endl;
  std::string name, input;
  std::stringstream strStream;
  strStream << in.rdbuf();
  input = strStream.str();
  if (input[input.length() - 1] == '\n')
    input[input.length() - 1]  = '$';
  if ( ! arguments.draw) std::cout << "File read" << std::endl;

  std::string output = arguments.output_name ? arguments.output_name : "suffixTree";

  SuffixTree tree;

  if ( ! arguments.draw) std::cout << "Constructing Suffix Tree" << std::endl;
  tree.construct(input, output);
  if ( ! arguments.draw) std::cout << "Done constructing Suffix Tree" << std::endl;

  if ( ! arguments.draw) std::cout << "Creating DFUDS files" << std::endl;
  tree.dfuds();
  if ( ! arguments.draw) std::cout << "DFUDS files created " << std::endl;
  if (arguments.draw)
    std::cout << tree.log_tree();
}
