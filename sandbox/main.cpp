#include <iostream>
#include <typedef.hpp>
#include <normal.hpp>
#include <timeseriesmodel.hpp>
#include <constantmean.hpp>
#include <constantvariance.hpp>
#include <derivative.hpp>
namespace timeseries {


template<typename T>
class LikelihoodHelper : public std::unary_function<Eigen::VectorXd, Real> {
    T model_;
    RealSeries data_;
public:
    typedef T ModelType;
    LikelihoodHelper(const T& model,
                     const RealSeries data): model_(model), data_(data) {}

  Real operator()(const Eigen::VectorXd& parameters){ 
    
    model_.parameters() = parameters.array();
    model_.setParameters();
    return model_.likelihood(data_);
  }
  T model(){
    return model_;
  }
  const ParameterArray& parameters() const{
    return model_.parameters();
  }

   ParameterArray& parameters() {
    return model_.parameters();
  }
    
};
/*
template<class M, class V, class D>
class Estimator{

  TimeSeriesModel<M, V, D> t_;
public:
  //  template<class M, class V, class D>
  Estimator(  TimeSeriesModel<M, V, D> t):t_(t){}
  Estimator(){}
  void test(){
    t_.parameters();
  }
};
*/

class NewtonMethod {
public:
    template<typename F>
    typename F::ModelType minimise(F f)const {
      Eigen::VectorXd df, p;
      Eigen::MatrixXd ddf;
      while(f.parameters().matrix().norm() > 1e-6){
	p = f.parameters().matrix();
	f(p, df);
	f(p, ddf);
	p += df.colPivHouseholderQr().solve(df);
	f.parameters() = p.array();
      }
      return f.model();
     
    }
};
}
int main() {
    using namespace timeseries;
    TimeSeriesModel<ConstantMean, ConstantVariance, Normal> timeSeriesModel;

    Size n = 1000;
    RealSeries sample = RealSeries::Random(n);
    timeSeriesModel.pdf(sample);

    timeSeriesModel.distributionParameterSize();

    ParameterArray p(2);
    p[0] = 0;
    p[1] = 1;
    timeSeriesModel.parameters() = p;

    timeSeriesModel.setParameters();
    p[0] = 2;
    p[1] = 3;
    timeSeriesModel.parameters() = p;
    timeSeriesModel.fetchParameters();
    timeSeriesModel.likelihood(sample);
    /*
    Estimator<
    ConstantMean,
      ConstantVariance,
      Normal> estimator;
    estimator.test();
    */
    p[0] = 0;
    p[1] = 1;

    LikelihoodHelper<TimeSeriesModel<ConstantMean, ConstantVariance, Normal> > objective(timeSeriesModel, sample);
    std::cout << objective(p) << std::endl;



    NumericalDerivative<LikelihoodHelper<TimeSeriesModel<ConstantMean, ConstantVariance, Normal> > > objective2(objective);

    ParameterArray df(2);
    Eigen::MatrixXd ddf(2,2);
    //    std::cout << objective2(p, df) << std::endl;
    //    std::cout << objective2(p, ddf) << std::endl;
    
    std::cout << p << "\n\n" << df << "\n\n" << ddf << std::endl;

    timeSeriesModel = NewtonMethod().minimise(objective2);
    
}
