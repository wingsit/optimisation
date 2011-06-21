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
    
  public:
    virtual void estimate(const RealSeries&) = 0;
  protected:
    virtual ~Model(){}
    class Parameter{
      virtual void reset() = 0;
    };
    class Result{
      std::map<std::string, boost::any> results;
    };

  };

  class MeanProcessModel:public Model{    
  public:
    virtual void residual(const RealSeries& data, RealSeries& residual) = 0;
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

  class MeanVolatilityModel{
    boost::shared_ptr<MeanProcessModel> meanModel_;
    boost::shared_ptr<VolatilityProcessModel> volatilityModel_;
    boost::shared_ptr<Distribution> distribution_;
    boost::shared_ptr<EstimationEngine> engine_;
  public:
    MeanVolatilityModel(){}
    template<typename M, typename V, typename D>
    MeanVolatilityModel(const M& mean, const V& vol, const D& dis)
      :meanModel_(new M(mean)),
      volatilityModel_(new V(vol)),
      distribution_(new D(dis))
    {}
    void setEngine(const boost::shared_ptr<EstimationEngine>& engine){
      engine_ = engine;
    }
    void estimate(const RealSeries& rtn){
      engine_->setup(meanModel_, 
        volatilityModel_, 
        distribution_);
      engine_->performEstimation(rtn);
    }
    
    const boost::shared_ptr<MeanProcessModel>&  meanModel() const{
      return meanModel_;
    }
    const boost::shared_ptr<VolatilityProcessModel>& volatilityModel() const{
      return volatilityModel_;
    }
    const boost::shared_ptr<EstimationEngine> engine() const{
      return engine_;
    }
    
  };
}

#endif
