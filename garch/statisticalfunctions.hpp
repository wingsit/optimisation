#ifndef timeseries_statisticalfunctions_hpp
#define timeseries_statisticalfunctions_hpp

#include <typedef.hpp>

namespace timeseries {

  template<typename T>
  inline Real mean(const T& data){
    return data.sum()/data.size();
  }

  template<typename T>
  inline Real variance(const T& data){
    return (data-mean(data)).pow(2.).sum()/(data.size()-1);
  }
  
  template<typename T>
  inline Real stdv(const T& data){
    return std::sqrt(variance(data));
  }
  
  template<typename T>
  inline Real nth_moment(const T& data, double n){
    return mean(data.pow(n));
  }
  
  template<typename T>
  inline Real nth_central_moment(const T& data, double n){
    return nth_moment(data - mean(data));
  }

  inline Eigen::ArrayXd log_return(const Eigen::ArrayXd& data){
    size_t n = data.size();
    return data.segment(1, n-1).log() - data.segment(0, n-1).log();
  }
}


#endif
