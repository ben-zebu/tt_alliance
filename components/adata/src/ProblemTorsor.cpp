#include "ProblemTorsor.h"

using namespace adata;

void ProblemTorsor::init(std::shared_ptr<abase::BaseCommand> command) {
    command->get_values(cards);
    
    // convert to 0-based index
    for (std::size_t i = 0; i < cards.size(); ++i) cards[i] -= 1;
}

void ProblemTorsor::verify(std::string& filecontext) const {
    // nothing to verify
}