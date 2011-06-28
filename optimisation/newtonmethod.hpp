#ifndef timeseries_newtonmethod_hpp
#define timeseries_newtonmethod_hpp

#define DEBUG_PRINT(x) std::cout << #x << ": \n" << x << "\n";

namespace timeseries {
  template<typename LineSearch>
  class NewtonMethod{
  public:
    template<typename F>
    Eigen::VectorXd operator()(const F& obj,  Eigen::VectorXd x) const{
      Eigen::VectorXd g, d;
      Eigen::MatrixXd G;
      LineSearch lineSearch;
      obj(x, g);
      obj(x, G);
      
      DEBUG_PRINT(x);
      DEBUG_PRINT(g);
      DEBUG_PRINT(G);
      DEBUG_PRINT(G.ldlt().solve(g));
      while(g.norm() > 10-3){
	std::cout << std::string(20, '-') << std::endl;
	d = G.ldlt().solve(g);
  DEBUG_PRINT(G.ldlt().solve(g));
	Real a = 1.;
	a = lineSearch(obj, x, d, a);

	DEBUG_PRINT(a);
	DEBUG_PRINT(obj(x));
	x = x +  a * d;
	obj(x, g);
	obj(x, G);
	DEBUG_PRINT(x);
	DEBUG_PRINT(g);
	DEBUG_PRINT(G);
      }
      return x;
    }
  };
}


#endif
