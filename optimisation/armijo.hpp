#ifndef timeseries_armijo_hpp
#define timeseries_armijo_hpp

#include <typedef.hpp>

namespace timeseries {

  class ArmijoLineSearch{
  public:
    ArmijoLineSearch(Real rate = 0.5,
		     Real rho = 10e-4,
		     Real sigma = 0.9):r_(rate), rho_(rho), sigma_(sigma)
    {}
    
    template<typename F>
    Real operator()(const F& f, 
		    const Eigen::VectorXd& x,
		    const Eigen::VectorXd& dir, 
		    Real stepSize){
      Eigen::VectorXd newX = x + stepSize * dir;
      Eigen::VectorXd df(x.size()), dfnew(x.size());
      f(x, df);
      f(newX, dfnew);
      while((f(newX) > f(x) + rho_ * stepSize * df.transpose() * df) || ( df.transpose() * dir < sigma_ * dfnew.transpose() * dir)){
	stepSize *= r_;	
	df = dfnew;
	newX = x + stepSize * dir;
	f(newX, dfnew);
      }
      return stepSize;
    }    
    Real rho_, r_, sigma_;
  };

}


#endif
