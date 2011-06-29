#ifndef timeseries_normal_hpp
#define timeseries_normal_hpp

#include <distribution.hpp>


namespace timeseries {

struct Normal {
    Real mean_;
    Real sigma_;
    Normal(Real mean = 0., Real sigma = 1.) : mean_(mean), sigma_(sigma) {}
    template<typename T>
    void pdfImpl(T& x) const {
        Real sigma2 = sigma_ * sigma_;
	T exponent = (-0.5* (x - mean_)*(x-mean_) / sigma2 );
        x = 1. /std::sqrt(2. * PI * sigma_) / sigma_ 
	  * std::exp(
		     (exponent <= -690).select(0, exponent)
		     );
    }
    Size distributionParameterSizeImpl() const {
        return 0;
    }
    void setDistributionParametersImpl(const ParameterArray& p) {
    }
    void getDistributionParametersImpl(Eigen::VectorBlock<ParameterArray> p) {
    }
};


}


#endif
