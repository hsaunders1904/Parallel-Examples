#include <iostream>
#include <omp.h>

/**
 * In this example we perform the integral of 4/(1 + x^2) between 1
 * and 0; this should equal pi. We sum over the integral range in a
 * for loop, which we parallelize using OpenMP.
 */
int main() {
  double pi{0.0}, sum{0.0}, x;
  const int N{1000000};
  const double w{1.0/N};

  /**
   * We declare the following block should be performed in parallel.
   *
   * We declare x as private, i.e. each thread keeps its own copy.
   * Private variables are NOT initialized, we can declare them outside
   * the parallel block's scope, but they must be initialized in each
   * thread.
   *
   * We declare sum as firstprivate, i.e. it's private but intialized
   * with the value defined in the master thread; 0.0 in this case.
   *
   * We declare pi as shared, any thread can access it. Shared is the
   * default, however being explicit is not a bad thing. You should be
   * careful that multiple threads are not writing to shared variables
   * at the same time (race conditions).
   */
  #pragma omp parallel private(x), firstprivate(sum), shared(pi)
  {
    #pragma omp for schedule(static)
    for (auto i = 0; i < N; ++i) {
      x = w*(i - 0.5);
      sum += 4.0/(1.0 + x*x);
    }
    /**
     * The following block is 'critical', i.e. we only let one thread
     * execute this block at a time.
     *
     * Other types of blocks include single (only one thread executes this
     * block), master (only the master thread executes this block),
     * atomic (protect a variable by changing it in one step)
     */
    #pragma omp critical
    {
      pi += w*sum;
      std::cout << "Hello from thread " << omp_get_thread_num() << "\n";
    }
  }
  std::cout << "Pi = " << pi << "\n";

  return 0;
}
