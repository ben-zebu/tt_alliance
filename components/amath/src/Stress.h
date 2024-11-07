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
            //Eigen::Matrix3d getMatrix() const;

            //void set_components(const Eigen::Vector<double,STRESS_SIZE>& components);
            const Vector6d& get_components() const { return components; }

            // Operators overloading
            Stress& operator+=(const Stress& other);
            Stress& operator-=(const Stress& other);
            Stress& operator*=(const double& scalar); 
            bool operator==(const Stress& other);
            bool operator!=(const Stress& other);

    };

    amath::Stress operator+(const amath::Stress& s1, const amath::Stress& s2);
    amath::Stress operator-(const amath::Stress& s1, const amath::Stress& s2);
    amath::Stress operator*(const double& scalar, const amath::Stress& s) { return amath::Stress(scalar * s.get_components()); }

}

