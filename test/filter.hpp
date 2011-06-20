#ifndef timeseries_utility_hpp
#define timeseries_utility_hpp

#include <Eigen/Dense>
#include <iostream>
namespace timeseries{

  using Eigen::DenseBase;  
  /*!
    solving for y[i] = x[i] - b^T * x[i:i-nb] + a^T & y[y-1:y-1-na]
   */
  template<
    typename Derived1, 
    typename Derived2,
    typename Derived3,
    typename Derived4>
  void filter(const DenseBase<Derived1>& x,
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
    if(mb == 0){
      for(size_t i = std::max(ma, mb); i < y.rows(); ++i){
	y.row(i) = 
	  x.row(i)
	  +((a.colwise().reverse()) * y.block(i-ma, 0, a.rows(), a.cols())).colwise().sum();
      }
    }
    else if(ma == 0){
      for(size_t i = std::max(ma, mb); i < y.rows(); ++i){
	y.row(i) = 
	  x.row(i)
	  -(b.block(0,0, b.rows(), b.cols()) * x.block(i-mb+1, 0, b.rows(), b.cols())).colwise().sum();
      }
    }else{
      for(size_t i = std::max(ma, mb); i < y.rows(); ++i){
	y.row(i) = 
	  x.row(i)
	  -(b.block(0,0, b.rows(), b.cols()) * x.block(i-mb+1, 0, b.rows(), b.cols())).colwise().sum()
	  +(a.block(0,0, a.rows(), a.cols()) * y.block(i-ma, 0, a.rows(), a.cols())).colwise().sum();
      }
    }
  }

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

}
#endif
