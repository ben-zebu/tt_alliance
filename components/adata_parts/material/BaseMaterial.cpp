#include "BaseMaterial.h"

using namespace adata::parts;

BaseMaterial& BaseMaterial::operator=(const BaseMaterial& material) {
    if (this != &material) {
        material_id = material.material_id;
        fatigue_law_id = material.fatigue_law_id;
        steel_type = material.steel_type;
        correction = material.correction;
        Kf = material.Kf;
        m = material.m;
        n = material.n;
    }
    return *this;
}


void BaseMaterial::init(const std::shared_ptr<abase::BaseCommand>& command) {
    abase::get_child_value(command, "MATERIAL_ID", material_id);
    abase::get_child_value(command, "FATIGUE_LAW_ID", fatigue_law_id);
    abase::get_child_value(command, "STEEL_TYPE", steel_type);
    abase::get_child_value(command, "CORRECTION", correction);
    abase::get_child_value(command, "KF", Kf);
    abase::get_child_value(command, "M", m);
    abase::get_child_value(command, "N", n);
}

void BaseMaterial::verify(const std::string& filecontext) const {
    // TODO: Implement the verification of the object
}
