#ifndef timeseries_typedef_hpp
#define timeseries_typedef_hpp

#include <Eigen/Eigen>

namespace timeseries {

typedef Eigen::ArrayXd RealSeries;
typedef Eigen::ArrayXd ParameterArray;
//typedef RealSeries VolatilitySeries;
typedef RealSeries VarianceSeries;

//typedef RealSeries MeanSeries;
typedef long Size;
typedef double Real;
static const Real PI = 3.141592653;

}


#endif


