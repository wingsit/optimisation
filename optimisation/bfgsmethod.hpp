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
      Eigen::MatrixXd H, G;
      LineSearch lineSaerch;
      
      size_t n = x.size(), iterCount = 0;

      obj(x, g);
      DEBUG_PRINT(g);
      //obj(x, H); H = H.ldlt().solve(Eigen::MatrixXd::Identity(n, n));
      
      H = 10e-6/g.norm() *Eigen::MatrixXd::Identity(n,n);
      xold = x;
      
      
      //      DEBUG_PRINT(H.ldlt().solve(Eigen::MatrixXd::Identity(n, n)));

      std::cout.precision(8);
      Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, 0, " ", "", "", "", "", "");
      while(g.norm() > (10e-6) * n){
#if DEBUG
	std::cout << std::string(20, '-') << std::endl;
#endif
	d = - H * g;
	//	DEBUG_PRINT(d);
	Real a = 1.;
	a = lineSaerch(obj, x, d, a);
	obj(x, g);
	DEBUG_PRINT((x + a * d).transpose());
#if TRACE
	std::cout.precision(8);
	std::cout << std::setw(5) <<  iterCount++ 
		  << " " << std::setw(10) << obj(x) 
		  << " " << x.transpose()
		  << " " << g.transpose() << " ";
#endif
	x = x +  a * d;
	DEBUG_PRINT(obj(x));
	DEBUG_PRINT(H.inverse());
	DEBUG_PRINT(H.inverse().ldlt().vectorD());
	obj(x, gnew);
	y = gnew - g;
	s = x - xold;
#if TRACE	
	std::cout << gnew.norm() << " " << s.norm() << " " << y.norm() << "\n";
#endif
       	if(s.dot(y) > 0){
	  H.noalias() = (Eigen::MatrixXd::Identity(n, n) - s * y.transpose() / (s.transpose() * y ) ) * H * 
	  (Eigen::MatrixXd::Identity(n, n) - y * s.transpose() / (s.transpose() * y )) + s * s.transpose() / (s.transpose() * y);
	}else{
	  H = x.norm() * Eigen::MatrixXd::Identity(n,n);
	  //H = Eigen::MatrixXd::Identity(n,n);
	}
	
	xold = x;
      }
      return x;
    }
  };

}


#endif
