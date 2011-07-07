#define FUSION_MAX_VECTOR_SIZE 50

#include <testutility.hpp>
#include <armijo.hpp>
#include <bfgsmethod.hpp>
#include <derivative.hpp>






BOOST_AUTO_TEST_CASE(BFGS_ON_ROSENBROCK) {
    using namespace timeseries;
    NumericalDerivative<Rosenbrock> f;
    std::cout << (f.criticalPoint() - BFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}



BOOST_AUTO_TEST_CASE(BFGS_ON_EXTENDED_ROSENBROCK) {

    using namespace timeseries;
    NumericalDerivative<ExtendedRosenbrock<10> > f;
    std::cout << (f.criticalPoint() - BFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

BOOST_AUTO_TEST_CASE(BFGS_ON_WOOD) {
    using namespace timeseries;
    NumericalDerivative<Wood> f;
    std::cout << (f.criticalPoint() - BFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

BOOST_AUTO_TEST_CASE(BFGS_ON_POWELLSINGULAR) {
    using namespace timeseries;
    NumericalDerivative<PowellSingular> f;
    std::cout << (f.criticalPoint() - BFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

BOOST_AUTO_TEST_CASE(BFGS_ON_TRIGONOMETRIC) {
    using namespace timeseries;
    NumericalDerivative<Trigonometric<> > f;
    std::cout << (f.criticalPoint() - BFGSMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

struct TestHelper {
    template<typename T>
    void operator()(T t) const {
        using namespace timeseries;
        using boost::timer;
        NumericalDerivative<T> f;
        std::cout << "Run test for <" << typeid(t).name() << "> \n";
        timer ti;
        BFGSMethod<ArmijoLineSearch> bfgs;
        std::cout << (f.criticalPoint() - bfgs(f, f.start())).norm() << std::endl;
        std::cout << "Time Elapsed " << ti.elapsed() << std::endl;
    }
};

BOOST_AUTO_TEST_CASE(BFGS) {
    using namespace timeseries;

    TestProblems testProblems;
    boost::fusion::for_each(testProblems, TestHelper());
}
