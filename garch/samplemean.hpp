#ifndef timeseries_samplemean_hpp
#define timeseries_samplemean_hpp

#include <meanprocess.hpp>

namespace timeseries {

struct SampleMean {
    Real mean_;

    void residualsImpl(const RealSeries& rtn, RealSeries& residuals) const {
        residuals = rtn - mean_;
    }

    Size meanProcessParameterSizeImpl() const {
        return 0;
    }
    void setMeanProcessParametersImpl(const ParameterArray& p) {
    }

    void getMeanProcessParametersImpl( Eigen::VectorBlock<ParameterArray> p) {
    }

    void setSampleMean(const RealSeries& rtn) {
        mean_ = rtn.sum()/rtn.size();
    }

    Real& sampleMean() {
        return mean_;
    }

    const Real& sampleMean() const {
        return mean_;
    }

};

}


#endif
