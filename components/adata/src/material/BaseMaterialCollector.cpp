#include "BaseMaterialCollector.h"

using namespace adata;

std::shared_ptr<BaseMaterial> BaseMaterialCollector::create_material() const {
    return std::make_shared<BaseMaterial>();
}

void BaseMaterialCollector::set_data(const std::shared_ptr<abase::BaseCommand>& command, const std::string& filecontext) {
    // create the material object
    std::shared_ptr<BaseMaterial> new_material = create_material();

    // initialize the material object
    new_material->init(command);
    new_material->verify(filecontext);

    // check if the material already exists
    for (const auto& material : materials) {
        if (material->material_id == new_material->material_id) {
            std::string msg = translate("MATERIAL_CONFLICT", new_material->material_id);
            file_input_error(msg, filecontext);
        }
    }

    // add the material to the collection
    materials.push_back(new_material);
}

std::shared_ptr<BaseMaterial> BaseMaterialCollector::get_material(const std::string& material_id) const {
    for (const auto& material : materials) {
        if (material->material_id == material_id) {
            return material;
        }
    }
    return nullptr;
}
