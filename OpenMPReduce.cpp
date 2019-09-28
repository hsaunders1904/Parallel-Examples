#include <omp.h>
#include <iostream>

int main() {
  double pi{0.0}, sum{0}, x;
  const int N{1000000};
  const double w{1.0/N};

  #pragma omp parallel private(x), reduction(+:sum)
  {
    #pragma omp for schedule(static)
    for (auto i = 0; i < N; ++i) {
      x = w*(i - 0.5);
      sum = sum + 4.0/(1.0 + x*x);
    }
  }
  pi = w*sum;
  std::cout << "Pi = " << pi << "\n";

  return 0;
}
