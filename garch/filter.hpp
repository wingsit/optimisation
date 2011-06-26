#ifndef timeseries_utility_hpp
#define timeseries_utility_hpp

#include <Eigen/Dense>
#include <iostream>
namespace timeseries {

using Eigen::DenseBase;
/*!
  solving for response y[i] = c + x[t] + sum(a[i] x[t-i], i = 1..., p) + sum(b[i] y[t-i], i = 1,..., q)
  \todo Only naive implementation now. More or less quadratic. Maybe using FFT/Z-transform will do some good. Need to think about the maths.
 */
template<
typename Derived1,
         typename Derived2,
         typename Derived3,
         typename Derived4
         >
void filter(const DenseBase<Derived1>& x,
            const DenseBase<Derived2>& a,
            const DenseBase<Derived3>& b,
            const DenseBase<Derived4>& y_,
            const double c = 0.,
            const double backfill = 0.,
            bool instantaneousX = true) {
    DenseBase<Derived4>& y = const_cast<DenseBase<Derived4>&>(y_);
    size_t ma = a.rows();
    size_t mb = b.rows();

    size_t mx = x.rows();
    size_t my = y.rows();
    assert(mx==my);

    y.topRows(std::max(ma, mb)) = DenseBase<Derived1>::Constant(std::max(ma, mb),y.cols(), backfill);

    if(instantaneousX) {
        for(size_t i = std::max(ma, mb); i < y.rows(); ++i) {
            y.row(i) =
                c
                + x.row(i)
                +(b.colwise().reverse() * x.block(i-mb+1, 0, b.rows(), b.cols())).colwise().sum()
                +(a.colwise().reverse() * y.block(i-ma, 0, a.rows(), a.cols())).colwise().sum();
        }
    } else {
        for(size_t i = std::max(ma, mb); i < y.rows(); ++i) {
            y.row(i) =
                c
                +(b.colwise().reverse() * x.block(i-mb+1, 0, b.rows(), b.cols())).colwise().sum()
                +(a.colwise().reverse() * y.block(i-ma, 0, a.rows(), a.cols())).colwise().sum();
        }
    }
}
/*
template<
  typename Derived1,
  typename Derived2,
  typename Derived3,
  typename Derived4>
void filter_naive(const DenseBase<Derived1>& x,
	    const DenseBase<Derived2>& a,
	    const DenseBase<Derived3>& b,
	    const DenseBase<Derived4>& y_){
  DenseBase<Derived4>& y = const_cast<DenseBase<Derived4>&>(y_);
  size_t ma = a.rows();
  size_t mb = b.rows();

  size_t mx = x.rows();
  size_t my = y.rows();
  assert(mx==my);

  y.topRows(std::max(ma, mb)) = DenseBase<Derived1>::Zero(std::max(ma, mb),y.cols());
  for(size_t i = std::max(ma, mb); i < y.rows(); ++i){
    for( size_t j = 0; j < y.cols() ; ++j){
y(i, j) = x(i, j);
for( size_t k = 0; k < mb; ++k){
  y(i, j) -= b(k, j) * x(i-mb, j);
}
for( size_t k = 0; k < ma; ++k){
  y(i, j) += a(k, j) * y(i-ma, j);
}
    }
  }
}
*/
}
#endif
