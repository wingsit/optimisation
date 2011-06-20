#include <estimationengine.hpp>
#include <timeseriesmodel.hpp>

namespace timeseries{

  void EstimationEngine::setup(MeanProcessModel* meanModel,
			       VolatilityProcessModel* volModel){
      meanModel_ = meanModel;
      volModel_ = volModel;
    }
}
