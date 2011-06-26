#ifndef timeseries_constantvariance_hpp
#define timeseries_constantvariance_hpp

#include <varianceprocess.hpp>

namespace timeseries {

struct ConstantVariance {
    Real meanVol_;
    void variancesImpl(const RealSeries& residuals, VarianceSeries& variances) const {
        //        variances = (residuals - meanVol_).pow(2.);
        variances = RealSeries::Constant(residuals.size(), meanVol_*meanVol_);
    }
    Size varianceProcessParameterSizeImpl() const {
        return 1;
    }
    void setVarianceProcessParametersImpl(const ParameterArray& p) {
        meanVol_ = p[0];
    }
    void getVarianceProcessParametersImpl(Eigen::VectorBlock<ParameterArray> p) {
        p[0] = meanVol_;
    }
    void forecastVarianceImpl(const RealSeries& rtn, Size t, RealSeries& forecast) const {
        forecast = RealSeries::Constant(t, meanVol_*meanVol_);
    }
};


}


#endif
