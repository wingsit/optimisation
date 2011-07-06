#ifndef timeseries_errorhandling_hpp
#define timeseries_errorhandling_hpp
#include <iomanip>
#define DEBUG 0

#if DEBUG
#define DEBUG_PRINT(x) std::cout << __FILE__ << ":" << __FUNCTION__<< ":" << __LINE__ << " " << #x << ": \n" << x << "\n";		
#else
#define DEBUG_PRINT(x) (x);
#endif

namespace timeseries {

}


#endif
