#ifndef timeseries_timeseriesmodel_hpp
#define timeseries_timeseriesmodel_hpp

#include <meanprocess.hpp>
#include <varianceprocess.hpp>
#include <statisticalfunctions.hpp>
#include <typedef.hpp>
#include <errorhandling.hpp>
namespace timeseries {

template<typename M> class MeanProcess;
template<typename V> class VarianceProcess;
template<typename D> class Distribution;

template<class M, class V, class D>
struct  TimeSeriesModel :
    public MeanProcess<M>,
    public VarianceProcess<V>,
    public Distribution<D> {
    TimeSeriesModel() {
        index[0] = 0;
        index[1] = this->meanProcessParameterSize();
        index[2] = index[1] + this->varianceProcessParameterSize();
        index[3] = index[2] + this->distributionParameterSize();
        parameters_.resize(index[3]);
    }

    Real loglikelihood(const RealSeries& data) {
        RealSeries residuals(data.size());
        RealSeries variances(data.size());
        this->residuals(data, residuals);
        this->variances(residuals, variances);
	/*
	DEBUG_PRINT(mean(data));
	DEBUG_PRINT(stdv(data));
	DEBUG_PRINT(mean(residuals));
	DEBUG_PRINT(stdv(residuals));
	DEBUG_PRINT(data.head(10).transpose());
	DEBUG_PRINT(data.size());
	//	DEBUG_PRINT(data.transpose());
	DEBUG_PRINT(data.tail(10).transpose());
	DEBUG_PRINT(residuals.head(10).transpose());
	DEBUG_PRINT(residuals.tail(10).transpose());
	DEBUG_PRINT(variances.head(10).transpose());
	DEBUG_PRINT(variances.tail(10).transpose());
	//	DEBUG_PRINT((residuals/variances.sqrt()).tail(10).transpose());
	*/
        residuals /= variances.sqrt();
	DEBUG_PRINT(residuals.head(10).transpose());
	DEBUG_PRINT(residuals.tail(10).transpose());
        this->pdf(residuals);
	DEBUG_PRINT(residuals.head(10).transpose());
	DEBUG_PRINT(residuals.tail(10).transpose());
	//	DEBUG_PRINT(residuals.log().tail(10).transpose());
	return -(residuals<1e-6).select(1e-6, residuals).log().sum();
	//        return -residuals.log().sum();

        //    this->residuals(const RealSeries& rtn, RealSeries& residuals);
        //    this->variances(const RealSeries& residuals, VarianceSeries& variances){);
    }

    mutable ParameterArray parameters_;
    Size index[4];

    ParameterArray& parameters() {
        return parameters_;
    }

    const ParameterArray& parameters() const {
        return parameters_;
    }

    void fetchParameters() {
        this->getMeanProcessParameters(parameters_.segment(index[0], this->meanProcessParameterSize()));
        this->getVarianceProcessParameters(parameters_.segment(index[1], this->varianceProcessParameterSize()));
        this->getDistributionParameters(parameters_.segment(index[2], this->distributionParameterSize()));
    }
    void setParameters() {
        this->setMeanProcessParameters(parameters_.segment(index[0], this->meanProcessParameterSize()));
        this->setVarianceProcessParameters(parameters_.segment(index[1], this->varianceProcessParameterSize()));
        this->setDistributionParameters(parameters_.segment(index[2], this->distributionParameterSize()));
    }

};

template<typename T>
class LikelihoodHelper : public std::unary_function<Eigen::VectorXd, Real> {
    mutable T model_;
    RealSeries data_;
public:
    typedef T ModelType;
    LikelihoodHelper(const T& model,
                     const RealSeries data): model_(model), data_(data) {}

    Real operator()(const Eigen::VectorXd& parameters) const {
        model_.parameters() = parameters.array();
        model_.setParameters();
	RealSeries residuals(data_.size());
	RealSeries variances(data_.size());
	
	model_.residuals(data_, residuals);
	model_.variances(residuals, variances);
	//	DEBUG_PRINT(variances.log().sum());
	//	DEBUG_PRINT((residuals.pow(2.)/variances).sum());
		
	//	return - variances.log().sum() - (residuals.pow(2.)/variances).sum();
	
	return model_.loglikelihood(data_);
    }
    T model() {
        return model_;
    }
    const ParameterArray& parameters() const {
        return model_.parameters();
    }

    ParameterArray& parameters() {
        return model_.parameters();
    }
 };

}


#endif
