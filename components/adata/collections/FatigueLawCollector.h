#pragma once
#include <unordered_map>

#include "DataCollector.h"
#include "Environment.h"
#include "FatigueLaw.h"

namespace adata {

    using namespace adata::parts;
    
    class FatigueLawCollector : public abase::DataCollector {
        private:
            /// @brief Create a fatigue law object based on the type
            /// @param type fatigue law type
            /// @return fatigue law object
            std::shared_ptr<FatigueLaw> create_law(const std::string& type) const;

        protected:
            /// @brief Convert read data from input file into data objects
            /// @param command read data from the input file
            /// @param filecontext file context
            virtual void set_data(const std::shared_ptr<abase::BaseCommand>& command, const std::string& filecontext);

            /// @brief Collection of fatigue laws
            std::vector<std::shared_ptr<FatigueLaw>> laws;

        public:            
            FatigueLawCollector() = default;
            ~FatigueLawCollector() = default;

            /// @brief Get the fatigue law associated to a given identification
            /// @param law_id identification of the fatigue law
            /// @param code construction code of the fatigue law
            /// @param edition construction code edition of the fatigue law
            /// @return shared pointer to the fatigue law
            std::shared_ptr<FatigueLaw> get_law(const std::string& law_id, 
                                                const std::string& code, const std::string& edition) const;

            /// @brief Clear the collection of fatigue laws
            void clear() { laws.clear(); }
    };
}