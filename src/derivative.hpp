#ifndef DERIVATIVE_HPP
#define DERIVATIVE_HPP

#include <Eigen/Dense>
#include <limits>
#include <boost/utility.hpp>
enum DifferenceMethod{
  Central,
  Forward
};




template<typename T, T t>
struct has_gradient : public boost::true_type{
  static const bool value = true;
};


template<typename Functor_, DifferenceMethod mode = Forward>
class NumericalDerivative : public Functor_{
public:
  typedef Functor_ function_type;
  typedef unsigned index_type;
  typedef typename function_type::VectorType vector_type;
  typedef typename vector_type::Scalar scalar_type;
  typedef typename Eigen::Matrix<scalar_type, Eigen::Dynamic, Eigen::Dynamic> matrix_type;

  NumericalDerivative(const function_type& f_, scalar_type ep = 0.):
  f(f_), epsilon(
    (mode==Forward)?
    std::max(ep, std::sqrt(std::numeric_limits<scalar_type>::epsilon())) //Numerical Optimization by Nocedal Page 168 Formula 7.6
    :std::max(ep, std::pow(std::numeric_limits<scalar_type>::epsilon(), 2/3)))//Numerical Optimization by Nocedal Page 169
  {}

  NumericalDerivative(scalar_type ep = 0.):
  f(Functor_()), epsilon(
    (mode==Forward)?
    std::max(ep, std::sqrt(std::numeric_limits<scalar_type>::epsilon()))
    :std::max(ep, std::pow(std::numeric_limits<scalar_type>::epsilon(), 2/3)))
  {}


  //original function forward
  inline scalar_type operator()(const vector_type& x){
        return f(x);
  }
  
  inline scalar_type operator()(vector_type x, 
				index_type i, 
				typename boost::enable_if<has_gradient<scalar_type(function_type::*)(vector_type, index_type), 
				&function_type::operator()> >::type* = 0){
    return 0;
  }
  /*
  //gradient
  inline scalar_type operator()(vector_type x, index_type i,
				typename boost::disable_if<has_gradient<scalar_type(function_type::*)(vector_type, index_type), 
				&function_type::operator()(vector_type, index_type)> >::type* = 0){
    scalar_type centre, backstep, forwardstep;
    switch(mode){
    case Forward:
      centre = f(x);
      x[i]+=epsilon;
      return (f(x) - centre)/epsilon;
    case Central:
      x[i]+=epsilon;
      forwardstep = f(x);
      x[i]-=2.*epsilon;
      backstep=f(x);
      return 0.5*(forwardstep - backstep)/epsilon;
    default:
      assert(false);
      return 0;
    }
  }


  */
  inline vector_type& operator()(const vector_type& x, vector_type& gradient){
    gradient.resize(x.size());
    for(index_type i = 0; i < x.size(); ++i){
      gradient[i] = (*this)(x,i);
    }
    return gradient;
  }



  //hessian
  inline scalar_type operator()(vector_type x, index_type i, index_type j){
    scalar_type topleft, topright, bottomleft, bottomright;
//    scalar_type left, top, bottom, right;
    switch(mode){
    case Forward:
      bottomleft = f(x);
      x[i] += epsilon;
      topleft = f(x);
      x[j] += epsilon;
      topright = f(x);
      x[i] -= epsilon;
      bottomright = f(x);
      return (topright + bottomleft - topleft - bottomright) / (epsilon * epsilon);
    case Central:
      x[i] += epsilon;
      x[j] += epsilon;
      topright = f(x);
      x[i] -= 2*epsilon;
      topleft = f(x);
      x[j] -= 2*epsilon;
      bottomleft = f(x);
      x[i] += 2*epsilon;
      bottomright = f(x);
      return 0.25*(topright + bottomleft - topleft - bottomright) / (epsilon * epsilon);
    default:
      assert(false);
      return 0;
    }
  }
  inline matrix_type& operator()(const vector_type& x, matrix_type& hessian){
    index_type n = x.size();
    hessian.resize(n, n);
    for(index_type i = 0; i < n; ++i){
      for(index_type j = 0; j <= i; ++j){
        if(i == j) hessian(i, i) = (*this)(x,i,i);
        else hessian(i, j) = hessian(j, i) = (*this)(x, i, j);
      }
    }
    return hessian;
  }

private:
  function_type f;
  const scalar_type epsilon;
};

#endif
