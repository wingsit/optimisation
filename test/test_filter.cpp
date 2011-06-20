#include <filter.hpp>
#include <testutility.hpp>
#include <iostream>
#include <boost/progress.hpp>

using boost::timer;


BOOST_AUTO_TEST_CASE(filter){
  size_t n = 1000;

  using Eigen::ArrayXd;

  ArrayXd x(10*n), y(10*n), a(n), b(2*n);
  {
    x = ArrayXd::Random(10*n);
    y = ArrayXd::Random(10*n);
    a = ArrayXd::Zero(n);
    b = ArrayXd::Zero(2*n);
  
    timeseries::filter(x+y, a, b+b, y);
    BOOST_REQUIRE((y.topRows(std::max(a.rows(), b.rows())) == ArrayXd::Zero(std::max(a.rows(), b.rows()))).all());

    x = ArrayXd::Zero(10*n);
    a = ArrayXd::Zero(n);
    b = ArrayXd::Zero(2*n);

    timeseries::filter(x, a, b+b, y);
    BOOST_REQUIRE((y == ArrayXd::Zero(10*n)).all());
  }
  n = 100000;
  /*
  //order = 1
  {
  x = ArrayXd::Random(10*n);
  y = ArrayXd::Random(10*n);
  b = ArrayXd::Zero(0);
  a = ArrayXd::Constant(1,1.);
  ArrayXd yCopy = y;


  timer t0;
  timeseries::filter(x, a, b, y);
  std::cout << t0.elapsed() << std::endl;

  t0.restart();
  yCopy[0] = 0;
  for(size_t i = 1; i < y.rows(); ++i){
  yCopy[i] = a[0] * yCopy[i-1] + x[i];
  }
  std::cout << t0.elapsed() << std::endl;

  for(size_t i = 1; i < y.rows(); ++i){
  BOOST_WARN_CLOSE(y[i], yCopy[i], 0.000001);
  }
  }
  //order = 2
  {
  x = ArrayXd::Random(10*n);
  y = ArrayXd::Random(10*n);
  b = ArrayXd::Zero(0);
  a = ArrayXd::Constant(2,1.);
  ArrayXd yCopy = y;


  timer t0;
  timeseries::filter(x, a, b, y);
  std::cout << t0.elapsed() << std::endl;

  t0.restart();
  yCopy[0] = 0;
  yCopy[1] = 0;
  for(size_t i = 2; i < y.rows(); ++i){
  yCopy[i] = a[0] * yCopy[i-1] 
  + a[1] * yCopy[i-2]
  + x[i];
  }
  std::cout << t0.elapsed() << std::endl;

  for(size_t i = 1; i < y.rows(); ++i){
  BOOST_WARN_CLOSE(y[i], yCopy[i], 0.000001);
  }
  }
  //order = 3
  {
  x = ArrayXd::Random(10*n);
  y = ArrayXd::Random(10*n);
  b = ArrayXd::Zero(0);
  a = ArrayXd::Constant(3,1.);
  ArrayXd yCopy = y;

  timer t0;
  timeseries::filter(x, a, b, y);
  std::cout << t0.elapsed() << std::endl;

  t0.restart();
  yCopy[0] = 0;
  yCopy[1] = 0;
  yCopy[2] = 0;
  for(size_t i = 3; i < y.rows(); ++i){
  yCopy[i] = a[0] * yCopy[i-1] 
  + a[1] * yCopy[i-2]
  + a[2] * yCopy[i-3] + x[i];
  }
  std::cout << t0.elapsed() << std::endl;

  for(size_t i = 1; i < y.rows(); ++i){
  BOOST_WARN_CLOSE(y[i], yCopy[i], 0.000001);
  }
  }
  //order = 4
  {
  x = ArrayXd::Random(10*n);
  y = ArrayXd::Random(10*n);
  b = ArrayXd::Zero(0);
  a = ArrayXd::Constant(4,1.);
  ArrayXd yCopy = y;

  timer t0;
  timeseries::filter(x, a, b, y);
  std::cout << t0.elapsed() << std::endl;

  t0.restart();
  yCopy[0] = 0;
  yCopy[1] = 0;
  yCopy[2] = 0;
  yCopy[3] = 0;
  for(size_t i = 4; i < y.rows(); ++i){
  yCopy[i] = a[0] * yCopy[i-1] 
  + a[1] * yCopy[i-2]
  + a[2] * yCopy[i-3]
  + a[3] * yCopy[i-4]
  + x[i];
  }
  std::cout << t0.elapsed() << std::endl;

  for(size_t i = 1; i < y.rows(); ++i){
  BOOST_WARN_CLOSE(y[i], yCopy[i], 0.000001);
  }
  }
  */
  {
    for(size_t m = 1; m < 20; ++m){
      x = ArrayXd::Random(10*n);
      x = x / x.matrix().norm();
      y = ArrayXd::Random(10*n);
      y = y / y.matrix().norm();
      b = ArrayXd::Zero(0);

      a = ArrayXd::Constant(m, 1.0);
      //      a = a / a.matrix().norm();

      ArrayXd yCopy = y;

      timer t0;
      timeseries::filter(x, a, b, y);

      std::cout << "m = " << m << std::endl;
      std::cout << t0.elapsed() << std::endl;

      t0.restart();
      for(size_t i = 0; i < m; ++i){
	yCopy[i] = 0;
      }

      for(size_t i = m; i < y.rows(); ++i){
	yCopy[i] = x[i];
	for(size_t j = 0; j < m; ++j){
	  yCopy[i] += a[j]*yCopy[i-1-j];
	}
	/*
	  yCopy[i] = a[0] * yCopy[i-1] 
	  + a[1] * yCopy[i-2]
	  + a[2] * yCopy[i-3]
	  + a[3] * yCopy[i-4]
	  + x[i];
	*/
      }
      std::cout << t0.elapsed() << std::endl;

      for(size_t i = 0; i < y.rows(); ++i){
	//	std::cout << i << std::endl;
	BOOST_REQUIRE_CLOSE(y[i], yCopy[i], 0.000001);
	//	BOOST_REQUIRE_EQUAL(y[i], yCopy[i]);
      }
    }
  }

  
}
