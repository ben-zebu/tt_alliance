#pragma once
#include <unordered_map>

#include "DataCollector.h"
#include "Environment.h"
#include "BaseMaterial.h"

namespace adata {

    class BaseMaterialCollector : public abase::DataCollector {
        private:
            /// @brief Create a material object
            /// @return material object
            std::shared_ptr<BaseMaterial> create_material() const;

        protected:
            /// @brief Convert read data from input file into data objects
            /// @param command read data from the input file
            /// @param filecontext file context
            void set_data(const std::shared_ptr<abase::BaseCommand>& command, const std::string& filecontext);

            /// @brief Collection of materials
            std::vector<std::shared_ptr<BaseMaterial>> materials;

        public:
            
            BaseMaterialCollector() = default;
            ~BaseMaterialCollector() = default;

            /// @brief Get the material associated to a given identification
            /// @param material_id material's identification
            /// @return shared pointer to the material
            std::shared_ptr<BaseMaterial> get_material(const std::string& material_id) const;

            /// @brief Clear the collection of materials
            void clear() { materials.clear(); }
    };

}