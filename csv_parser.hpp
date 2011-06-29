#ifndef timeseries_csv_parser_hpp
#define timeseries_csv_parser_hpp

#include <iostream>
#include <algorithm>
#include <fstream>


#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/home/phoenix/statement/sequence.hpp>
#include <boost/spirit/home/phoenix/operator/arithmetic.hpp>

#include <boost/spirit/home/phoenix/core.hpp>
#include <boost/spirit/home/phoenix/function.hpp>
#include <boost/spirit/home/phoenix/operator.hpp>
#include <boost/spirit/home/phoenix/statement.hpp>
//#include <boost/spirit/home/phoenix/statement.hpp>

#include <boost/spirit/include/support_multi_pass.hpp>


namespace timeseries {


namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template<typename Iterator>
struct parse_time_series_csv : boost::spirit::qi::grammar<Iterator, boost::spirit::ascii::space_type>{
  parse_time_series_csv(timeseries::RealSeries& series) : parse_time_series_csv::base_type(lines), lineCount(0), series_(series){
    using boost::spirit::qi::double_;
    using boost::spirit::qi::int_;
    using boost::spirit::qi::on_error;
    using boost::spirit::qi::fail;
    using boost::spirit::qi::_1;
    using boost::phoenix::ref;
    using boost::phoenix::val;
    day = int_;
    month = int_;
    year = int_;
    hour = int_;
    minute = int_;

    open = double_;
    close = double_;


    date = day >> "/" >> month >> "/" >> year;
    time = hour >> ":" >> minute;

    record = date >> "," >> time >> "," >> open
      [
       ref(series_)[ref(lineCount)++] = _1
       //       ref(lineCount) += 1
       //       , std::cout <<lineCount << ": " << val(_1) << "\n"
       ] >> "," >> close;
    lines = +record ;
    
    BOOST_SPIRIT_DEBUG_NODE(day);
    BOOST_SPIRIT_DEBUG_NODE(month);
    BOOST_SPIRIT_DEBUG_NODE(year);
    BOOST_SPIRIT_DEBUG_NODE(hour);
    BOOST_SPIRIT_DEBUG_NODE(minute);
    BOOST_SPIRIT_DEBUG_NODE(open);
    BOOST_SPIRIT_DEBUG_NODE(close);
    BOOST_SPIRIT_DEBUG_NODE(record);
    BOOST_SPIRIT_DEBUG_NODE(lines);

    day.name("day");
    month.name("month");
    year.name("year");
    hour.name("hour");
    minute.name("minute");
    
    open.name("open");
    close.name("close");
    record.name("record");
    lines.name("lines");

    on_error<fail>
      (
       lines,
       std::cout << val("Error!") << std::endl
       );
    
  }
  

  //  (Iterator first, Iterator last, timeseries::RealSeries& series){
  //  using boost::spirit::qi::double_;
  //  using boost::spirit::qi::int_;
  //  using boost::spirit::qi::phrase_parse;
  
  
  //  using boost::spirit::qi::

  qi::rule<Iterator, int(), ascii::space_type> day;
  qi::rule<Iterator, int(), ascii::space_type> month;
  qi::rule<Iterator, int(), ascii::space_type> year;
  qi::rule<Iterator, int(), ascii::space_type> hour;
  qi::rule<Iterator, int(), ascii::space_type> minute;

  qi::rule<Iterator,  ascii::space_type> date;
  qi::rule<Iterator,  ascii::space_type> time;
     
  qi::rule<Iterator, double(), ascii::space_type> open;
  qi::rule<Iterator, double(), ascii::space_type> close;
  
  int lineCount;
  timeseries::RealSeries& series_;
  qi::rule<Iterator, ascii::space_type> record;
  qi::rule<Iterator, ascii::space_type> lines;
};

}


#endif
