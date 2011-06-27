#include <typedef.hpp>

class Trigonometric{
  size_t n_; 
public:
  Trigonometric(size_t n = 100):n_(n){}
  Real operator()(const RealSeries& x) const{
    Real acc = 0;
    for(size_t i=0;i<n_;++i){
      Real acc2 = 0;
      for(size_t j = 0; j < n_; ++j){
	acc2 += cos(x[j]);
      }
      acc += std::pow(n_ + i * ( 1 - cos( x[i]) ) - sin(x[i]) - acc2, 2);	
    }
    return acc;
  }
  RealSeries start() const{
    RealSeries start = RealSeries::Constant(n_, 0.2/n_);
    
    return start;
  }
  RealSeries criticalPoint() const{

    return RealSeries::Zero(n_);
  }
  Real min() const{
    return 0.;
  }
  size_t size() const{
    return n;
  }

};
