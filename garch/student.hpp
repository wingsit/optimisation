#ifndef timeseries_student_hpp
#define timeseries_student_hpp

#include <typedef.hpp>
#include <distribution.hpp>

namespace timeseries {


class GammaFunction : public std::unary_function<Real, Real> {
public:
    template<typename T>
    T logValue(const T& x) const {
        Real temp = x + 5.5;
        temp -= (x + 0.5) * std::log(temp);
        Real ser = 1.000000000190015;
        ser += c1_ / (x + 1.0);
        ser += c2_ / (x + 2.0);
        ser += c3_ / (x + 3.0);
        ser += c4_ / (x + 4.0);
        ser += c5_ / (x + 5.0);
        ser += c6_ / (x + 6.0);
        return  -temp + std::log(2.5066282746310005 * ser / x);
    }
private:
    static const Real c1_, c2_, c3_, c4_, c5_, c6_;

};

const Real GammaFunction::c1_ = 76.18009172947146;
const Real GammaFunction::c2_ = -86.50532032941677;
const Real GammaFunction::c3_ = 24.01409824083091;
const Real GammaFunction::c4_ = -1.231739572450155;
const Real GammaFunction::c5_ = 0.1208650973866179e-2;
const Real GammaFunction::c6_ = -0.5395239384953e-5;

//! Student t-distribution
/*! Probability density function for \f$ n \f$ degrees of freedom
  (see mathworld.wolfram.com or wikipedia.org):
  \f[
  f(x) = \frac {\Gamma\left(\frac{n+1}{2}\right)} {\sqrt{n\pi}
  \, \Gamma\left(\frac{n}{2}\right)}\:
  \frac {1} {\left(1+\frac{x^2}{n}\right)^{(n+1)/2}}
  \f]
*/
struct Student {
public:
    Student(Real n = 2) : n_(n) {}
    template<typename T>
    Real pdfImpl(T& x) const {
        static GammaFunction G;
        Real g1 = std::exp (G.logValue(0.5 * (n_ + 1)));
        Real g2 = std::exp (G.logValue(0.5 * n_));
        T power = (1. + x * x / n_).pow( 0.5 * (n_ + 1));
        x =  1./g1 * (g2 * power * std::sqrt (M_PI * n_));
    }
private:
    Real n_;
};

}


#endif
