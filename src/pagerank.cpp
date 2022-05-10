#include "pagerank.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

Pagerank::Pagerank(double a, double e, size_t mi, bool v, string d)
    : verbose(v), alpha(a), epsilon(e), max_iterations(mi), delimeter(d) {}

Pagerank::~Pagerank() { reset(); }

void Pagerank::reset() {
  num_outgoing.clear();
  rows.clear();
  pr.clear();
}

void Pagerank::reserve(size_t size) {
  num_outgoing.reserve(size);
  rows.reserve(size);
}

const size_t Pagerank::get_num_rows() { return rows.size(); }

void Pagerank::set_num_rows(size_t num_rows) {
  num_outgoing.resize(num_rows);
  rows.resize(num_rows);
}

const double Pagerank::get_alpha() { return alpha; }

void Pagerank::set_alpha(double a) { alpha = a; }

const unsigned int Pagerank::get_max_iterations() { return max_iterations; }

void Pagerank::set_max_iterations(unsigned int mi) { max_iterations = mi; }

const double Pagerank::get_epsilon() { return epsilon; }

void Pagerank::set_epsilon(double e) { epsilon = e; }

const vector<double> &Pagerank::get_pagerank() { return pr; }

const bool Pagerank::get_verbose() { return verbose; }

void Pagerank::set_verbose(bool v) { verbose = v; }

const string Pagerank::get_delimeter() { return delimeter; }

void Pagerank::set_delimeter(string d) { delimeter = d; }

int Pagerank::read_file(const string &filename) {
  pair<map<string, size_t>::iterator, bool> ret;
  reset();

  istream *file;

  if (filename.empty()) {
    cerr << "Please provide a valid filename to read graph." << endl;
    exit(1);
  }
  file = new ifstream(filename.c_str());
  if (!file) {
    cerr << "Cannot open file " << filename << endl;
    exit(1);
  }

  size_t delim_len = delimeter.length();
  size_t linenum = 0;
  string line;
  while (getline(*file, line)) {
    string from_str, to_str;
    size_t from, to;
    size_t pos = line.find(delimeter);
    if (pos != string::npos) {
      from_str = line.substr(0, pos);
      from = strtol(from_str.c_str(), NULL, 10);
      to = strtol(line.substr(pos + delim_len).c_str(), NULL, 10);
      add_edge(from, to);
    }
    linenum++;
    line.clear();
  }

  cerr << "Read " << linenum << " lines, " << rows.size() << " vertices."
       << endl;
  return 0;
}

template <class Vector, class T>
bool Pagerank::insert_into_vector(Vector &v, const T &t) {
  typename Vector::iterator i = lower_bound(v.begin(), v.end(), t);
  if (i == v.end() || t < *i) {
    v.insert(i, t);
    return true;
  } else {
    return false;
  }
}

bool Pagerank::add_edge(size_t from, size_t to) {
  bool ret = false;
  size_t max_dim = max(from, to);
  if (verbose) {
    printf("Adding edge %zu => %zu\n", from, to);
  }
  if (rows.size() <= max_dim) {
    max_dim = max_dim + 1;
    if (verbose) {
      printf("Resizing rows from %zu to %zu\n", rows.size(), max_dim);
    }
    rows.resize(max_dim);
    if (num_outgoing.size() <= max_dim) {
      num_outgoing.resize(max_dim);
    }
  }

  ret = insert_into_vector(rows[to], from);
  if (ret) {
    num_outgoing[from]++;
    if (verbose) {
      cout << "Added " << from << " => " << to << endl;
    }
  }
  return ret;
}

const void Pagerank::print_params(ostream &out) {
  out << "alpha = " << alpha << " epsilon = " << epsilon
      << " max_iterations = " << max_iterations << " delimiter = '" << delimeter
      << "'" << endl;
}

const void Pagerank::print_outgoing() {
  vector<size_t>::iterator cn;

  cout << "[ ";
  for (cn = num_outgoing.begin(); cn != num_outgoing.end(); cn++) {
    cout << *cn << " ";
  }
  cout << "]" << endl;
}

const void Pagerank::print_matrix() {
  vector<vector<size_t>>::iterator cr;
  vector<size_t>::iterator cc; // current column

  size_t i = 0;
  for (cr = rows.begin(); cr != rows.end(); cr++) {
    cout << i << ":[ ";
    for (cc = cr->begin(); cc != cr->end(); cc++) {
      cout << *cc << " ";
    }
    cout << "]" << endl;
    i++;
  }
}

void Pagerank::calculate_pagerank() {
  std::vector<size_t>::iterator ci;
  double error = 99999;
  size_t i;
  double sum_pr;
  double dangling_pr;
  unsigned long num_iterations = 0;
  std::vector<double> old_pr;

  size_t num_rows = rows.size();

  if (num_rows == 0) {
    return;
  }

  pr.resize(num_rows);
  for (int i = 0; i < num_rows; ++i) {
    pr[i] = 1.0 / num_rows;
  }
  // if (verbose) {
  //     print_pagerank();
  // }

  while (error > epsilon && num_iterations < max_iterations) {
    sum_pr = 0;
    dangling_pr = 0;

    for (size_t i = 0; i < pr.size(); ++i) {
      double cpr = pr[i];
      sum_pr += cpr;
      if (num_outgoing[i] == 0) {
        dangling_pr += cpr;
      }
    }

    if (num_iterations == 0) {
      old_pr = pr;
    } else {
      for (size_t i = 0; i < pr.size(); ++i) {
        old_pr[i] = pr[i] / sum_pr;
      }
    }

    sum_pr = 1;

    double one_A = alpha * dangling_pr / num_rows;
    double one_I = (1 - alpha) * sum_pr / num_rows;

    double diff = 0;
    for (size_t i = 0; i < num_rows; ++i) {
      double h = 0.0;
      for (ci = rows[i].begin(); ci != rows[i].end(); ci++) {
        double h_i = (num_outgoing[*ci]) ? 1.0 / num_outgoing[*ci] : 0.0;
        if (num_iterations == 0 && verbose) {
          cout << "h[" << i << ", " << *ci << "] = " << h_i << endl;
        }
        h += h_i * old_pr[*ci];
      }
      h *= alpha;
      pr[i] = h + one_A + one_I;
      diff += fabs(pr[i] - old_pr[i]);
    }
    error = diff;
    num_iterations++;
    if (verbose && num_iterations % 100 == 0) {
      cout << "Iterations: " << num_iterations << ", error: " << error << endl;
    }
  }
}

const void Pagerank::print_pagerank() {
  size_t i;
  size_t num_rows = pr.size();
  double sum = 0;

  cout.precision(numeric_limits<double>::digits10);

  for (i = 0; i < num_rows; i++) {
    cout << i << " = " << pr[i] << endl;
    sum += pr[i];
  }

  cerr << "s = " << sum << " " << endl;
}
