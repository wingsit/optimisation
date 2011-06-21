#ifndef timeseries_estimationengine_hpp
#define timeseries_estimationengine_hpp

#include <timeseries.hpp>
#include <boost/shared_ptr.hpp>
#include <distributions.hpp>
namespace timeseries{

  class MeanProcessModel;
  class VolatilityProcessModel;

  class EstimationEngine{
  protected:    
    class Arguments;
    class Result;
    boost::shared_ptr<MeanProcessModel> meanModel_;
    boost::shared_ptr<VolatilityProcessModel> volModel_;
    boost::shared_ptr<Distribution> distribution_;

  public:
    //    virtual Arguments* getParameters() = 0;
    //    virtual Result getResults() = 0;
    //    virtual void reset() = 0;
    virtual void setup( boost::shared_ptr<MeanProcessModel> meanModel,
		        boost::shared_ptr<VolatilityProcessModel> volModel,
            boost::shared_ptr<Distribution> dist);
    virtual void performEstimation(const RealSeries& rtn) = 0;
    
  };
  


  template<typename MeanModel, typename VarianceModel, typename Dist>
  class GenericEstimationEngine : public EstimationEngine{};

}


#endif
