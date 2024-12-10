#include <algorithm>

#include "ProblemTable.h"

using namespace adata;

void ProblemTable::init(const std::shared_ptr<abase::BaseCommand>& command, std::size_t id) {

    // get the name of the table
    name = "";
    command->get_value(name);
    if (name.empty()) default_name(id);

    // get the table values
    std::vector<double> values;
    command->get_values(values);

    std::vector<double> abciss;
    std::vector<double> ordinates;
    for (std::size_t i = 0; i < values.size()/2; i++) {
        std::size_t j = 2*i;
        abciss.push_back(values[j]);
        ordinates.push_back(values[j+1]);
    }
    table = amath::Table(abciss, ordinates);

}

void ProblemTable::verify(const std::string& filecontext) const {
    // check if the table abciss is ordered
    std::vector<double> abciss = table.get_xrange();
    if (!std::is_sorted(abciss.begin(), abciss.end())) {
        std::string msg = translate("ERROR_TABLE_UNORDERED_ABCISS", name);
        file_input_error(msg, filecontext);
    }
}