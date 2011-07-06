#define FUSION_MAX_VECTOR_SIZE 50

#include <testutility.hpp>
#include <armijo.hpp>
#include <bfgsmethod.hpp>
#include <derivative.hpp>

#include <rosenbrock.hpp>
#include <extendedrosenbrock.hpp>
#include <wood.hpp>
#include <powellsingular.hpp>
#include <trigonometric.hpp>


#include <boost/progress.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/algorithm.hpp>

typedef boost::fusion::vector<Rosenbrock, 
			      ExtendedRosenbrock<10>, 
			      ExtendedRosenbrock<20>, 
			      ExtendedRosenbrock<100>, 
			      //			      ExtendedRosenbrock<200>, 
			      //			      ExtendedRosenbrock<500>, 
			      //			      ExtendedRosenbrock<1000>, 
			      Wood, 
			      PowellSingular, 
			      Trigonometric<10> ,
			      Trigonometric<20> ,
			      Trigonometric<100> ,
			      Trigonometric<200> ,
			      Trigonometric<500> ,
			      Trigonometric<1000>
			      > TestProblems;

BOOST_AUTO_TEST_CASE(BFGS_ON_ROSENBROCK){
  using namespace timeseries;
  NumericalDerivative<Rosenbrock> f;
  std::cout << (f.criticalPoint() - BFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}



BOOST_AUTO_TEST_CASE(BFGS_ON_EXTENDED_ROSENBROCK){

  using namespace timeseries;
  NumericalDerivative<ExtendedRosenbrock<> > f;
  std::cout << (f.criticalPoint() - BFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

BOOST_AUTO_TEST_CASE(BFGS_ON_WOOD){
  using namespace timeseries;
  NumericalDerivative<Wood> f;
  std::cout << (f.criticalPoint() - BFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

BOOST_AUTO_TEST_CASE(BFGS_ON_POWELLSINGULAR){
  using namespace timeseries;
  NumericalDerivative<PowellSingular> f;
  std::cout << (f.criticalPoint() - BFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

BOOST_AUTO_TEST_CASE(BFGS_ON_TRIGONOMETRIC){
  using namespace timeseries;
  NumericalDerivative<Trigonometric<> > f;
  std::cout << (f.criticalPoint() - BFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

struct TestHelper{
  template<typename T>
  void operator()(T const& t) const{
    using namespace timeseries;
    using boost::timer;
    NumericalDerivative<T> f(t);
    std::cout << "Run test for <" << typeid(t).name() << "> \n";
    timer ti;
    BFGSMethod<ArmijoLineSearch> bfgs;
    std::cout << (f.criticalPoint() - bfgs(f, f.start())).norm() << std::endl;
    std::cout << "Time Elapsed " << ti.elapsed() << std::endl;
  }
};

BOOST_AUTO_TEST_CASE(BFGS){
  using namespace timeseries;

  TestProblems testProblems;
  boost::fusion::for_each(testProblems, TestHelper());
}
