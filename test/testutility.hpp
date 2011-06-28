#ifndef testutility_hpp
#define testutility_hpp
 
#include <boost/test/unit_test.hpp>
#include <typedef.hpp>
#include <iostream>
#include <boost/random.hpp>

static  boost::variate_generator<boost::mt19937, 
    boost::normal_distribution<double> > 
  generator(boost::mt19937(), (boost::normal_distribution<double>()));

#define DEBUG_PRINT(x) std::cout << #x << ": \n" << (x) << "\n";

#endif
