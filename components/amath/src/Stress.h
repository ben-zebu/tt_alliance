#pragma once

#include <Eigen/Dense>

namespace amath {
    /// \brief Stress tensor size: 6 components (3D tensor) are stored in a vector.
    static constexpr size_t STRESS_SIZE = 6;
    /// \brief Tolerance for stress components comparisons.
    static constexpr double STRESS_TOLERANCE = 1e-10;

    /// \brief Represents a 6-dimensional vector of double values based on Eigen library.
    using Vector6d = Eigen::Matrix<double, 1, STRESS_SIZE>;

    /// \brief Represents a 3D stress tensor.
    ///
    /// The stress tensor is represented as a vector of 6 components:
    /// \f$(s_{11}, s_{22}, s_{33}, s_{12}, s_{13}, s_{23})\f$
    ///
    /// This class provides various operations and utility functions for working with stress tensors,
    /// such as calculating principal stresses, von Mises stress, and Tresca stress.
    class Stress {
        private:
            /// \brief Stress components stored as a vector: 
            //// \f$(s_{11}, s_{22}, s_{33}, s_{12}, s_{13}, s_{23})\f$
            Vector6d components = Vector6d::Zero();
            
            /// \brief Constant factor used in the calculation of an equivalent stress based on
            /// an homothecy from von Mises stress. The von Mises stress is calculated as
            /// \f$\sigma_\text{VM} = \sqrt{\frac{2}{3}\left(\sigma_1^2 + \sigma_2^2 + \sigma_3^2\right)}\f$, 
            ///  where \f$\sigma_1\f$, \f$\sigma_2\f$, and \f$\sigma_3\f$ are the principal stresses. 
            /// This constant factor is equal to \f$\frac{2}{\sqrt{3}}\f$.
            static constexpr double VM_FACTOR = 1.1547005383792517;

            /// \brief Calculates the principal stresses from the stress tensor components.
            ///
            /// This function takes the stress tensor components and calculates the three principal stresses.
            /// The principal stresses are returned through the output parameters a, b, and c.
            void getPrincipalStresses(double& a, double& b, double& c) const;

            /// \brief Sorts the three eigen values in descending order.
            ///
            /// This function takes three eigen values by reference and sorts them in descending order.
            /// The sorted values are stored back in the input parameters a, b, and c.
            static void sortEigenvalues(double& a, double& b, double& c);

            /// \brief Solves the cubic equation \f$x^3 + px^2 + qx + r = 0\f$ for its three real roots.
            ///
            /// This function takes the coefficients \f$p\f$, \f$q\f$, and \f$r\f$ of the cubic equation and computes 
            /// the three real roots \f$x_1\f$, \f$x_2\f$, and \f$x_3\f$.
            ///
            /// \param p The coefficient of the quadratic term.
            /// \param q The coefficient of the linear term.
            /// \param r The constant term.
            /// \param[out] x1 The first real root of the cubic equation.
            /// \param[out] x2 The second real root of the cubic equation.
            /// \param[out] x3 The third real root of the cubic equation.
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
            /// \brief Returns the number of stress components.
            ///
            /// This function returns the number of stress components stored in the `components` vector.
            size_t size() const { return components.cols(); }

            /// \brief Calculates the three principal stresses from the stress tensor components.
            ///
            /// This function computes the three principal stresses from the stress tensor components stored in the 
            /// `components` vector. The principal stresses are returned in descending order.
            Eigen::Vector3d principal_stresses() const;

            /// \brief Returns a reference to the vector of stress tensor components.
            ///
            /// This function returns a constant reference to the `components` vector, which stores the six independent
            /// components of the stress tensor.
            const Vector6d& get_components() const { return components; }

            // Operators overloading
            Stress& operator+=(const Stress& other);
            Stress& operator-=(const Stress& other);
            Stress& operator*=(const double& scalar); 
            bool operator==(const Stress& other);
            bool operator!=(const Stress& other);

            /// \brief Calculates the Tresca stress, also known as the maximum shear stress.
            ///
            /// The Tresca stress is the maximum absolute difference between the principal stresses. It is determined
            /// with the following formula:
            /// \f$\sigma_\text{Tresca} = \max\left(\sigma_1 - \sigma_2, \sigma_2 - \sigma_3, \sigma_3 - \sigma_1\right)\f$.
            /// where \f$\sigma_1\f$, \f$\sigma_2\f$, and \f$\sigma_3\f$ are the principal stresses.
            double tresca() const;

            /// \brief Calculates the von Mises stress.
            ///
            /// The von Mises stress is calculated from the principal stresses using the following formula:
            /// \f$\sigma_\text{von Mises} = \sqrt{\frac{1}{2}\left[(\sigma_1 - \sigma_2)^2 + 
            ///                              (\sigma_2 - \sigma_3)^2 + (\sigma_3 - \sigma_1)^2\right]}\f$
            /// where \f$\sigma_1\f$, \f$\sigma_2\f$, and \f$\sigma_3\f$ are the principal stresses.
            double mises() const;
            
            /// \brief Calculates the reduced von Mises stress. It is a homothetic transformation of the von Mises 
            //// stress with the factor \f$\frac{2}{\sqrt{3}}\f$.
            double reduced_mises() const;

    };

    amath::Stress operator+(const amath::Stress& s1, const amath::Stress& s2);
    amath::Stress operator-(const amath::Stress& s1, const amath::Stress& s2);
    amath::Stress operator*(const double& scalar, const amath::Stress& s) { return amath::Stress(scalar * s.get_components()); }

}
