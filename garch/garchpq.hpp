#ifndef timeseries_garchpq_hpp
#define timeseries_garchpq_hpp

#include <varianceprocess.hpp>
#include <filter.hpp>
namespace timeseries {

/*!
  s^2 = gamma + alpha(p) * a^2 + beta(q) * s^2
*/
template <Size p, Size q>
class Garch {
    Real gamma_;
    ParameterArray alpha_, beta_;
public:

    Garch():alpha_(p), beta_(q) {}
    Garch(const ParameterArray& alpha,
          const ParameterArray& beta,
          Real gamma
         ):alpha_(alpha), beta_(beta), gamma_(gamma) {
        //check the size
    }
    Size varianceProcessParameterSizeImpl() const {
        return p+q+1;
    }

    void setVarianceProcessParametersImpl(const ParameterArray& parameters) {
        gamma_ = parameters[0];
        alpha_ = parameters.segment(1, p);
        beta_ = parameters.segment(p+1, q);
    }
    void getVarianceProcessParametersImpl(Eigen::VectorBlock<ParameterArray> parameters) {
        parameters[0] = gamma_;
        parameters.segment(1, p) = alpha_;
        parameters.segment(p+1, q) = beta_;
    }
    void variancesImpl(const RealSeries& residuals, VarianecSeries& variances) const {
        Real backfill = residuals.segment(0, 20).pow(2).sum()/19;
        filter(residuals, beta_, alpha_, variances, gamma_, backfill, false);
    }

};


/*
  ///Should specialise this for optimisation
template<>
class Garch<1, 1>{
};
*/
}


#endif
