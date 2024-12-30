#pragma once
#include <unordered_map>

#include "Environment.h"
#include "Commands.h"
#include "FileReader.h"
#include "Table.h"

namespace adata::parts {

    /// @brief Value used as a flag to check if the Young modulus has been defined or not for experimental fatigue laws
    inline constexpr double UNSET_YOUNG_MODULUS = 1.;
    
    class FatigueLaw {
        protected:
            void general_verification(const std::string& filecontext) const;

            /// @brief Set the generic parameters of the fatigue law (law_id, Ec, description, code_editions).
            /// @note This function must be called by the init function of each derived class.
            /// @param command values read from the input file
            void set_generic_parameters(const std::shared_ptr<abase::BaseCommand>& command);
            /// @brief Copy the generic parameters of the fatigue law (law_id, Ec, description, code_editions) 
            /// to an other object.
            /// @param other other fatigue law object 
            void copy_generic_parameters(std::shared_ptr<FatigueLaw> other) const;


            std::unordered_map<std::string, std::string> descriptions;
            std::unordered_map<std::string, std::vector<std::string>> editions;

            /// @brief Young modulus of the material used to build the fatigue law
            double Ec = 0.;
            /// @brief Ponderation factor for number of cycles. A value of upper than 1. means that the material is 
            /// more sensitive to the number of cycles.
            double N_ratio = 1.;
            /// @brief Identification of the fatigue law
            std::string law_id = "";

        public:

            FatigueLaw() = default;
            virtual ~FatigueLaw() = default;

            std::string get_id() const { return law_id; }
            bool support_code(const std::string& code, const std::string& description) const;

            bool is_same_law(const FatigueLaw& other) const;


            /// @brief Give the endurance limit associated to the fatigue law
            /// @return endurance limit
            virtual double endurance_limit() const = 0;
            /// @brief Compute the allowable stress associated to a number of cycles
            virtual double allowable_stress(std::size_t Na) const = 0;
            /// @brief Compute the allowable number of cycles associated to a stress
            virtual std::size_t allowable_cycles(double Sa) const = 0;
            /// @brief Compute the Young modulus ratio \f $\frac{E_c}{E}$ \f
            /// @param E Apparant Young modulus of the material
            /// @return Young modulus ratio
            double young_ratio(double E) const { return Ec / E; }

            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            virtual void init(const std::shared_ptr<abase::BaseCommand>& command) = 0;

            /// @brief Verify if the object is correctly initialized
            /// @param filecontext context of the input file
            /// @return status of the object initialization
            virtual void verify(const std::string& filecontext) const = 0;

            /// @brief Clone the current object
            /// @return shared pointer to the cloned object
            virtual std::shared_ptr<FatigueLaw> clone() const = 0;
    };


    class TabularFatigueLaw : public FatigueLaw {
        protected:
            amath::Table table;

        public:

            TabularFatigueLaw() = default;
            virtual ~TabularFatigueLaw() = default;

            /// @brief Give the endurance limit associated to the fatigue law
            /// @return endurance limit
            virtual double endurance_limit() const override { return table.get_ymin(); }
            /// @brief Compute the allowable stress associated to a number of cycles
            virtual double allowable_stress(std::size_t Na) const override;
            /// @brief Compute the allowable number of cycles associated to a stress
            virtual std::size_t allowable_cycles(double Sa) const override;

            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            virtual void init(const std::shared_ptr<abase::BaseCommand>& command) override;

            /// @brief Verify if the object is correctly initialized
            /// @param filecontext context of the input file
            /// @return status of the object initialization
            virtual void verify(const std::string& filecontext) const override;

            virtual std::shared_ptr<FatigueLaw> clone() const override;
    };

    class PowerFatigueLaw : public FatigueLaw {
        protected:
            double alpha = 0.;
            double beta = 0.;
            double Seq = 0.;

        public:
            PowerFatigueLaw() = default;
            virtual ~PowerFatigueLaw() = default;

            /// @brief Give the endurance limit associated to the fatigue law
            /// @return endurance limit
            virtual double endurance_limit() const override { return Seq; }
            /// @brief Compute the allowable stress associated to a number of cycles
            virtual double allowable_stress(std::size_t Na) const override;
            /// @brief Compute the allowable number of cycles associated to a stress
            virtual std::size_t allowable_cycles(double Sa) const override;
            
            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            virtual void init(const std::shared_ptr<abase::BaseCommand>& command) override;

            /// @brief Verify if the object is correctly initialized
            /// @param filecontext context of the input file
            /// @return status of the object initialization
            virtual void verify(const std::string& filecontext) const override;
         
            virtual std::shared_ptr<FatigueLaw> clone() const override;
    };

    class PolynomialFatigueLaw : public FatigueLaw {
        protected:
            std::vector<double> coefficients;
            double beta = 0.;
            double Seq = 0.;
        public:

            PolynomialFatigueLaw() = default;
            virtual ~PolynomialFatigueLaw() = default;

            /// @brief Give the endurance limit associated to the fatigue law
            /// @return endurance limit
            virtual double endurance_limit() const override { return Seq; };
            /// @brief Compute the allowable stress associated to a number of cycles
            virtual double allowable_stress(std::size_t Na) const override;
            /// @brief Compute the allowable number of cycles associated to a stress
            virtual std::size_t allowable_cycles(double Sa) const override;

            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            virtual void init(const std::shared_ptr<abase::BaseCommand>& command) override;

            /// @brief Verify if the object is correctly initialized
            /// @param filecontext context of the input file
            /// @return status of the object initialization
            virtual void verify(const std::string& filecontext) const override;

            virtual std::shared_ptr<FatigueLaw> clone() const override;
    };

}