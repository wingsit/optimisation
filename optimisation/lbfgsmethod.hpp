#ifndef timeseries_lbfgsmethod_hpp
#define timeseries_lbfgsmethod_hpp

#include <Eigen/Eigen>
#include <errorhandling.hpp>
#include <fixedqueue.hpp>
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
      LineSearch lineSaerch;
      fixed_queue<Eigen::VectorXd> ys(storage_size_), ss(storage_size_);      
      size_t n = x.size(), iterCount = 0;
      /*
      for(size_t i =0; i < storage_size_; ++i){
	ys.push_back(Eigen::VectorXd::Zero(n));
	ss.push_back(Eigen::VectorXd::Zero(n));
      }
      */

      
      


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
	d = -( H * g);


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
	//	DEBUG_PRINT(H.inverse());
	//	DEBUG_PRINT(H.inverse().ldlt().vectorD());
	obj(x, gnew);
	y = gnew - g;
	s = x - xold;
#if TRACE	
	std::cout << gnew.norm() << " " << s.norm() << " " << y.norm() << "\n";
#endif
       	if(s.dot(y) > 0){

	  {
	    Eigen::VectorXd q = g;
	    Eigen::VectorXd alpha(g.size());	  
	    ys.push_back(y);
	    ss.push_back(s);
	    size_t counter = 0;
	    {

	    fixed_queue<Eigen::VectorXd>::const_reverse_iterator yi = ys.rbegin();
	    fixed_queue<Eigen::VectorXd>::const_reverse_iterator si = ss.rbegin();
	    for(; yi != ys.rend(); ++yi, ++si, ++counter){
	      alpha[counter] = 1. / yi->dot(*si) * si->dot(q);
	      q = q - alpha[counter] * (*yi);
	    }
	    }
	    Eigen::VectorXd r = H * q;
	    fixed_queue<Eigen::VectorXd>::const_iterator yi = ys.begin();
	    fixed_queue<Eigen::VectorXd>::const_iterator si = ss.begin();
	    {
	      for(; yi != ys.end(); ++yi, ++si, --counter){
		double beta = 1./ yi->dot(*si) * yi->transpose() * r;
		r = r + *si * (alpha[counter] - beta);
	      }
	    }
	    
	  }


	  H.noalias() = (Eigen::MatrixXd::Identity(n, n) - s * y.transpose() / (s.transpose() * y ) ) * H * (Eigen::MatrixXd::Identity(n, n) - y * s.transpose() / (s.transpose() * y )) + s * s.transpose() / (s.transpose() * y);
	  
	}else{
#if TRACE
	  std::cout << "Reseted" << std::endl;
#endif
	  H = x.norm() * Eigen::MatrixXd::Identity(n,n); 
	  //	  H = Eigen::MatrixXd::Identity(n,n); 
	}
	
	xold = x;
      }
      return x;
    }
  };

}


#endif
