#include <testutility.hpp>
#include <derivative.hpp>
#include <typedef.hpp>
#include <iostream>
using Eigen::VectorXd;
using Eigen::MatrixXd;
using namespace timeseries;

struct SymmetricInnerProduct {
    MatrixXd A_;
    SymmetricInnerProduct(const MatrixXd& A):A_(A) {
    }
    Real operator()(const VectorXd& v) const {
        return v.transpose() * A_ * v;
    }
    void derivative(const VectorXd& v, VectorXd& gradient) {
        gradient= 2. * A_ * v;
    }
    void hessian(const VectorXd&, MatrixXd& hessian) {
        hessian = 2. * A_;
    }
};

BOOST_AUTO_TEST_CASE(FIRST_ORDER) {


    size_t n = 10;

    MatrixXd A = MatrixXd::Random(n, n), hessian1(n, n), hessian2(n, n);
    A = A.transpose()*A;


    VectorXd x = VectorXd::Random(n), gradient1(n), gradient2(n);
    NumericalDerivative<SymmetricInnerProduct> fun(A);
    SymmetricInnerProduct trueFun(A);


    //  std::cout << "fun(x)" << fun(x) << std::endl;
    DEBUG_PRINT(fun(x));
    DEBUG_PRINT(trueFun(x));
    fun(x, gradient1);
    DEBUG_PRINT(gradient1);
    trueFun.derivative(x, gradient2);
    DEBUG_PRINT(gradient2);
    DEBUG_PRINT(gradient1 - gradient2);
    DEBUG_PRINT(A);
    fun(x, hessian1);
    DEBUG_PRINT(hessian1);
    trueFun.hessian(x, hessian2);
    DEBUG_PRINT(hessian2);
    DEBUG_PRINT(hessian1-hessian2);
    VectorXd direction = hessian1.ldlt().solve(gradient1);
    DEBUG_PRINT(direction - hessian2.ldlt().solve(gradient2));
    DEBUG_PRINT(x + direction);
    DEBUG_PRINT(fun(x-direction));
}
