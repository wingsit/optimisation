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
  //  parameter << 5.82589081260572e-9, 0.7, 0.3;
  parameter[0] = 5.825e-9;
  parameter[1] = 0.7;
  parameter[2] = 0.3;
  
  // parameter << 0, 0.5, 0.5;
  model.parameters() = parameter;
  model.setParameters();
  std::cout << model.loglikelihood(sample) << std::endl;
  
  NumericalDerivative<LikelihoodHelper<TimeSeriesModel<SampleMean, Garch<1,1> ,Normal> > >
    objective(model, sample);

  DEBUG_PRINT(objective(parameter));
  
  std::cout << BFGSMethod<ArmijoLineSearch>()(objective, parameter) << std::endl;
  
  //  std::cout << mean( residuals) << std::endl;
  
}

