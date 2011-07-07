#ifndef testutility_hpp
#define testutility_hpp

#include <boost/test/unit_test.hpp>
#include <typedef.hpp>
#include <iostream>
#include <boost/random.hpp>
#include <errorhandling.hpp>
static  boost::variate_generator<boost::mt19937,
     boost::normal_distribution<double> >
     generator(boost::mt19937(), (boost::normal_distribution<double>(0., 1.)));

#include <boost/progress.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <rosenbrock.hpp>
#include <extendedrosenbrock.hpp>
#include <wood.hpp>
#include <powellsingular.hpp>
#include <trigonometric.hpp>

typedef boost::fusion::vector<
Rosenbrock,
ExtendedRosenbrock<2>,
ExtendedRosenbrock<10>,
ExtendedRosenbrock<20>,
ExtendedRosenbrock<100>,
//  ExtendedRosenbrock<200>,
//  ExtendedRosenbrock<500>,
//  ExtendedRosenbrock<1000>,
Wood,
PowellSingular,
Trigonometric<10> ,
Trigonometric<20> ,
Trigonometric<100>
//  Trigonometric<200> ,
//  Trigonometric<500>
//Trigonometric<1000>
> TestProblems;

//#define DEBUG_PRINT(x) std::cout << #x << ": \n" << (x) << "\n";

#endif
