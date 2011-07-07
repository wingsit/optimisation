#include <testutility.hpp>
#include <boost/progress.hpp>

using Eigen::MatrixXd;
using Eigen::MatrixBase;
using boost::timer;


const size_t n = 200;

MatrixXd f1() {
    MatrixXd X(n, n);
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            X(i, j) = i + j;
        }
    }
    return X;
}

MatrixXd& f1(MatrixXd & X) {
    X.resize(n, n);
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            X(i, j) = i + j;
        }
    }
}



BOOST_AUTO_TEST_CASE(EIGEN_TYPE_MOVE_SEMANTIC) {

    timer t0;


    t0.restart();
    for(int i = 0; i < n; ++i) {
        auto X = f1();
        auto Y(f1());
        X = std::move(Y);
    }
    std::cout << t0.elapsed() << std::endl;
    t0.restart();

    for(int i = 0; i < n; ++i) {
        MatrixXd X(n, n);
        f1(X);
        MatrixXd Y(n,n);
        f1(Y);
        X = std::move(Y);
    }
    std::cout << t0.elapsed() << std::endl;
}

template<typename Derived1, typename Derived2>
auto multi(const MatrixBase<Derived1>& A,
           const MatrixBase<Derived2>& B) -> decltype( (A.transpose() * A).ldlt().solve(B))
{
    return (A.transpose() * A).ldlt().solve(B);
}

BOOST_AUTO_TEST_CASE(EIGEN_TYPE_DECLTYPE) {
    size_t n = 100;
    MatrixXd X = MatrixXd::Random(n, n), Y = MatrixXd::Random(n, n);
    auto C(multi(X, Y));
    //MatrixXd C(multi(MatrixXd::Random(n, n),MatrixXd::Random(n, n)));
    std::cout << C << std::endl;
    //  C(0,0);
}
