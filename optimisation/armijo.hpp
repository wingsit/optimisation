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
      int maxIter = 10, counter = 0;
      f(x, df);
      f(newX, dfnew);
      //      size_t counter = 0;
      if(f(newX) > f(x) + rho_ * stepSize * (df.dot( dir))){
	do{
	  stepSize *= r_;
	  df = dfnew;
	  newX = x + stepSize * dir;
	  f(newX, dfnew);
	  if(counter++ > maxIter) break;
	} while((f(newX) > f(x) + rho_ * stepSize * df.transpose() * df) 
		|| ( df.transpose() * dir < sigma_ * dfnew.transpose() * dir));
      }
      return stepSize;
    }
    Real rho_, r_, sigma_;
  };

}


#endif
