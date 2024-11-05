#pragma once

#include <Eigen/Dense>
#include <vector>


class StressStates {
public:

    // Add a new stress state (s11, s22, s33, s12, s13, s23)
    void addState(const std::vector<double>& components);
    
    // Get stress state by index
    const std::vector<double>& getComponents(size_t index) const;
    Eigen::Matrix3d getMatrix(size_t index) const;
    
    // Compute Stress invariants
    double tresca(size_t index) const;
    double trescaFast(size_t index) const;
    double trescaWithCardan(size_t index) const;
    double mises(size_t index) const;
    double misesReduced(size_t index) const;
    
    // Utility
    size_t size() const { return m_states.size(); }

private:
    std::vector<std::vector<double>> m_states;
    
    static void sortEigenvalues(double& a, double& b, double& c);
    static void solveCardan(const double p, const double q, const double r, 
                           double& x1, double& x2, double& x3);
    static constexpr double VM_FACTOR = 0.8660254037844386; // sqrt(3)/2
    //static void sortEigenvalues_r(float& a, float& b, float& c);
};

