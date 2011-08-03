#define FUSION_MAX_VECTOR_SIZE 50
#include <testutility.hpp>
#include <lbfgs.hpp>

#include <iostream>
#include <lbfgs.hpp>
#include <lbfgsmethod.hpp>
#include <derivative.hpp>
#include <armijo.hpp>
/*
BOOST_AUTO_TEST_CASE(LBFGS)
{
  int passes = 1;

  int n = 100;
  std::vector<double> x(n); double* xb = &(*(x.begin()));
  std::vector<double> g(n); double* gb = &(*(g.begin()));
  for (int ipass = 0; ipass < passes; ipass++) {
    for(int j=0;j<n;j+=2) {
      x[j] = -1.2;
      x[j+1] = 1.;
    }
    timeseries::lbfgs::minimizer<double, int> minimizer(n);
    timeseries::lbfgs::traditional_convergence_test<double, int> is_converged(n);
    if (passes == 1) {
      std::cout << "n: " << minimizer.n() << std::endl;
      std::cout << "m: " << minimizer.m() << std::endl;
      std::cout << "xtol: " << minimizer.xtol() << std::endl;
    }
    for(;;) {
      double f = 0.;
      for(int j=0;j<n;j+=2) {
        double t1 = 1.e0 - x[j];
        double t2 = 1.e1 * (x[j+1] - x[j] * x[j]);
        g[j+1] = 2.e1 * t2;
        g[j] = -2.e0 * (x[j] * g[j+1] + t1);
        f = f + t1 * t1 + t2 * t2;
      }
      if (minimizer.run(xb, f, gb)) continue;
      if (passes == 1) {
        std::cout << "f: " << f;
        std::cout << " gnorm: " << minimizer.euclidean_norm(&(*(g.begin())));
        std::cout << std::endl;
        std::cout << " " << minimizer.iter();
        std::cout << " " << minimizer.nfun();
        std::cout << " " << minimizer.stp();
        std::cout << std::endl;
      }
      if (is_converged(xb, gb)) break;
      if (minimizer.nfun() > 2000) break;
      minimizer.run(xb, f, gb);
    }
  }
}
*/

BOOST_AUTO_TEST_CASE(LBFGS_ON_ROSENBROCK) {
    using namespace timeseries;
    NumericalDerivative<Rosenbrock> f;
    std::cout << (f.criticalPoint() - LBFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}



BOOST_AUTO_TEST_CASE(LBFGS_ON_EXTENDED_ROSENBROCK) {

    using namespace timeseries;
    NumericalDerivative<ExtendedRosenbrock<10> > f;
    std::cout << (f.criticalPoint() - LBFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

BOOST_AUTO_TEST_CASE(LBFGS_ON_WOOD) {
    using namespace timeseries;
    NumericalDerivative<Wood> f;
    std::cout << (f.criticalPoint() - LBFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

BOOST_AUTO_TEST_CASE(LBFGS_ON_POWELLSINGULAR) {
    using namespace timeseries;
    NumericalDerivative<PowellSingular> f;
    std::cout << (f.criticalPoint() - LBFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

BOOST_AUTO_TEST_CASE(LBFGS_ON_TRIGONOMETRIC) {
    using namespace timeseries;
    NumericalDerivative<Trigonometric<> > f;
    std::cout << (f.criticalPoint() - LBFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

struct TestHelper {
    template<typename T>
    void operator()(T t) const {
        using namespace timeseries;
        using boost::timer;
        NumericalDerivative<T> f;
        std::cout << "Run test for <" << typeid(t).name() << "> \n";
        timer ti;
        LBFGSMethod<ArmijoLineSearch> bfgs;
        std::cout << (f.criticalPoint() - bfgs(f, f.start())).norm()/std::sqrt((double)f.size()) << std::endl;
        std::cout << "Time Elapsed " << ti.elapsed() << std::endl;
    }
};

BOOST_AUTO_TEST_CASE(LBFGS) {
    using namespace timeseries;

    TestProblems testProblems;
    boost::fusion::for_each(testProblems, TestHelper());
}
