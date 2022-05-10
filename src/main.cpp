#include "pagerank.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
using namespace std;

string parse_args(int argc, char *argv[], Pagerank &pr) {
  bool verbose = false;
  double alpha = 0, epsilon = 0;
  int size = 0, max_iterations = 0;
  string delimeter, filename;
  int c, opterror;

  while ((c = getopt(argc, argv, "va:e:s:m:d:f:")) != -1) {
    switch (c) {
    case 'v': {
      verbose = true;
      pr.set_verbose(verbose);
      break;
    }
    case 'a': {
      alpha = stod(optarg);
      pr.set_alpha(alpha);
      break;
    }
    case 'e': {
      epsilon = stod(optarg);
      pr.set_epsilon(epsilon);
      break;
    }
    case 's': {
      size = stoi(optarg);
      pr.set_num_rows(size);
      break;
    }
    case 'm': {
      max_iterations = stoi(optarg);
      pr.set_max_iterations(max_iterations);
      break;
    }
    case 'd': {
      delimeter = string(optarg);
      pr.set_delimeter(delimeter);
      break;
    }
    case 'f': {
      filename = string(optarg);
      break;
    }
    default:
      break;
    }
  }
  return filename;
}
int main(int argc, char *argv[]) {
  Pagerank pr;
  string filename;
  filename = parse_args(argc, argv, pr);
  pr.print_params(cerr);
  pr.read_file(filename);
  pr.print_outgoing();
  pr.print_matrix();
  pr.calculate_pagerank();
  pr.print_pagerank();
  return 0;
}
