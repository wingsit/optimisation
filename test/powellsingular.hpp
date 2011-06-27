#include <typedef.hpp>

class PowellSingular{
public:
  Real operator()(const RealSeries& x) const{
    return std::pow(x[0] + 10. * x[1], 2.) + 5. * std::pow(x[2] - x[3], 2.) 
      + std::pow(x[1] - 2. * x[2], 4.) + 10. * pow(x[0] - x[3], 4.);
  }
  RealSeries start() const{
    RealSeries start(4);
    start << 3., -1., 0., 1.;
    return start;
  }
  RealSeries criticalPoint() const{
    return RealSeries::Zero(4);
  }
  Real min() const{
    return 0.;
  }
  size_t size() const{
    return 4;
  }
};
