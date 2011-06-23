#ifndef timeseries_constantmean_hpp
#define timeseries_constantmean_hpp

#include <meanprocess.hpp>

namespace timeseries {





struct ConstantMean {
    Real mean_;
    void residualsImpl(const RealSeries& rtn, RealSeries& residuals) const {
        residuals = rtn - mean_;
    }
    Size meanProcessParameterSizeImpl() const {
        return 1;
    }
    void setMeanProcessParametersImpl(const ParameterArray& p) {
        mean_ = p[0];
    }

    void getMeanProcessParametersImpl( Eigen::VectorBlock<ParameterArray> p) {
        p[0] = mean_;
    }
};

}


#endif
