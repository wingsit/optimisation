#include <typedef.hpp>

using timeseries::Real;
using timeseries::RealSeries;

template<int n_ = 10>
class ExtendedRosenbrock{
  //  size_t n_;
public:
  /*
  ExtendedRosenbrock()){
    //    assert((n_%2)==0);
  }
  */
  Real operator()(const Eigen::VectorXd& x) const{
    Real acc = 0;
    for(int i = 0; i < n_ - 1; ++i){
      acc += 100. * std::pow(x[i+1] - std::pow(x[i], 2.), 2) + std::pow(1 - x[i] , 2.);
    }
    return acc;
  }
  Eigen::VectorXd start() const{
    Eigen::VectorXd start(n_);
    for(int i = 0; i < n_; i+=2){
      start[i] = -1.2;
      start[i+1] = 1;
    }
    return start;
  }
  Eigen::VectorXd criticalPoint() const{
    return Eigen::VectorXd::Constant(n_, 1.);
  }
  Real min() const{
    return 0.;
  }
  size_t size() const{
    return n_;
  }
};
