#ifndef timeseries_estimationengine_hpp
#define timeseries_estimationengine_hpp

#include <timeseries.hpp>

namespace timeseries{

  class MeanProcessModel;
  class VolatilityProcessModel;

  class EstimationEngine{
    
    class Arguments;
    class Result;
    MeanProcessModel* meanModel_;
    VolatilityProcessModel* volModel_;
  public:
    //    virtual Arguments* getParameters() = 0;
    //    virtual Result getResults() = 0;
    //    virtual void reset() = 0;
    virtual void setup( MeanProcessModel* meanModel,
		        VolatilityProcessModel* volModel);
    virtual void performEstimation(const RealSeries& rtn) = 0;
    
  };
  


  template<typename MeanModel, typename VarianceModel, typename Dist>
  class GenericEstimationEngine : public EstimationEngine{};

}


#endif
