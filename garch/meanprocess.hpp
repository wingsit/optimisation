#ifndef timeseries_meanprocess_hpp
#define timeseries_meanprocess_hpp

#include <typedef.hpp>

namespace timeseries {


template<class Derived>
struct MeanProcess : public Derived {
    void residuals(const RealSeries& rtn, RealSeries& residuals) const {
        static_cast<const Derived*>(this)->residualsImpl( rtn, residuals);
    }
    Size meanProcessParameterSize() const {
        return static_cast<const Derived*>(this)->meanProcessParameterSizeImpl();
    }
    void setMeanProcessParameters(const ParameterArray& parameters) {
        static_cast<Derived*>(this)->setMeanProcessParametersImpl(parameters);
    }
    void getMeanProcessParameters(Eigen::VectorBlock<ParameterArray> parameters) {
        static_cast<Derived*>(this)->getMeanProcessParametersImpl(parameters);
    }
};

}


#endif
