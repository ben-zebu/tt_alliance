#pragma once

#include "Environment.h"
#include "Commands.h"
#include "FileReader.h"

namespace adata::parts {

    class ProblemTransient {
        private:
            /// @brief Get the maximum rank associated to loadsteps
            std::size_t max_loadstesp() const;

            /// @brief maximum excepted rank associated to time steps
            std::size_t _nb_loadsteps = 0;

            /// @brief define a default name for the transient: "TR_" + id
            /// @param id rank associated to the transient
            void default_name(std::size_t id) { name = "TR_" + std::to_string(id); };

        public:
            /// @brief transient name
            std::string name = "";
            /// @brief number of cycles associated to the transient
            std::size_t nb_cycles = 0;
            /// @brief time steps associated to the transient
            std::vector<std::size_t> loadsteps;
            /// @brief transient's groups
            std::vector<std::string> groups;
            /// @brief defined the crossing groups
            std::vector<std::string> crossing_transient;
            /// @brief shared group
            std::string shared_group = "";
            /// @brief variant group
            std::string variant_group = "";
            /// @brief number of cycles for the variant group
            std::size_t variant_cycles = 0;
            /// @brief defined if the transient is alone
            bool is_alone = true;
            /// @brief defined if the transient must be used for 3Sm analysis
            bool is_3sm = true; 
            /// @brief defined if the transient must be used for thermal ratchet
            bool is_thermal_ratchet = true;

            ProblemTransient() = default;
            virtual ~ProblemTransient() = default;

            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            /// @param id rank associated to the transient
            /// @param nb_torsors number of external torsors
            void init(const std::shared_ptr<abase::BaseCommand>& command, std::size_t id, std::size_t nb_loadsteps);

            /// @brief Verify if the object is correctly initialized
            /// @param filecontext context of the input file
            /// @return status of the object initialization
            void verify(const std::string& filecontext) const;
    };

}