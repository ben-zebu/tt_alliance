#include "ProblemTorsor.h"

using namespace adata::parts;

void ProblemTorsor::init(const std::shared_ptr<abase::BaseCommand>& command) {
    command->get_values(cards);
    
    // convert to 0-based index
    for (std::size_t i = 0; i < cards.size(); ++i) cards[i] -= 1;
}

void ProblemTorsor::verify(const std::string& filecontext) const {
    // nothing to verify
}