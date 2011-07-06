#include <typedef.hpp>


template<int n_ = 10>
class Trigonometric{
  //  size_t n_; 
public:
  //  Trigonometric(){}
  Real operator()(const Eigen::VectorXd& x) const{
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
  Eigen::VectorXd start() const{
    Eigen::VectorXd start = Eigen::VectorXd::Constant(n_, 0.2/n_);
    
    return start;
  }
  Eigen::VectorXd criticalPoint() const{

    return Eigen::VectorXd::Zero(n_);
  }
  Real min() const{
    return 0.;
  }
  size_t size() const{
    return n_;
  }

};
