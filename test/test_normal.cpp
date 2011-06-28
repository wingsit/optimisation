#include <testutility.hpp>
#include <normal.hpp>


BOOST_AUTO_TEST_CASE(NORMAL_DISTRIBUTION){
  
  using timeseries::Normal;
  
  Normal normal;
  double x = 0.;
  normal.pdfImpl(x);
  BOOST_REQUIRE_CLOSE(x, 0.398942, 0.0001);

}
