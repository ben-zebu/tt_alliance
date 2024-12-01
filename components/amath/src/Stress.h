#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

namespace amath {
    /// \brief Stress tensor size: 6 components (3D tensor) are stored in a vector.
    static constexpr std::size_t STRESS_SIZE = 6;
    /// \brief Tolerance for stress components comparisons.
    static constexpr double STRESS_TOLERANCE = 1e-6;

    /// \brief Represents a 3D stress tensor.
    ///
    /// The stress tensor is represented as a vector of 6 components:
    /// \f$(s_{11}, s_{22}, s_{33}, s_{12}, s_{13}, s_{23})\f$
    ///
    /// This class provides various operations and utility functions for working with stress tensors,
    /// such as calculating principal stresses, von Mises stress, and Tresca stress.
    class Stress {
        protected:
            /// \brief Stress components stored as a vector: 
            //// \f$(s_{11}, s_{22}, s_{33}, s_{12}, s_{13}, s_{23})\f$
            std::array<double, STRESS_SIZE> components;
        
        private:
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
            Stress() = default ;
            Stress(const std::array<double, STRESS_SIZE>& stress) : components(stress) {};
            Stress(const Stress& other) : components(other.components) {};
            Stress(const std::vector<double>& components);
            Stress& operator=(const Stress& other);

            // Utility
            bool is_diag() const;
            /// \brief  Set all stress components to zero.
            void zero();

            /// \brief Calculates the three principal stresses from the stress tensor components.
            ///
            /// This function computes the three principal stresses from the stress tensor components stored in the 
            /// `components` vector. The principal stresses are returned in descending order.
            std::array<double, STRESS_SIZE/2> principal_stresses() const;

            // Accessors
            double operator[](std::size_t index) const { return components[index]; }
            double& operator[](std::size_t index) { return components[index]; }
            /// \brief Returns the number of stress components.
            std::size_t size() const { return components.size(); } 

            Stress& operator+=(const Stress& other);
            Stress& operator-=(const Stress& other);
            Stress& operator*=(const double& scalar); 
            bool operator==(const Stress& other);
            bool operator!=(const Stress& other);
            
            void print(std::ostream& flux) const;

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
}

namespace amath {
    Stress operator+(const Stress& s1, const Stress& s2);
    Stress operator-(const Stress& s1, const Stress& s2);
    Stress operator*(const double& scalar, const Stress& s);
    std::ostream& operator<<(std::ostream& flux, const Stress& stress);
}

