#ifndef timeseries_samplevariance_hpp
#define timeseries_samplevariance_hpp

namespace timeseries {

class SampleVariance {
    Real meanVol_;
public:
    void variancesImpl(const RealSeries& residuals, VarianceSeries& variances) const {
        variances = (residuals - meanVol_).pow(2.);
    }
    Size varianceProcessParameterSizeImpl() const {
        return 0;
    }
    void setVarianceProcessParametersImpl(const ParameterArray& p) {
    }
    void getVarianceProcessParametersImpl(Eigen::VectorBlock<ParameterArray> p) {
    }
    void forecastVarianceImpl(const RealSeries& rtn, Size t, RealSeries& forecast) const {
        forecast = RealSeries::Constant(t, meanVol_*meanVol_);
    }

    void setSampleVariance(const RealSeries& residuals) {
      meanVol_ = std::sqrt(residuals.pow(2).sum()/ (residuals.size() - 1));
    }


  Real& sampleStandardDeviation(){
    return meanVol_;
  }

  const Real& sampleStandardDeviation() const{
    return meanVol_;
  }


  

};

}


#endif
