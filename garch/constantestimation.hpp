#ifndef timeseries_constantestimation_hpp
#define timeseries_constantestimation_hpp

#include <timeseriesmodel.hpp>
#include <distributions.hpp>

namespace timeseries{

class ConstantEstimation : public GenericEstimationEngine<ConstantMean, ConstantVol, Normal>{

public:
  
  void performEstimation(const RealSeries& rtn){
    RealSeries residual = rtn;
    this->meanModel_->estimate(rtn);
    this->meanModel_->residual(rtn, residual);
    this->volModel_->estimate(residual);
    
  }
  
};
}

#endif
