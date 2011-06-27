#include <typedef.hpp>

using timeseries::Real;
using timeseries::RealSeries;


class Rosenbrock{
public:
  Real operator()(const RealSeries& x) const{
    return 100. * std::pow(x[1] - x[0] * x[0] , 2.) + std::pow(1-x[0], 2.);
  }
  RealSeries start() const{
    RealSeries start(2);
    start << -1.2 , 1;
    return start;
  }
  RealSeries criticalPoint() const{
    RealSeries start(2);
    start << 1. , 1.;
    return start;
  }
  Real min() const{
    return 0.;
  }
  size_t size() const{
    return 4;
  }
};


