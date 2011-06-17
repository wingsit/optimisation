#include "typedef.hpp"
#include "timeseries.hpp"
namespace timeseries{
  template<typename Derived>
  class Distribution{
  public:
    typedef Derived DistributionType;
    void cdf(RealSeries& x) const;
    void inverse_cdf(RealSeries& x) const;
    void pdf(RealSeries& x) const{
      static_cast<const DistributionType*>(this)->pdf(x);
    }
    void operator()(RealSeries& x) const{
      pdf(x);
    }
    void derivative(RealSeries& x) const;
    void sample(RealSeries& x) const;
    Real mean() const{static_cast<const DistributionType*>(this)->mean();}
    Real variance() const{return std::pow(stdv(), 2.);}    
    Real stdv() const{static_cast<const DistributionType*>(this)->stdv();}
    
  };

  class Normal : public Distribution<Normal>{
    Real mean_;
    Real stdv_;
  public:
    Normal(Real mean = 0, 
	   Real stdv = 1):
      mean_(mean), stdv_(stdv){}
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
