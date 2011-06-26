#include <iostream>
#include <typedef.hpp>
#include <normal.hpp>
#include <timeseriesmodel.hpp>
#include <constantmean.hpp>
#include <constantvariance.hpp>
#include <derivative.hpp>
#include <samplemean.hpp>
#include <samplevariance.hpp>

namespace timeseries {


template<typename T>
class LikelihoodHelper : public std::unary_function<Eigen::VectorXd, Real> {
    T model_;
    RealSeries data_;
public:
    typedef T ModelType;
    LikelihoodHelper(const T& model,
                     const RealSeries data): model_(model), data_(data) {}

    Real operator()(const Eigen::VectorXd& parameters) {

        model_.parameters() = parameters.array();
        model_.setParameters();
        return model_.loglikelihood(data_);
    }
    T model() {
        return model_;
    }
    const ParameterArray& parameters() const {
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
        size_t iter = 0;
        while(f.parameters().matrix().norm() > 1e-6) {

            p = f.parameters().matrix();
            f(p, df);

            f(p, ddf);
            std::cout << std::string(20, '=') << std::endl;
            std::cout << iter << "\n\n" << p << "\n\n" << df << "\n\n"  << ddf << "\n\n";
            p += ddf.ldlt().solve(df);
            f.parameters() = p.array();
            iter++;
        }
        return f.model();

    }
};


class SampleEstimation{
public:
  SampleEstimation(const TimeSeriesModel<SampleMean, SampleVariance, Normal> model):model_(model){}
  const TimeSeriesModel<SampleMean, SampleVariance, Normal>& estimate(const RealSeries&
rtn){
    model_.setSampleMean(rtn);
    RealSeries residuals(rtn.size());
    model_.residuals(rtn, residuals);
    model_.setSampleVariance(residuals);
    return model_;
  }
private:
  TimeSeriesModel<SampleMean, SampleVariance, Normal> model_;
};
  
}


/*
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
    timeSeriesModel.loglikelihood(sample);
    /*
    Estimator<
    ConstantMean,
      ConstantVariance,
      Normal> estimator;
    estimator.test();
    */
/*
    p[0] = 5;
    p[1] = 20;

    LikelihoodHelper<TimeSeriesModel<ConstantMean, ConstantVariance, Normal> > objective(timeSeriesModel, sample);
    std::cout << objective(p) << std::endl;



    NumericalDerivative<LikelihoodHelper<TimeSeriesModel<ConstantMean, ConstantVariance, Normal> > , Central> objective2(objective);

    ParameterArray df(2);
    Eigen::MatrixXd ddf(2,2);
    //    std::cout << objective2(p, df) << std::endl;
    //    std::cout << objective2(p, ddf) << std::endl;

    std::cout << p << "\n\n" << df << "\n\n" << ddf << std::endl;

    timeSeriesModel = NewtonMethod().minimise(objective2);
    
    RealSeries forecast;
    timeSeriesModel.forecastVariance(sample, 10, forecast);
    std::cout << forecast << std::endl;



    TimeSeriesModel<SampleMean, SampleVariance, Normal> timeSeriesModel2;
    SampleEstimation sampleEstimation(timeSeriesModel2);
    timeSeriesModel2 = sampleEstimation.estimate(sample);
    std::cout << timeSeriesModel2.sampleMean() << "," << timeSeriesModel2.sampleStandardDeviation() << std::endl;
    Real m;
    m = sample.sum() / sample.size();
    std::cout << (sample.sum()/sample.size()) << "," << std::sqrt((sample - m).pow(2).sum()/(sample.size()-1)) << std::endl;
                                                std::cout <<  std::sqrt((sample - m).pow(2).sum()/(sample.size()-1)) << std::endl;
    timeSeriesModel2.residuals(sample, forecast);
    std::cout << (std::sqrt(forecast.pow(2).sum()/(forecast.size()-1))) << std::endl;

}
*/
