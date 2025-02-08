#pragma once

#include "Environment.h"
#include "DataCollector.h"

#include "ProblemDescription.h"
#include "ProblemFemInterface.h"
#include "ProblemLoadstep.h"
#include "ProblemMaterial.h"
#include "ProblemPlate.h"
#include "ProblemSection.h"
#include "ProblemTable.h"
#include "ProblemTorsor.h"
#include "ProblemTransient.h"

namespace adata {

    using namespace adata::parts;

    class DataManager : public abase::DataCollector {
        private:
            /// @brief Read the title of the problem in the input file
            /// @param reader file reader associated to the input file
            /// @param collector commands collector used to read the input file
            /// @return problem title as a vector of strings
            std::vector<std::string> read_title(abase::FileReader& reader, const abase::CommandsCollector& collector);

        protected:
            /// @brief Convert read data from input file into data objects
            /// @param command read data from the input file
            /// @param filecontext file context
            virtual void set_data(const std::shared_ptr<abase::BaseCommand>& command, const std::string& filecontext);

            /// @brief Read special line without any command
            /// @param command_name name of the command linked to the special line
            /// @param reader file reader associated to the input file
            /// @param collector commands collector used to read the input file
            virtual void read_special_line(const std::string& command_name, abase::FileReader& reader, 
                                           const abase::CommandsCollector& collector);

            ProblemFemInterface fem_interface;
            ProblemDescription description;
            ProblemPlate plate;
            ProblemTorsor torsor;
            std::vector<ProblemLoadstep> loadsteps;
            std::vector<ProblemTransient> transients;
            std::vector<ProblemTable> tables;
            std::vector<ProblemMaterial> materials;
            std::vector<ProblemSection> sections;

        public:
            DataManager() = default;
            ~DataManager() = default;
            
            /// @brief Verify if the object is correctly initialized with consistent objects
            void verify() const;

            /// @brief Return the number of transients
            std::size_t nb_transients() const { return transients.size(); }
            /// @brief Return the number of loadsteps
            std::size_t nb_loadsteps() const { return loadsteps.size(); }
            /// @brief Return the number of materials
            std::size_t nb_materials() const { return materials.size(); }
            /// @brief Return the number of sections
            std::size_t nb_sections() const { return sections.size(); }
            /// @brief Return the number of tables
            std::size_t nb_tables() const { return tables.size(); }

            /// @brief Return a reference to a transient
            /// @param name transient name
            /// @return reference to the transient
            const ProblemTransient& get_transient(const std::string& name) const;
            /// @brief Return a reference to a transient
            /// @param rank transient rank
            /// @return reference to the transient
            const ProblemTransient& get_transient(const std::size_t rank) const;

    };

}