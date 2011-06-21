
#include "timeseries.hpp"
#include <timeseriesmodel.hpp>
#include <constantestimation.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(mean_model) {
  using namespace timeseries;
  {
    ZeroMean meanProcess;
    const static unsigned n = 5000;
    RealSeries rtn = RealSeries::Random(n), residual = RealSeries(n);
    meanProcess.residual(rtn, residual);
    BOOST_REQUIRE_MESSAGE(rtn == residual, "Residual of the zero mean process is not the return itself");  
  }
  {
    ConstantMean meanProcess;
    BOOST_REQUIRE_NO_THROW(meanProcess.mean() = 0.001);    
    const static unsigned n = 5000;
    RealSeries rtn = RealSeries::Random(n), residual = RealSeries(n);
    BOOST_REQUIRE_NO_THROW(meanProcess.estimate(rtn));    
    meanProcess.residual(rtn, residual);
    BOOST_REQUIRE_MESSAGE(((rtn.array()-meanProcess.mean()).matrix() - residual).norm() < 10e-15,
			  "Residual of the zero mean process is not the return itself and the norm of the diff is " 
			  << ((rtn.array()-meanProcess.mean()).matrix() - residual).norm());
    
  }

}

BOOST_AUTO_TEST_CASE(variance_model) {
  using namespace timeseries;
  {
    ConstantMean meanProcess;
    const static unsigned n = 5000;
    RealSeries rtn = RealSeries::Random(n), residual = RealSeries(n);
    meanProcess.residual(rtn, residual);
    ConstantVol volProcess;
    volProcess.estimate(residual);
    BOOST_REQUIRE_CLOSE(std::sqrt(volProcess.meanVariance()),std::sqrt((rtn.array() - rtn.sum()/rtn.size()).matrix().squaredNorm() / (rtn.size()-1)), 0.1);
    

    meanProcess.mean() = -10.;
    volProcess.meanVariance() = 1.;

    MeanVolatilityModel mvModel(meanProcess, volProcess, Normal());
    boost::shared_ptr<EstimationEngine> engine(new ConstantEstimation);
    mvModel.setEngine(engine);
    mvModel.estimate(rtn);
    std::cout << boost::dynamic_pointer_cast<ConstantMean>(mvModel.meanModel())->mean()  << std::endl;

  }
}
