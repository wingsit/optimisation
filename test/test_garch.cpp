#include <testutility.hpp>
#include <normal.hpp>
#include <timeseriesmodel.hpp>
#include <samplemean.hpp>
#include <garchpq.hpp>
#include <statisticalfunctions.hpp>
#include <fstream>
#include <csv_parser.hpp>
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

  sample = sample.reverse();
  sample = log_return(sample);
  TimeSeriesModel<SampleMean, Garch<1, 1>, Normal> model;
  RealSeries  residuals(n);
  
  std::cout << "Return Sample Mean: " << mean(sample) << "\n";
  std::cout << "Return Sample Stdv: " << stdv(sample) << "\n";

  model.setSampleMean(sample);
  ParameterArray parameter(3);
  parameter << 2.59048e-7, 0.7, 0.3;
  model.parameters() = parameter;
  
  std::cout << model.loglikelihood(sample) << std::endl;

  
  //  std::cout << mean( residuals) << std::endl;
  
}

