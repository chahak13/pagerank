#ifndef PAGERANK_H_
#define PAGERANK_H_
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

const double DEFAULT_ALPHA = 0.85;
const double DEFAULT_EPSILON = 0.0001;
const unsigned int DEFAULT_MAX_ITERATIONS = 1000;
const string DEFAULT_DELIM = " ";

class Pagerank {
public:
  Pagerank(double alpha = DEFAULT_ALPHA, double epsilon = DEFAULT_EPSILON,
           size_t = DEFAULT_MAX_ITERATIONS, bool verbose = false,
           string delimeter = DEFAULT_DELIM);
  virtual ~Pagerank();
  /**
   * Allocate space for the internal matrix used for calculations.
   */
  void reserve(size_t size);

  /**
   * Return number of rows in the adjacency matrix.
   */
  const size_t get_num_rows();

  /**
   * Set number of rows of the adjacency matrix.
   */
  void set_num_rows(size_t num_rows);

  /**
   * Read graph from file.
   */
  int read_file(const string &filename);

  /**
   * Calculate pagerank values.
   */
  void calculate_pagerank();

  /**
   * Return pagerank vector.
   */
  const vector<double> &get_pagerank();

  /**
   * Return the alpha value (pagerank damping factor).
   */
  const double get_alpha();

  /**
   * Set alpha value (pagerank damping factor).
   */
  void set_alpha(double a);

  /**
   * Return the max iterations to run for pagerank algorithm.
   */
  const unsigned int get_max_iterations();

  /**
   * Set the max iterations to run for pagerank algorithm.
   */
  void set_max_iterations(unsigned int max_iter);

  /**
   * Return the epsilon value used to determine convergence.
   */
  const double get_epsilon();

  /**
   * Set the epsilon value used to determine convergence.
   */
  void set_epsilon(double e);

  /**
   * Returns true if verbose output is enables, false otherwise.
   */
  const bool get_verbose();

  /**
   * Set true for verbose output.
   */
  void set_verbose(bool v);

  /**
   * Return delimeter used to parse graph file. The lines of the
   * file are parsed as:
   * <from><delimeter><to>
   */
  const string get_delimeter();

  /**
   * Set the delimeter to be used for parsing graph file.
   */
  void set_delimeter(string d);

  template <class Vector, class T>
  bool insert_into_vector(Vector &v, const T &t);
  /**
   * Printing utilities
   */
  const void print_params(ostream &out);
  const void print_outgoing();
  const void print_matrix();
  const void print_pagerank();

private:
  bool verbose = false;
  double alpha = 0, epsilon = 0;
  int size = 0;
  unsigned int max_iterations = 0;
  string delimeter = NULL;
  vector<size_t> num_outgoing;
  vector<vector<size_t>> rows;
  vector<double> pr;

  void reset();
  bool add_edge(size_t from, size_t to);
};

#endif // PAGERANK_H_
