#ifndef timeseries_bfgsmethod_hpp
#define timeseries_bfgsmethod_hpp

namespace timeseries {

  class BFGSMethod{
  public:
    Eigen::VectorXd operator()(const F& obj, const Eigen::VectorXd& x) const{
      Eigen::VectorXd g, gnew, d, y;
      Eigen::MatrixXd B;
      LinearSearch lineSaerch;
      obj(x, g);
      obj(x, B);
      Real a = 1.;
      
      while(g.norm() > 10-3){
	d = -B.ldl.solve(g);
	a = linearSaerch(obj, x, d, a);
	obj(x, g);
	x += a * d;
	obj(x, gnew);
	y = g - gnew;
	B += g*g.transpose() / (y.transpose() * a * d) - g * B * B * g.transpose() / g * B * g.transpose();
      }
      return x;
    }
  };

}


#endif
