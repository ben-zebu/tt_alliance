#include "DrainMaterial.h"

using namespace adata::parts;

DrainMaterial& DrainMaterial::operator=(const DrainMaterial& material) {
    if (this != &material) {
        BaseMaterial::operator=(material);
        this->stress_intensities = material.stress_intensities;
        this->stress_ratio = material.stress_ratio;
        this->stress_limit = material.stress_limit;
    }
    return *this;
}

void DrainMaterial::init(const std::shared_ptr<abase::BaseCommand>& command) {
    BaseMaterial::init(command);
    abase::get_child_value(command, "STRESS_RATIO", stress_ratio);
    abase::get_child_value(command, "STRESS_LIMIT", stress_limit);

    std::vector<double> xvalues, yvalues;
    abase::get_child_values(command, "ANGLES", xvalues);
    abase::get_child_values(command, "STRESS_INTENSITIES", yvalues);
    if (xvalues.size() > 0 && yvalues.size() > 0) stress_intensities = amath::Table(xvalues, yvalues);
}

void DrainMaterial::verify(const std::string& filecontext) const {
    BaseMaterial::verify(filecontext);
    if (stress_limit <= 0.) {
        file_input_error(translate("ERROR_DRAIN_MATERIAL_LIMIT", material_id), filecontext);
    }
    if (stress_intensities.size() > 0 && stress_ratio <= 0.) {
        file_input_error(translate("ERROR_DRAIN_MATERIAL_RATIO", material_id), filecontext);
    }
}