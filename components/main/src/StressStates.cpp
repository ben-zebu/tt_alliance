#include "StressStates.h"

void StressStates::addState(const std::vector<double>& components) {
    m_states.push_back(components);
}

const std::vector<double>& StressStates::getComponents(size_t index) const {
    return m_states[index];
}

Eigen::Matrix3d StressStates::getMatrix(size_t index) const {
    const auto& components = m_states[index];
    Eigen::Matrix3d stress_matrix;
    stress_matrix << components[0], components[3], components[4],
                    components[3], components[1], components[5],
                    components[4], components[5], components[2];
    return stress_matrix;
}

double StressStates::tresca(size_t index) const {
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(getMatrix(index));
    
    double s1 = solver.eigenvalues()(0);
    double s2 = solver.eigenvalues()(1);
    double s3 = solver.eigenvalues()(2);
    
    sortEigenvalues(s1, s2, s3);
    return std::abs(s1 - s3);
}

double StressStates::trescaFast(size_t index) const {
    const auto& s = m_states[index];
    
    // Compute invariants
    double I1 = s[0] + s[1] + s[2];
    double I2 = s[0]*s[1] + s[1]*s[2] + s[2]*s[0] - (s[3]*s[3] + s[4]*s[4] + s[5]*s[5]);
    double I3 = s[0]*s[1]*s[2] + 2*s[3]*s[4]*s[5] - (s[0]*s[5]*s[5] + s[1]*s[4]*s[4] + s[2]*s[3]*s[3]);
    
    // Solve cubic equation using Cardan method
    double p = I2 - I1*I1/3.0;
    double q = 2.0*I1*I1*I1/27.0 - I1*I2/3.0 + I3;
    double r = -I1/3.0;

    double s1, s2, s3;
    solveCardan(p, q, r, s1, s2, s3);
    
    sortEigenvalues(s1, s2, s3);
    return std::abs(s1 - s3);
}

double StressStates::trescaWithCardan(size_t index) const {
    Eigen::Matrix3d stress = getMatrix(index);
    
    double I1 = stress.trace();
    double I2 = 0.5 * (I1*I1 - (stress * stress).trace());
    double I3 = stress.determinant();
    
    double p = I2 - I1*I1/3.0;
    double q = 2.0*I1*I1*I1/27.0 - I1*I2/3.0 + I3;
    double r = -I1/3.0;
    
    double s1, s2, s3;
    solveCardan(p, q, r, s1, s2, s3);
    
    sortEigenvalues(s1, s2, s3);
    return std::abs(s1 - s3);
}

double StressStates::mises(size_t index) const {
    const auto& s = m_states[index];
    
    // Von Mises = sqrt(0.5*[(s11-s22)² + (s22-s33)² + (s33-s11)² + 6*(s12² + s23² + s31²)])
    return std::sqrt(0.5 * (
        std::pow(s[0] - s[1], 2) + 
        std::pow(s[1] - s[2], 2) + 
        std::pow(s[2] - s[0], 2) + 
        6.0 * (s[3]*s[3] + s[4]*s[4] + s[5]*s[5])
    ));
}

double StressStates::misesReduced(size_t index) const {
    return mises(index) / VM_FACTOR;
}

void StressStates::sortEigenvalues(double& a, double& b, double& c) {
    if (a < b) std::swap(a, b);
    if (b < c) std::swap(b, c);
    if (a < b) std::swap(a, b);
}

void StressStates::solveCardan(const double p, const double q, const double r,
                        double& x1, double& x2, double& x3) {
    const double pi = M_PI;
    double D = q*q/4.0 + p*p*p/27.0;
    
    if (D > 0) {
        double u = std::cbrt(-q/2.0 + std::sqrt(D));
        double v = std::cbrt(-q/2.0 - std::sqrt(D));
        x1 = u + v + r;
        x2 = -0.5*(u + v) + r;
        x3 = x2;
    } else {
        double phi = std::acos(-q/(2.0*std::sqrt(-p*p*p/27.0)));
        double rho = 2.0*std::sqrt(-p/3.0);
        x1 = rho*std::cos(phi/3.0) + r;
        x2 = rho*std::cos((phi + 2.0*pi)/3.0) + r;
        x3 = rho*std::cos((phi + 4.0*pi)/3.0) + r;
    }
}
