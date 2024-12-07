#include "ProblemTorsor.h"

using namespace adata;

void ProblemTorsor::init(std::shared_ptr<abase::BaseCommand> command) {
    command->get_values(cards);
}

void ProblemTorsor::verify(std::string& filecontext) const {
    // nothing to verify
    std::cout << "ProblemTorsor::verify cards:";
    for (const auto& card : cards) std::cout << " " << card;
    std::cout << std::endl;
}