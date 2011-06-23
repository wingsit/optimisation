#ifndef timeseries_varianceprocess_hpp
#define timeseries_varianceprocess_hpp

#include <typedef.hpp>

namespace timeseries {



template<class Derived>
struct  VarianceProcess : public Derived {
    void variances(const RealSeries& residuals, VarianceSeries& variances) const {
        static_cast< const Derived*>(this)->variancesImpl(residuals, variances);
    }
    Size varianceProcessParameterSize() const {
        return static_cast<const Derived*>(this)->varianceProcessParameterSizeImpl();
    }
    void setVarianceProcessParameters(const ParameterArray& parameters) {
        static_cast<Derived*>(this)->setVarianceProcessParametersImpl(parameters);
    }
    void getVarianceProcessParameters(Eigen::VectorBlock<ParameterArray> parameters) {
        static_cast< Derived*>(this)->getVarianceProcessParametersImpl(parameters);
    }
};

}


#endif
