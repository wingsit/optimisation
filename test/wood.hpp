#include <typedef.hpp>

using timeseries::Real;
using timeseries::RealSeries;

class Wood{
public:
  Real operator()(const Eigen::VectorXd& x) const{
   return 100. * std::pow(x[0] * x[0] - x[1], 2.) + std::pow(x[0] - 1 , 2.) 
     + std::pow(x[2] -1, 2.) + 90. * std::pow(x[2] * x[2] - x[3], 2.) + 
     10.1 * (std::pow( x[1] - 1, 2.) + std::pow(x[3] -1, 2.) ) 
     + 19.8 * (x[1] - 1. ) * (x[3] - 1.);
  }
  Eigen::VectorXd start() const{
    Eigen::VectorXd start(4);
    start << -3, -1, -3, -1.;
    return start;
  }
  Eigen::VectorXd criticalPoint() const{
    return Eigen::VectorXd::Constant(4, 1.);
  }
  Real min() const{
    return 0.;
  }
  size_t size() const{
    return 4;
  }
};
