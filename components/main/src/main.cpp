#include "MechanicalProblem.h"
#include "Initiate.h"

int main(int argc, char* argv[]) {
    // initialization process
    init::start(argc, argv);

    amech::MechanicalProblem problem = amech::MechanicalProblem();
    problem.init();
    problem.verify();
    problem.solve();
    problem.close();

    return 0;
}