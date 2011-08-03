#ifndef timeseries_lbfgsmethod_hpp
#define timeseries_lbfgsmethod_hpp

#include <Eigen/Eigen>
#include <errorhandling.hpp>
#include <fixedqueue.hpp>
#include <lbfgs.hpp>
//#include <Eigen/Core>
//#include <Eigen/Core>

const static size_t storage_size_ = 10;

namespace timeseries {

  template<typename LineSearch>
  class LBFGSMethod{    
  public:
    template<typename F>
    Eigen::VectorXd operator()(const F& obj, Eigen::VectorXd x) const{
      Eigen::VectorXd g, gnew, d, y, xold, s;
      Eigen::MatrixXd H, G;

      size_t n = x.size();

      lbfgs::minimizer<Real> minizer(n);
      lbfgs::traditional_convergence_test<Real> is_c(n);
      for(;;){
	Real f = obj(x);
	obj(x, g);
	if(minizer.run(x.data(), f, g.data())) continue;
	if(is_c(x.data(), g.data())) break;
	if(minizer.nfun() > 5000 * n) break;
      }

      return x;
    }
  };

}


#endif
