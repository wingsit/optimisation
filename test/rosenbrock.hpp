#include <typedef.hpp>

using timeseries::Real;
using timeseries::RealSeries;

class Rosenbrock{
public:
  Real operator()(const Eigen::VectorXd& x) const{
    return (100. * std::pow(x[1] - std::pow(x[0], 2.) , 2.) + std::pow(1 - x[0], 2.));
  }
  Eigen::VectorXd start() const{
    Eigen::VectorXd start(2);
    start << -1.2 , 1;
    return start;
  }
  Eigen::VectorXd criticalPoint() const{
    Eigen::VectorXd start(2);
    start << 1. , 1.;
    return start;
  }
  Real min() const{
    return 0.;
  }
  size_t size() const{
    return 2;
  }
};
