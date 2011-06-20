#ifndef timeseriesmodel_hpp
#define timeseriesmodel_hpp

#include <Eigen/Dense>
#include "timeseries.hpp"
#include "typedef.hpp"
#include <map>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <distributions.hpp>
#include <estimationengine.hpp>
//#include <iostream>
namespace timeseries{
  
  static const Size DynamicSize = -1;

  template<Size i>
  class IsDynamic:public boost::false_type{};
  
  template<>
  class IsDynamic<DynamicSize> : public boost::true_type{};
      
  class Model{
    void estimate();
    
  protected:
    ~Model(){}
    class Parameter{
      virtual void reset() = 0;
    };
    class Result{
      std::map<std::string, boost::any> results;
    };
    
  };

  class MeanProcessModel:public Model{    
  protected:
    class Parameter : public Model::Parameter{};
    class Result : public Model::Result{};
    
  };
  
  class VolatilityProcessModel:public Model{
    
  };
  
  template< Size arSize , Size maSize >
  class Arma : public MeanProcessModel{
  public:
    void residual(const RealSeries& data, RealSeries& residual){
      
    }
    
  };

  class ConstantMean : public MeanProcessModel{
    Real mean_;
  public:
    ConstantMean(Real mean = 0.):mean_(mean){}
    void residual(const RealSeries& data, RealSeries& residual){
      residual.resize(data.size());
      residual = (data.array() - mean_).matrix();
    }
    Real& mean(){
      return mean_;
    }
    const Real& mean() const{
      return mean_;
    }
    void estimate(const RealSeries& trainingSample){
      mean_ = trainingSample.sum()/trainingSample.size();
    }
  };
  
  class ZeroMean : public ConstantMean{
  public:
    void residual(const RealSeries& data, RealSeries& residual){
      residual.resize(data.size());
      residual = data;
    }
    void estimate(){}
  };

  template<Size arSize , Size maSize >
  class Garch: public VolatilityProcessModel{
  public:


  };
  
  /*
    template<>
    class Garch<1,1> : public VolatilityProcessModel{
    
    };
  */

  template<Size arSize>
  class Arch : public Garch<arSize, 0>{
  };

  class ConstantVol : public VolatilityProcessModel{
    Real meanVariance_;
  public:
    void estimate(const RealSeries& residual){
      meanVariance_ = residual.squaredNorm()/(residual.size()-1);
    }
    const Real& meanVariance() const{
      return meanVariance_;
    }

    Real& meanVariance() {
      return meanVariance_;
    }
  };

  
  template<typename MeanModel, 
	   typename VolModel,
	   typename Dist = Normal
	   >
  class TimeSeriesModel{
    MeanModel meanProcess_;
    VolModel volProcess_;
    boost::shared_ptr<EstimationEngine > engine_;
  public:
    TimeSeriesModel(){}

    TimeSeriesModel(MeanModel const& meanProcess,
		    VolModel const& volProcess):
      meanProcess_(meanProcess), volProcess_(volProcess){
            
    }

    void setEngine(const boost::shared_ptr<EstimationEngine>& engine){
      engine_ = engine;
    }
    void estimate(const RealSeries& rtn){
      engine_->setup(static_cast<MeanProcessModel*>(&meanProcess_),
		     static_cast<VolatilityProcessModel*>(&volProcess_));
      engine_->performEstimation(rtn);    
    }
    MeanModel& meanProcess(){
      return meanProcess_;
    }

    VolModel& volProcess(){
      return volProcess_;
    }

    /*    
    void forecastVolatility();
    void forecastValue();
    void forecast();
    */
  };
}

#endif
