#include <iostream>
#include <string>

/*
 * In this example we calculate values in the Fibonacci sequence recursively
 * and in parallel, using OpenMP tasks.
 *
 * Warning: This is not efficient code! Any n > 20 will create too many tasks
 * and clog up your processor.
 */
int fib(int n) {
  if (n == 0 || n == 1)
    return 1;
  else {
    int f1, f2;

    /*
     * Here we spin up two OpenMP "tasks". One of the tasks computes fib(n -1)
     * the other computes fib(n -2). Each of those calls generates 2 tasks as
     * well.
     */
    #pragma omp task shared(f1) firstprivate(n)
    f1 = fib(n - 1);
    #pragma omp task shared(f2) firstprivate(n)
    f2 = fib(n - 2);
    /*
    * The "omp taskwait" directive ensures the previous two tasks are complete
    * before we sum the result.
    */
    #pragma omp taskwait
    return f1 + f2;
  }
}

int main(int argc, char *argv[]) {
  int n;
  if (argc == 1)
    n = 20;
  else {
    try {
      n = std::stoi(argv[1], nullptr, 0);
    } catch (const std::exception &exc) {
      std::cerr << "Error: Could not convert '" << argv[1]
                << "' to integer.\n";
      return 1;
    }
  }

  #pragma omp parallel shared(n)
  {
    /*
    * Only one thread will execute this block, however the "fib" call will
    * spin up multiple threads.
    */
    #pragma omp single
    {
      std::cout << fib(n) << "\n";
    }
  }
  return 0;
}
