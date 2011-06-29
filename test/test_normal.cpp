#include <testutility.hpp>
#include <normal.hpp>


BOOST_AUTO_TEST_CASE(NORMAL_DISTRIBUTION){
  
  using timeseries::Normal;
  using timeseries::RealSeries;
  /*  
  Normal normal;
  double x = 0.;
  normal.pdfImpl(x);
  BOOST_REQUIRE_CLOSE(x, 0.398942, 0.0001);
  */
  Normal normal;
  RealSeries samples(10);
  samples << -5, -4, -3, -2, -1, 0, 1, 2, 3,4 ;
  normal.pdfImpl(samples);  
  std::cout << samples.transpose() << std::endl;
}
