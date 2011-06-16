#include <distributions.hpp>

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_CASE(normal){
  using namespace timeseries;
  
  RealSeries x = RealSeries::Zero(100);
  Normal()(x);
  Normal().mean() == 0;
  
}
