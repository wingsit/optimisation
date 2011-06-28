#include <testutility.hpp>
#include <statisticalfunctions.hpp>
BOOST_AUTO_TEST_CASE(STATISTICAL_FUNCTIONS){
  using namespace timeseries;
  
  RealSeries data = RealSeries::Zero(100);  
  BOOST_REQUIRE(mean(data) == 0);
  BOOST_REQUIRE(variance(data) == 0);
  BOOST_REQUIRE(stdv(data) == 0);
  
  data = RealSeries::Constant(100, 1.);
  BOOST_REQUIRE(mean(data) == 1);
  BOOST_REQUIRE(variance(data) == 0);
  BOOST_REQUIRE(stdv(data) == 0);
  
  
}
