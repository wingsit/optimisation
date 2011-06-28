#include <testutility.hpp>

#include <samplemean.hpp>
#include <samplevariance.hpp>
#include <normal.hpp>
#include <timeseriesmodel.hpp>
#include <statisticalfunctions.hpp>



namespace timeseries{
class SampleEstimation {
public:
    SampleEstimation(const TimeSeriesModel<SampleMean, SampleVariance, Normal> model):model_(model) {}
    const TimeSeriesModel<SampleMean, SampleVariance, Normal>& estimate(const RealSeries&
            rtn) {
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

BOOST_AUTO_TEST_CASE(SAMPLE_MODEL){
  using namespace timeseries;

    Size n = 1000;


    RealSeries sample = RealSeries::Random(n), 
      forecast;
    
    for(int i = 0; i < n; ++i){
      sample[i] = generator();
    }
    
    TimeSeriesModel<SampleMean, SampleVariance, Normal> timeSeriesModel2;
    SampleEstimation sampleEstimation(timeSeriesModel2);
    timeSeriesModel2 = sampleEstimation.estimate(sample);
    /*
    std::cout << timeSeriesModel2.sampleMean() << "," << timeSeriesModel2.sampleStandardDeviation() << std::endl;
    Real m;
    m = sample.sum() / sample.size();
    std::cout << (sample.sum()/sample.size()) << "," << std::sqrt((sample - m).pow(2).sum()/(sample.size()-1)) << std::endl;
                                                std::cout <<  std::sqrt((sample - m).pow(2).sum()/(sample.size()-1)) << std::endl;
    timeSeriesModel2.residuals(sample, forecast);
    std::cout << (std::sqrt(forecast.pow(2).sum()/(forecast.size()-1))) << std::endl;
    */

    double TOL = 10e-8;
    BOOST_REQUIRE_CLOSE(timeSeriesModel2.sampleMean(), mean(sample), TOL);
    BOOST_REQUIRE_CLOSE(timeSeriesModel2.sampleStandardDeviation(), stdv(sample), TOL);
    
}
