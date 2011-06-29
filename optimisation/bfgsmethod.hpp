#ifndef timeseries_bfgsmethod_hpp
#define timeseries_bfgsmethod_hpp

#include <Eigen/Eigen>
#include <errorhandling.hpp>
//#include <Eigen/Core>
//#include <Eigen/Core>

namespace timeseries {

  template<typename LineSearch>
  class BFGSMethod{
  public:
    template<typename F>
    Eigen::VectorXd operator()(const F& obj, Eigen::VectorXd x) const{
      Eigen::VectorXd g, gnew, d, y, xold, s;
      Eigen::MatrixXd H;
      LineSearch lineSaerch;
      
      size_t n = x.size();

      obj(x, g);
      obj(x, H);
      xold = x;
      
      H = H.ldlt().solve(Eigen::MatrixXd::Identity(n, n));
      DEBUG_PRINT(H.ldlt().solve(Eigen::MatrixXd::Identity(n, n)));

      
      while(g.norm() > (10e-6 * n)){
#if DEBUG
	std::cout << std::string(20, '-') << std::endl;
#endif
	d = - H * g;
	//	DEBUG_PRINT(d);
	Real a = 1.;
	a = lineSaerch(obj, x, d, a);
	obj(x, g);
	DEBUG_PRINT((x + a * d).transpose());
	x = x +  a * d;
	obj(x, gnew);
	y = gnew - g;
	s = x - xold;
	H.noalias() = (Eigen::MatrixXd::Identity(n, n) - s * y.transpose() / (s.transpose() * y ) ) * H * 
	  (Eigen::MatrixXd::Identity(n, n) - y * s.transpose() / (s.transpose() * y )) + s * s.transpose() / (s.transpose() * y);
	xold = x;
      }
      return x;
    }
  };

}


#endif
