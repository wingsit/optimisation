#include <testutility.hpp>
#include <typedef.hpp>
#include <csv_parser.hpp>



BOOST_AUTO_TEST_CASE(CSV_PARSER) {
    using namespace timeseries;

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
    RealSeries series(8000);

    typedef std::istreambuf_iterator<char> base_iter_type;
    boost::spirit::multi_pass<base_iter_type> first =
        boost::spirit::make_default_multi_pass(base_iter_type(ifFile));

    boost::spirit::multi_pass<base_iter_type> end =
        boost::spirit::make_default_multi_pass(base_iter_type());

    parse_time_series_csv< boost::spirit::multi_pass<base_iter_type> > csvGrammar(series);

    //  while(first != end)
    boost::spirit::qi::phrase_parse(first, end, csvGrammar, ascii::space);
    //series);
    /*
    for(int i = 0; i < series.size(); ++i){
      std::cout << i << " " << series[i] << std::endl;
    }
    */
}
