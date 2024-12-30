#pragma once

#include "Environment.h"
#include "Commands.h"
#include "FileReader.h"

namespace adata {

    class StressCoefficient {
        public:
            /// @brief table name associated to the coefficient
            std::string name = "";
            /// @brief expected size of values
            std::size_t expected_size = 0;
            /// @brief Values associated to the coefficient
            std::vector<double> values;

            StressCoefficient() = default;
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
            /// @brief Initialize the object with the values read from the input file and verify the coherence of 
            /// the material definition. This function is a combination of the init, verify and set_default_values
            /// functions.
            /// @param command command read from the input file
            /// @param filecontext file context for error message
            virtual void init_and_verify(const std::shared_ptr<abase::BaseCommand>& command, const std::string& filecontext);
    };

    class PKCoefficient : public StressCoefficient {
        public:
            /// @brief table name associated to the coefficient
            std::string name = "PK";
            /// @brief expected size of values
            std::size_t expected_size = 12;

            PKCoefficient() = default;
            virtual ~PKCoefficient() = default;
    };

    class KFCoefficient : public StressCoefficient {
        public:
            /// @brief table name associated to the coefficient
            std::string name = "KF";
            /// @brief expected size of values
            std::size_t expected_size = 6;

            KFCoefficient() = default;
            virtual ~KFCoefficient() = default;
    };

    class KMCoefficient : public StressCoefficient {
        public:
            /// @brief table name associated to the coefficient
            std::string name = "KM";
            /// @brief expected size of values
            std::size_t expected_size = 6;

            KMCoefficient() = default;
            virtual ~KMCoefficient() = default;
    };

    class OvalCoefficient : public StressCoefficient {
        private:
            /// @brief set values for the coefficient based on specific rules
            void set_other_values();
        public:
            /// @brief table name associated to the coefficient
            std::string name = "OVAL";
            /// @brief expected size of values
            std::size_t expected_size = 5;

            OvalCoefficient() = default;
            virtual ~OvalCoefficient() = default;

            /// @brief Initialize the object with the values read from the input file and verify the coherence of 
            /// the material definition. This function is a combination of the init, verify and set_default_values
            /// functions.
            /// @param command command read from the input file
            /// @param filecontext file context for error message
            virtual void init_and_verify(const std::shared_ptr<abase::BaseCommand>& command, const std::string& filecontext);            
    };

    class PMBCoefficient : public StressCoefficient {
        public:
            /// @brief table name associated to the coefficient
            std::string name = "PMB";
            /// @brief expected size of values
            std::size_t expected_size = 3;

            PMBCoefficient() = default;
            virtual ~PMBCoefficient() = default;
    };

}