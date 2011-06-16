#ifndef timeseries_hpp
#define timeseries_hpp

#include "typedef.hpp"
#include <Eigen/Dense>
namespace timeseries{
  
  typedef Eigen::Matrix<Real, Eigen::Dynamic, 1> TimeSeries;
  typedef TimeSeries RealSeries;
  typedef TimeSeries VolatilitySeries;
  
}
#endif
