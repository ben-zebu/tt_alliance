#pragma once

#include "Environment.h"
#include "Commands.h"
#include "FileReader.h"

namespace adata::parts {

    class StressCoefficient {
        public:
            /// @brief table name associated to the coefficient
            std::string name = "";
            /// @brief expected size of values
            std::size_t expected_size = 0;
            /// @brief Values associated to the coefficient
            std::vector<double> values;

            StressCoefficient(const std::string& a_name, std::size_t a_expected_size) 
                              : name(a_name), expected_size(a_expected_size) {};
            virtual ~StressCoefficient() = default;

            /// @brief clear read data associated to the command
            virtual void clear() { values.clear(); }
            /// @brief set default values for the coefficient
            virtual void set_default_values();

            /// @brief Initialize the object with the values read from the input file
            /// @param command command read from the input file
            virtual void init(const std::shared_ptr<abase::BaseCommand>& command);
            /// @brief Verify the coherence of the material definition
            virtual void verify(const std::string& filecontext) const;
    };

    class PKCoefficient : public StressCoefficient {
        public:
            PKCoefficient() : StressCoefficient("PK", 12) {};
    };

    class KFCoefficient : public StressCoefficient {
        public:
            KFCoefficient() : StressCoefficient("KF", 6) {};
    };

    class KMCoefficient : public StressCoefficient {
        public:
            KMCoefficient() : StressCoefficient("KM", 6) {};
    };

    class OvalCoefficient : public StressCoefficient {
        private:
            /// @brief set values for the coefficient based on specific rules
            void set_other_values();
        public:
            OvalCoefficient() : StressCoefficient("OVAL", 5) {};

            /// @brief Initialize the object with the values read from the input file
            /// @param command command read from the input file
            virtual void init(const std::shared_ptr<abase::BaseCommand>& command);           
    };

    class PMBCoefficient : public StressCoefficient {
        public:
            PMBCoefficient() : StressCoefficient("PMB", 6) {};
    };

}
