#include "FatigueLawCollector.h"

using namespace adata;

std::shared_ptr<FatigueLaw> FatigueLawCollector::create_law(const std::string& type) const {
    if (type == "TABULAR_FATIGUE_LAW") return std::make_shared<TabularFatigueLaw>();
    if (type == "POWER_FATIGUE_LAW") return std::make_shared<PowerFatigueLaw>();
    if (type == "POLYNOMIAL_FATIGUE_LAW") return std::make_shared<PolynomialFatigueLaw>();
    return nullptr;
}

void FatigueLawCollector::set_data(const std::shared_ptr<abase::BaseCommand>& command, const std::string& filecontext) {
    std::string name = command->get_name();

    // create the fatigue law object
    std::shared_ptr<FatigueLaw> new_law = create_law(name);
    if (new_law == nullptr) {
        file_input_error(translate("FATIGUE_LAW_TYPE_UNKNOWN", name), filecontext);
    }

    // initialize the fatigue law object
    new_law->init(command);
    new_law->verify(filecontext);

    // check if the law already exists
    for (const auto& law : laws) {
        if (law->is_same_law(*new_law)) {
            std::string msg = translate("FATIGUE_LAW_CONFLICT", new_law->get_id());
            file_input_error(msg, filecontext);
        }
    }

    // add the fatigue law to the collection
    laws.push_back(new_law);
}

std::shared_ptr<FatigueLaw> FatigueLawCollector::get_law(const std::string& law_id, const std::string& code="", 
                                                         const std::string& edition="") const { 
    for (const auto& law : laws) {
        if (law->get_id() == law_id && law->support_code(code, edition)) return law;
    }
    return nullptr;
}

