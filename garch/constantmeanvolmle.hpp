#ifndef timeseries_constantmeanvolmle_hpp
#define timeseries_constantmeanvolmle_hpp

#include <estimationengine.hpp>


class ConstantMeanVolMLE : public EstimationEngine{
  RealSeries parameters_;
  struct ObjectiveHelper{
    boost::shared_ptr<MeanProcessModel> meanModel_;
    boost::shared_ptr<VolatilityProcessModel> volModel_;
    boost::shared_ptr<Distribution> dist_;
    ObjectiveHelper(boost::shared_ptr<MeanProcessModel> meanModel,
		    boost::shared_ptr<VolatilityProcessModel> volModel,
		    boost::shared_ptr<Distribution> dist,
		    const RealSeries& rtn)
      :meanModel_(meanModel),
       volModel_(volModel),
       dist_(dist){}
    Real operator(const RealSeries& rtn){
      RealSeries residual(rtn.size()), vols(rtn.size());
      meanModel_->residual(rtn, residual);
      volModel_->volatilities(residual, vols);
      residual = (residual.array()/vols.array()).matrix();
      distribution->pdf(residual);
      return residual.array().log().sum();
    }
  };
public:
  virtual void setup( boost::shared_ptr<MeanProcessModel> meanModel,
		      boost::shared_ptr<VolatilityProcessModel> volModel,
		      boost::shared_ptr<Distribution> dist){
    EstimationEngine::setup(meanModel,
			    volModel,
			    dist);
    Size totalLen = meanModel_->parameterLength()+
      volModel_->parameterLength()+
      distribution_->parameterLength();
    parameters_.resize(totalLen);
    parameters_.segment(0, meanModel_->parameterLength());
    parameters_.segment(meanModel_->parameterLength(), volModel_->parameterLength());
    parameters_.segment(volModel_->parameterLength(), distribution_->parameterLength());     
  }
  virtual void performEstimation(const RealSeries& rtn){
  }
};

#endif
