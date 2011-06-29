#ifndef timeseries_errorhandling_hpp
#define timeseries_errorhandling_hpp

#define DEBUG 0

#if DEBUG
#define DEBUG_PRINT(x) std::cout << #x << ": \n" << x << "\n";		
#else
#define DEBUG_PRINT(x) (x);
#endif

namespace timeseries {

}


#endif
