#include <testutility.hpp>
#include <timeseriesmodel.hpp>
#include <constantmean.hpp>
#include <constantvariance.hpp>
#include <normal.hpp>
#include <derivative.hpp>
#include <newtonmethod.hpp>
#include <bfgsmethod.hpp>
#include <armijo.hpp>
#include <statisticalfunctions.hpp>
namespace timeseries{

  /*
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
	    DEBUG_PRINT(iter);
	    DEBUG_PRINT(p);
	    DEBUG_PRINT(f(p));
	    DEBUG_PRINT(df);
	    DEBUG_PRINT(ddf);
            p -= ddf.ldlt().solve(df);
            f.parameters() = p.array();
            iter++;
        }
        return f.model();
    }
};
  */
}
BOOST_AUTO_TEST_CASE(NEWTON_METHOD){
  using namespace timeseries;
  
  TimeSeriesModel<ConstantMean, ConstantVariance, Normal> timeSeriesModel;

    Size n = 500;
    //    RealSeries sample = RealSeries::Random(n);
        RealSeries sample(n);
	for(int i = 0; i < n; ++i){
	  sample[i] = generator();
	}
	  
    

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

    p[0] = 0.5;
    p[1] = 1;

    LikelihoodHelper<TimeSeriesModel<ConstantMean, ConstantVariance, Normal> > objective(timeSeriesModel, sample);
    std::cout << objective(p) << std::endl;

    NumericalDerivative<LikelihoodHelper<TimeSeriesModel<ConstantMean, ConstantVariance, Normal> > , Central> objective2(objective);

    //std::cout << NewtonMethod<ArmijoLineSearch>()(objective2, p).transpose() << std::endl;
    //    std::cout << BFGSMethod<ArmijoLineSearch>()(objective2, p).transpose() << std::endl;
    std::cout << mean(sample) << " " << stdv(sample) << "\n";

    /*
    ParameterArray df(2);
    Eigen::MatrixXd ddf(2,2);
    //    std::cout << objective2(p, df) << std::endl;
    //    std::cout << objective2(p, ddf) << std::endl;

    std::cout << p << "\n\n" << df << "\n\n" << ddf << std::endl;

    timeSeriesModel = NewtonMethod().minimise(objective2);

    RealSeries forecast;
    timeSeriesModel.forecastVariance(sample, 10, forecast);
    std::cout << forecast << std::endl;
    */
}


#include <rosenbrock.hpp>

BOOST_AUTO_TEST_CASE(NEWTON_ON_ROSENBROCK){
  using namespace timeseries;
  NumericalDerivative<Rosenbrock> f;
  std::cout << (f.criticalPoint() - NewtonMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}

#include <extendedrosenbrock.hpp>

BOOST_AUTO_TEST_CASE(NEWTON_ON_EXTENDED_ROSENBROCK){
  using namespace timeseries;
  NumericalDerivative<ExtendedRosenbrock<> > f;
  std::cout << (f.criticalPoint() - NewtonMethod<ArmijoLineSearch>()(f, f.start())).norm() << std::endl;
}
