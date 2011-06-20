#ifndef timeseries_constantestimation_hpp
#define timeseries_constantestimation_hpp

#include <timeseriesmodel.hpp>
#include <distributions.hpp>

namespace timeseries{

class ConstantEstimation : public GenericEstimationEngine<ConstantMean, ConstantVol, Normal>{
  ConstantMean* meanProcess_;
  ConstantVol* volProcess_;
  
public:
  
  void performEstimation(const RealSeries& rtn){
    RealSeries residual = rtn;
    meanProcess_->residual(rtn, residual);
    volProcess_->estimate(residual);
  }
  
};
}

#endif
