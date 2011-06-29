 #include <testutility.hpp>
#include <armijo.hpp>
#include <bfgsmethod.hpp>
#include <derivative.hpp>

#include <rosenbrock.hpp>

BOOST_AUTO_TEST_CASE(BFGS_ON_ROSENBROCK){
  using namespace timeseries;
  NumericalDerivative<Rosenbrock> f;
  std::cout << (f.criticalPoint() - BFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

#include <extendedrosenbrock.hpp>

BOOST_AUTO_TEST_CASE(BFGS_ON_EXTENDED_ROSENBROCK){
  using namespace timeseries;
  NumericalDerivative<ExtendedRosenbrock> f(100);
  std::cout << (f.criticalPoint() - BFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}


