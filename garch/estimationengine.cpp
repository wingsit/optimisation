#include <estimationengine.hpp>
#include <timeseriesmodel.hpp>

namespace timeseries{

  void EstimationEngine::setup(boost::shared_ptr<MeanProcessModel> meanModel,
			       boost::shared_ptr<VolatilityProcessModel> volModel,
             boost::shared_ptr<Distribution> dist){
      meanModel_ = meanModel;
      volModel_ = volModel;
      distribution_ = dist;
    }
}
