#include <vector>
#include <Eigen/Eigen>
#include <iostream>
typedef Eigen::ArrayXd RealSeries;
typedef Eigen::ArrayXd ParameterArray;
//typedef RealSeries VolatilitySeries;
typedef RealSeries VarianceSeries;

//typedef RealSeries MeanSeries;
typedef long Size;
typedef double Real;


static const Real PI = 3.141592653; 

template<class Derived>
struct Distribution : public Derived{
  void pdf(RealSeries& sample) const{
    static_cast<const Derived*>(this)->pdfImpl(sample);
  }
  Size distributionParameterSize() const{
    return static_cast<const Derived*>(this)->distributionParameterSizeImpl();
  }
  void setDistributionParameters(const ParameterArray& parameters) {
    static_cast<Derived*>(this)->setDistributionParametersImpl(parameters);
  }    
  void getDistributionParameters(Eigen::VectorBlock<ParameterArray> p){
   static_cast<Derived*>(this)->getDistributionParametersImpl(p);
  }    
};

struct Normal{
  Real mean_;
  Real sigma_;
  Normal(Real mean = 0., Real sigma = 1.) : mean_(mean), sigma_(sigma){}
  template<typename T>
  void pdfImpl(T& x) const{
    Real sigma2 = sigma_ * sigma_;
    x = 1. /std::sqrt(2. * PI * sigma2) * std::exp((-0.5* (x - mean_)*(x-mean_) / sigma2 ));
  }
  Size distributionParameterSizeImpl() const{
    return 0;
  }
  void setDistributionParametersImpl(const ParameterArray& p){
  }
  void getDistributionParametersImpl(Eigen::VectorBlock<ParameterArray> p) {
  }    
};

class GammaFunction : public std::unary_function<Real, Real> {
public:
  template<typename T>
  T logValue(const T& x) const{
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
struct Student{
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


template<class Derived>
struct MeanProcess : public Derived{
  void residuals(const RealSeries& rtn, RealSeries& residuals) const{
    static_cast<const Derived*>(this)->residualsImpl( rtn, residuals);
  }
  Size meanProcessParameterSize() const{
    return static_cast<const Derived*>(this)->meanProcessParameterSizeImpl();
  }
  void setMeanProcessParameters(const ParameterArray& parameters) {
    static_cast<Derived*>(this)->setMeanProcessParametersImpl(parameters);
  }
  void getMeanProcessParameters(Eigen::VectorBlock<ParameterArray> parameters) {
    static_cast<Derived*>(this)->getMeanProcessParametersImpl(parameters);
  }      
};

template<class Derived>
struct  VarianceProcess : public Derived{
  void variances(const RealSeries& residuals, VarianceSeries& variances) const {
    static_cast< const Derived*>(this)->variancesImpl(residuals, variances);
  }
  Size varianceProcessParameterSize() const{
    return static_cast<const Derived*>(this)->varianceProcessParameterSizeImpl();
  }
  void setVarianceProcessParameters(const ParameterArray& parameters) {
    static_cast<Derived*>(this)->setVarianceProcessParametersImpl(parameters);
  }
  void getVarianceProcessParameters(Eigen::VectorBlock<ParameterArray> parameters) {
     static_cast< Derived*>(this)->getVarianceProcessParametersImpl(parameters);
  }
};

struct ConstantMean{
  Real mean_;
  void residualsImpl(const RealSeries& rtn, RealSeries& residuals) const{
    residuals = rtn - mean_;
  }
  Size meanProcessParameterSizeImpl() const{
    return 1;
  }
  void setMeanProcessParametersImpl(const ParameterArray& p){
    mean_ = p[0];
  }

  void getMeanProcessParametersImpl( Eigen::VectorBlock<ParameterArray> p){
    p[0] = mean_;
  }
};

struct ConstantVariance{
  Real meanVol_;
  void variancesImpl(const RealSeries& residuals, VarianceSeries& variances) const{
    variances = (residuals - meanVol_).pow(2.);
  }
  Size varianceProcessParameterSizeImpl() const{
    return 1;
  }
  void setVarianceProcessParametersImpl(const ParameterArray& p){
    meanVol_ = p[0];
  }
  void getVarianceProcessParametersImpl(Eigen::VectorBlock<ParameterArray> p){
    p[0] = meanVol_;
  }
};



template<class M, class V, class D>
struct  TimeSeriesModel : 
  public MeanProcess<M>, 
  public VarianceProcess<V>, 
  public Distribution<D>{
  TimeSeriesModel(){
    index[0] = 0;
    index[1] = this->meanProcessParameterSize();
    index[2] = index[1] + this->varianceProcessParameterSize();
    index[3] = index[2] + this->distributionParameterSize();
    parameters_.resize(index[3]);
  }
  Real likelihood(const RealSeries& data) {    
    RealSeries residuals(data.size());
    RealSeries variances(data.size());
    this->residuals(data, residuals);
    this->variances(residuals, variances);
    residuals /= variances.sqrt();
    this->pdf(residuals);
    return -residuals.log().sum();

    //    this->residuals(const RealSeries& rtn, RealSeries& residuals);
    //    this->variances(const RealSeries& residuals, VarianceSeries& variances){);
  }  

  mutable ParameterArray parameters_;
  Size index[4];

  ParameterArray& parameters(){
    return parameters_;
  }

  const ParameterArray& parameters() const{
    return parameters_;
  }

  void fetchParameters() {
    this->getMeanProcessParameters(parameters_.segment(index[0], meanProcessParameterSize()));
    this->getVarianceProcessParameters(parameters_.segment(index[1], varianceProcessParameterSize()));
    this->getDistributionParameters(parameters_.segment(index[2], distributionParameterSize()));
  }
  void setParameters(){
    this->setMeanProcessParameters(parameters_.segment(index[0], meanProcessParameterSize()));
    this->setVarianceProcessParameters(parameters_.segment(index[1], varianceProcessParameterSize()));
    this->setDistributionParameters(parameters_.segment(index[2], distributionParameterSize()));
  }  
  
};

template<typename T>
class LikelihoodHelper{
  T model_;
  RealSeries data_;
public:
  LikelihoodHelper(const T& model,
    const RealSeries data): model_(model), data_(data){}
  Real operator()(const ParameterArray& parameters){
    model_.parameters() = parameters;
    model_.setParameters();
    return model_.likelihood(data_);
  }
};

template<class M, class V, class D>
class Estimator{

  TimeSeriesModel<M, V, D> t_;
public:
  template<class M, class V, class D>
  Estimator(  TimeSeriesModel<M, V, D> t):t_(t){}
  Estimator(){}
  void test(){
    t_.parameters();
  }
};


int main(){
  TimeSeriesModel<ConstantMean, ConstantVariance, Normal> timeSeriesModel;

  Size n = 1000;
  RealSeries sample = RealSeries::Random(n);
  timeSeriesModel.pdf(sample);

  timeSeriesModel.distributionParameterSize();

  ParameterArray p(2);
  p[0] = 0;
  p[1] = 1;
  timeSeriesModel.parameters() = p;

  timeSeriesModel.setParameters();
  p[0] = 2;
  p[1] = 3;
  timeSeriesModel.parameters() = p;
  timeSeriesModel.fetchParameters();
  timeSeriesModel.likelihood(sample);
  Estimator<
  ConstantMean, 
    ConstantVariance, 
    Normal> estimator;
  estimator.test(); 
  p[0] = 0;
  p[1] = 1;

  LikelihoodHelper<TimeSeriesModel<ConstantMean, ConstantVariance, Normal> > objective(timeSeriesModel, sample);
  std::cout << objective(p) << std::endl;
}
