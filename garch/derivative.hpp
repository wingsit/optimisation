#ifndef DERIVATIVE_HPP
#define DERIVATIVE_HPP

#include <Eigen/Dense>
#include <limits>
#include <boost/utility.hpp>
enum DifferenceMethod {
    Central,
    Forward
};
/*
template<typename T, T t>
struct has_gradient : public boost::true_type{
  static const bool value = true;
};
*/

template<typename Functor_, DifferenceMethod mode = Forward>
class NumericalDerivative : public Functor_ {
public:
    typedef Functor_ function_type;
    typedef unsigned index_type;
    typedef typename Eigen::VectorXd vector_type;
    typedef typename vector_type::Scalar scalar_type;
    typedef typename Eigen::Matrix<scalar_type, Eigen::Dynamic, Eigen::Dynamic> matrix_type;

    void defaultEpsilon(scalar_type ep = 0.) {
      
      epsilon = (mode==Forward)?
	//Numerical Optimization by Nocedal Page 168 Formula 7.6
	std::max(ep, std::sqrt(std::numeric_limits<scalar_type>::epsilon()))
	//Numerical Optimization by Nocedal Page 169
	:std::max(ep, std::pow(std::numeric_limits<scalar_type>::epsilon(), 2./3.));
      
      //      epsilon = 1e-6;
    }

    NumericalDerivative(const function_type& f_, scalar_type ep = 0.):
        Functor_(f_) {
        defaultEpsilon(ep);
    }

    NumericalDerivative(scalar_type ep = 0.):function_type() {
        defaultEpsilon(ep);
    }

    template<typename T0>
    NumericalDerivative(const T0& t0, scalar_type ep = 0.):function_type(t0) {
        defaultEpsilon(ep);
    }

    template<typename T0, typename T1>
    NumericalDerivative(const T0& t0, const T1& t1):function_type(t0, t1) {
        defaultEpsilon();
    }

    template<typename T0, typename T1, typename T2>
    NumericalDerivative(const T0& t0, const T1& t1, const T2& t2)
        :function_type(t0, t1, t2) {
        defaultEpsilon();
    }

    template<typename T0, typename T1, typename T2, typename T3>
    NumericalDerivative(const T0& t0, const T1& t1, const T2& t2, const T3& t3)
        :function_type(t0, t1, t2,t3) {
        defaultEpsilon();
    }





    //original function forward
    inline scalar_type operator()(const vector_type& x) const {
        return function_type::operator()(x);
    }

    /*
    inline scalar_type operator()(vector_type x,
    			index_type i,
    			typename boost::enable_if<has_gradient<scalar_type(function_type::*)(vector_type, index_type),
    			&function_type::operator()> >::type* = 0){
      return 0;
    }
    */
    //gradient
    /// \todo http://groups.google.com/group/comp.lang.c++.moderated/msg/e5fbc9305539f699
    inline scalar_type operator()(vector_type x, index_type i) const{
      scalar_type centre, backstep, forwardstep, epsilon_ = epsilon;// * x[i];
      /*
      if(epsilon_ == 0.){
	epsilon_ = epsilon;
      }
      */
        switch(mode) {
        case Forward:
            centre = function_type::operator()(x);
            x[i]+=epsilon_;
            return (function_type::operator()(x) - centre)/epsilon_;
        case Central:
            x[i]+=epsilon_;
            forwardstep = function_type::operator()(x);
            x[i]-=2.*epsilon_;
            backstep=function_type::operator()(x);
            return 0.5*(forwardstep - backstep)/epsilon_;
        default:
            assert(false);
            return 0;
        }
    }



    inline size_t operator()(const vector_type& x, vector_type& gradient) const{
        gradient.resize(x.size());
        for(index_type i = 0; i < x.size(); ++i) {
            gradient[i] = (*this)(x,i);
        }
        return x.size() * 2;
        //    return gradient;
    }



    //hessian
    inline scalar_type operator()(vector_type x, index_type i, index_type j)const {
        scalar_type topleft, topright, bottomleft, bottomright;
        scalar_type result;
	scalar_type epsilon_ = std::max(10e-6, epsilon);// * std::min(x[i], x[j]);
        switch(mode) {
        case Forward:
            bottomleft = function_type::operator()(x);
            x[i] += epsilon_;
            topleft = function_type::operator()(x);
            x[j] += epsilon_;
            topright = function_type::operator()(x);
            x[i] -= epsilon_;
            bottomright = function_type::operator()(x);
            return result = (topright + bottomleft - topleft - bottomright) / (epsilon_ * epsilon_);
        case Central:
            x[i] += epsilon_;
            x[j] += epsilon_;
            topright = function_type::operator()(x);
            x[i] -= 2*epsilon_;
            topleft = function_type::operator()(x);
            x[j] -= 2*epsilon_;
            bottomleft = function_type::operator()(x);
            x[i] += 2*epsilon_;
            bottomright = function_type::operator()(x);
            return result = 0.25*(topright + bottomleft - topleft - bottomright) / (epsilon_ * epsilon_);
        default:
            assert(false);
            return 0;
        }
    }
    inline size_t operator()(const vector_type& x, matrix_type& hessian) const {
        index_type n = x.size();
        hessian.resize(n, n);
        for(index_type i = 0; i < n; ++i) {
            for(index_type j = 0; j <= i; ++j) {
                if(i == j) hessian(i, i) = (*this)(x,i,i);
                else hessian(i, j) = hessian(j, i) = (*this)(x, i, j);
            }
        }
        return n * (n+1) * 2;
    }

private:
    //  function_type f;
    scalar_type epsilon;
};

#endif
