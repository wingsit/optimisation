#ifndef timeseriesmodel_hpp
#define timeseriesmodel_hpp

#include <Eigen/Dense>
#include "timeseries.hpp"
#include "typedef.hpp"
#include <map>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <distributions.hpp>
//#include <iostream>
namespace timeseries{
  
  static const Size DynamicSize = -1;

  template<Size i>
  class IsDynamic:public boost::false_type{};
  
  template<>
  class IsDynamic<DynamicSize> : public boost::true_type{};
    

  class EstimationEngine{
    class Arguments;
    class Result;
    //    virtual Arguments* getParameters() = 0;
    //    virtual Result getResults() = 0;
    //    virtual void reset() = 0;
    //    virtual void estimate() = 0;
  };
  


  template<typename MeanModel, typename VarianceModel>
  class GenericEstimationEngine : public EstimationEngine{};



  class Model{
    
    //    boost::shared_ptr<EstimationEngine> engine_;
    virtual void estimate();
    virtual ~Model(){}
  protected:
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
    //    Arma():coefficients_(arSize+maSize+1){}
    
  };

  class ConstantMean : public MeanProcessModel{
    Real mean_;
  public:
    class Parameter : public MeanProcessModel::Parameter{
      Real mean;
      void reset(){ mean = 0;}
    };

    class Result : public MeanProcessModel::Result{};
  };
  
  class ZeroMean : public ConstantMean{
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
  public:

  };

  
  template<typename MeanModel, 
	   typename VolModel,
	   typename Dist = Normal
	   >
  class TimeSeriesModel{
    MeanModel meanProcess_;
    VolModel volProcess_;
  public:
    TimeSeriesModel(){}
    /*
      TimeSeriesModel(MeanProcessModel const& meanProcess,
      VolatilityProcessModel const& volProcess){
            
      }
    */
    void estimate();
    void forecastVolatility();
    void forecastValue();
    void forecast();
  };
}

#endif
