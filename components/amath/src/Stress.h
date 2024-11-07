#pragma once

#include <Eigen/Dense>

namespace amath {
    static constexpr size_t STRESS_SIZE = 6;
    static constexpr double STRESS_TOLERANCE = 1e-10;

    using Vector6d = Eigen::Matrix<double, 1, STRESS_SIZE>;

    class Stress {
        private:
            // Stress components (s11, s22, s33, s12, s13, s23)
            Vector6d components = Vector6d::Zero();

            static constexpr double VM_FACTOR = 1.1547005383792517; // 2/sqrt(3)

            void getPrincipalStresses(double& a, double& b, double& c) const;
            static void sortEigenvalues(double& a, double& b, double& c);
            static void solveCardan(const double p, const double q, const double r,
                                    double& x1, double& x2, double& x3);

        public:
            // Constructors and copy constructor
            Stress() = default;
            Stress(const Vector6d& components);
            Stress(const Stress& other);
            Stress(const std::initializer_list<double>& components);
            Stress& operator=(const Stress& other);

            // Utility
            bool is_diag() const;
            size_t size() const { return components.cols(); }
            Eigen::Vector3d principal_stresses() const;

            const Vector6d& get_components() const { return components; }

            // Operators overloading
            Stress& operator+=(const Stress& other);
            Stress& operator-=(const Stress& other);
            Stress& operator*=(const double& scalar); 
            bool operator==(const Stress& other);
            bool operator!=(const Stress& other);

            // Stress invariants
            double tresca() const;
            double mises() const;
            double reduced_mises() const;

    };

    amath::Stress operator+(const amath::Stress& s1, const amath::Stress& s2);
    amath::Stress operator-(const amath::Stress& s1, const amath::Stress& s2);
    amath::Stress operator*(const double& scalar, const amath::Stress& s) { return amath::Stress(scalar * s.get_components()); }

}

