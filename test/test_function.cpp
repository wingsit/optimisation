#include <boost/test/unit_test.hpp>
#include <function.hpp>
#include <derivative.hpp>
#include <Eigen/Dense>
/*
class F{
public:
  typedef Eigen::VectorXd VectorType;
  double operator()(Eigen::VectorXd const& x)const{
    return x.norm();
  }
};

class G{
public:
  typedef Eigen::VectorXd VectorType;
  double operator()(Eigen::VectorXd const& x)const{
    return x.norm();
  }
  double operator()(Eigen::VectorXd const& x, size_t i)const{
    return 0;
  }
};

BOOST_AUTO_TEST_CASE(FUNCTION){
  Eigen::VectorXd vec = Eigen::VectorXd::Random(10);
  BOOST_REQUIRE_EQUAL(F()(vec), vec.norm());
  NumericalDerivative<F> derivativeF;
  for(size_t i = 0; i < 10; ++i){
    std::cout << derivativeF(Eigen::VectorXd::Random(10)) << std::endl;
    std::cout << derivativeF(Eigen::VectorXd::Random(10), i) << std::endl;
  }

}
*/
