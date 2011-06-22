#ifndef timeseries_distribution_hpp
#define timeseries_distribution_hpp

#include "typedef.hpp"
#include "timeseries.hpp"
namespace timeseries{

  class Distribution{
  public:
    virtual void pdf(RealSeries& x) const = 0;
    virtual Size parameterLength() const = 0;
    void operator()(RealSeries& x) const{
      pdf(x);
    }
  };

  class Normal : public Distribution{
    Real mean_;
    Real stdv_;
  public:
    Normal(Real mean = 0, 
	   Real stdv = 1):
      mean_(mean), stdv_(stdv){}
    Size parameterLength() const{
      return 0;
    }
    void pdf(RealSeries& x) const{      
      x = (1./std::sqrt(2. * 3.141592653 * stdv_ * stdv_) * (-(x.array()-mean_).square()/(2 * stdv_ * stdv_)).exp()).matrix();
    }
    void derivative(RealSeries& x) const{
      x = (-x.array()*(1./std::sqrt(2. * 3.141592653 * stdv_ * stdv_) * (-(x.array()-mean_).square()/(2 * stdv_ * stdv_)).exp())).array();
    }    
    Real mean() const{
      return mean_;
    }
    Real stdv() const{
      return stdv_;
    }    
  };
}

#endif
