#ifndef timeseries_distribution_hpp
#define timeseries_distribution_hpp

namespace timeseries {


template<class Derived>
struct Distribution : public Derived {
    void pdf(RealSeries& sample) const {
        static_cast<const Derived*>(this)->pdfImpl(sample);
    }
    Size distributionParameterSize() const {
        return static_cast<const Derived*>(this)->distributionParameterSizeImpl();
    }
    void setDistributionParameters(const ParameterArray& parameters) {
        static_cast<Derived*>(this)->setDistributionParametersImpl(parameters);
    }
    void getDistributionParameters(Eigen::VectorBlock<ParameterArray> p) {
        static_cast<Derived*>(this)->getDistributionParametersImpl(p);
    }
};

}


#endif
