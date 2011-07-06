#include <testutility.hpp>
#include <normal.hpp>
#include <timeseriesmodel.hpp>
#include <samplemean.hpp>
#include <garchpq.hpp>
#include <statisticalfunctions.hpp>
#include <fstream>
#include <csv_parser.hpp>
#include <derivative.hpp>
#include <bfgsmethod.hpp>
#include <newtonmethod.hpp>
#include <armijo.hpp>
#include <data.hpp>

namespace timeseries{

  template<>
  class LikelihoodHelper <TimeSeriesModel<SampleMean, Garch<1, 1>, Normal> >: public std::unary_function<Eigen::VectorXd, Real> {
    typedef TimeSeriesModel<SampleMean, Garch<1,1>, Normal> T;
    mutable T  model_;
    RealSeries data_;
  public:
    typedef T ModelType;
    LikelihoodHelper(const T& model,
                     const RealSeries data): model_(model), data_(data) {}

    Real operator()(const Eigen::VectorXd& parameters) const {
      model_.parameters() = parameters.array();
      model_.setParameters();
      RealSeries residuals(data_.size());
      RealSeries variances(data_.size());
	
      model_.residuals(data_, residuals);
      model_.variances(residuals, variances);
      //	DEBUG_PRINT(variances.log().sum());
      //	DEBUG_PRINT((residuals.pow(2.)/variances).sum());
      static double count = 0;
      //      return  variances.log().sum() + (residuals.pow(2.)/variances).sum() - 1e6 * std::pow(0.8, count++)* (std::log(parameters[0]) + std::log(1-parameters[0])); 
	
      return model_.loglikelihood(data_) - 1e10 * std::pow(0.95, count++)* (std::log(parameters[0]) + std::log(1.-parameters[0])); 
    }
    T model() {
      return model_;
    }
    const ParameterArray& parameters() const {
      return model_.parameters();
    }

    ParameterArray& parameters() {
      return model_.parameters();
    }

    Size operator()(const Eigen::VectorXd& parameters, Eigen::VectorXd& gradient) const{
      model_.parameters() = parameters.array();
      model_.setParameters();
      gradient.resize(parameters.size());
      Size n = data_.size();
      RealSeries residuals(n), variances(n);
      model_.residuals(data_, residuals);
      model_.variances(residuals, variances);
  
      Eigen::MatrixXd g(n, 3);
  
      g(0,0) = 1.;
      g(0,1) =   variance(data_.segment(0, 20));
      g(0,2) =   variance(data_.segment(0, 20));
      Real gamma = parameters[0];
      Real alpha = parameters[1];
      Real beta = parameters[2];

      for(Size i = 1; i < n; ++i){
	g(i, 0) = 1. + beta * g(i-1, 0);
	g(i, 1) = std::pow(data_[i-1], 2.) + beta * g(i-1, 1);
	g(i, 2) = variances[i-1] + beta * g(i-1, 2);
      }
  
      Eigen::VectorXd l = -(0.5 * variances.inverse() * ( data_.pow(2) / variances - 1.));
      gradient = (l.transpose() * g).transpose();
    }
  };



}

BOOST_AUTO_TEST_CASE(GARCH11){
  using namespace timeseries;
  
  size_t n = 8000;

  std::ifstream ifFile("../test/SPY_100_5.csv");
  {
    //chop the first two line
    std::string l;
    std::getline(ifFile, l);
    std::getline(ifFile, l);
  }


  /*
    size_t lineCount = std::count(std::istreambuf_iterator<char>(ifFile),
    std::istreambuf_iterator<char>(), '\n');
    std::cout << lineCount << std::endl;
  */
  RealSeries sample(8000);

  typedef std::istreambuf_iterator<char> base_iter_type;
  boost::spirit::multi_pass<base_iter_type> first = 
    boost::spirit::make_default_multi_pass(base_iter_type(ifFile));

  boost::spirit::multi_pass<base_iter_type> end = 
    boost::spirit::make_default_multi_pass(base_iter_type());  

  parse_time_series_csv< boost::spirit::multi_pass<base_iter_type> > csvGrammar(sample);

  //  while(first != end)
  boost::spirit::qi::phrase_parse(first, end, csvGrammar, ascii::space);
  //series);
  /*  
      for(int i = 0; i < series.size(); ++i){
      std::cout << i << " " << series[i] << std::endl;
      }
  */

  sample.reverseInPlace();
  sample = log_return(sample);
  TimeSeriesModel<SampleMean, Garch<1, 1>, Normal> model;
  RealSeries  residuals(n);
  
  std::cout << "Return Sample Mean: " << mean(sample) << "\n";
  std::cout << "Return Sample Stdv: " << stdv(sample) << "\n";

  model.setSampleMean(sample);
  ParameterArray parameter(3);
  parameter << 5.82589081260572e-9, 0.7, 0.3;
  model.parameters() = parameter;
  model.setParameters();
  std::cout << model.loglikelihood(sample) << std::endl;
  
  NumericalDerivative<LikelihoodHelper<TimeSeriesModel<SampleMean, Garch<1,1> ,Normal> > >
    objective(model, sample);

  std::cout << BFGSMethod<ArmijoLineSearch>()(objective, parameter) << std::endl;
  
  //  std::cout << mean( residuals) << std::endl;
  
}


BOOST_AUTO_TEST_CASE(GARCH112){
  using namespace timeseries;
  
  size_t n = 20974;

  RealSeries sample(7520);
  
  std::copy(spx_daily_return + n - 7520, spx_daily_return + n, sample.data());


  
  TimeSeriesModel<SampleMean, Garch<1, 1>, Normal> model;
  RealSeries  residuals(n);
  
  std::cout << "Return Sample Mean: " << mean(sample) << "\n";
  std::cout << "Return Sample Stdv: " << stdv(sample) << "\n";

  model.setSampleMean(sample);
  ParameterArray parameter(3);
  parameter << 5.82589081260572e-9, 0.7, 0.3;
  //  parameter << 0.00014289, 0.9156, 0.0755;
  //  parameter << 0.00014289, 0.92, 0.077;

  
  //  parameter << 0., 0.7, 0.3;
  model.parameters() = parameter;
  model.setParameters();
  std::cout << model.loglikelihood(sample) << std::endl;
  
  NumericalDerivative<LikelihoodHelper<TimeSeriesModel<SampleMean, Garch<1,1> ,Normal> > >
    //  LikelihoodHelper<TimeSeriesModel<SampleMean, Garch<1,1> ,Normal> >
    objective(model, sample);
  LikelihoodHelper<TimeSeriesModel<SampleMean, Garch<1,1> ,Normal> >
    objective2(model, sample);

  DEBUG_PRINT(objective(parameter));
  
  std::cout << BFGSMethod<ArmijoLineSearch>()(objective, parameter) << std::endl;
  //  std::cout << BFGSMethod<ArmijoLineSearch>()(objective2, parameter) << std::endl;
  /*
    Eigen::VectorXd grad;
    objective(parameter, grad);
    DEBUG_PRINT(grad);
    objective2(parameter, grad);
    DEBUG_PRINT(grad);
  */
  //  std::cout << mean( residuals) << std::endl;
  
}

