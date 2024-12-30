#include <algorithm>
#include <cmath>
#include <limits>

#include "FatigueLaw.h"

using namespace adata::parts;

//
// Fatigue Law
//

void FatigueLaw::general_verification(const std::string& filecontext) const {
    if (law_id.empty()) {
        file_input_error(translate("ERROR_FATIGUE_LAW_ID"), filecontext);
    }
    if (Ec <= 0.) {
        file_input_error(translate("ERROR_FATIGUE_LAW_PARAMETER", {"Eref", law_id}), filecontext);
    }
    if (N_ratio <= 0.) {
        file_input_error(translate("ERROR_FATIGUE_LAW_PARAMETER", {"N_ratio", law_id}), filecontext);
    }
}

bool FatigueLaw::support_code(const std::string& code, const std::string& edition) const { 
    // case for experimental fatigue laws without code link
    std::size_t nb_codes = 0;
    for (const auto& known_code : {"rccm", "asme"}) {
        if (descriptions.find(known_code) != descriptions.end()) ++nb_codes;
    }
    if (nb_codes == 0) return true;

    // if code is empty, the law is supported
    if (code.empty()) return true;
    // if code is defined but not in descriptions list, the law is not supported
    if (descriptions.find(str::lowercase(code)) == descriptions.end()) return false;
    // if code is defined and in descriptions list, but description is empty, the law is supported
    if (edition.empty()) return true;
    // if code is defined and in descriptions list, and description is defined, check if it is in the list
    for (const auto& version : editions) {
        if (version.first == str::lowercase(code)) {
            return std::find(version.second.begin(), version.second.end(), edition) != version.second.end();
        }
    }
    return false;
}

bool FatigueLaw::is_same_law(const FatigueLaw& other) const {
    if (law_id != other.law_id) return false;

    for (const auto& code : {"rccm", "asme"}) {
        if (descriptions.find(code) == descriptions.end()) continue;
        for (const auto& version : editions.at(code)) {
            if (other.support_code(code, version)) return true;
        }
    }
    return false;
}

void FatigueLaw::set_generic_parameters(const std::shared_ptr<abase::BaseCommand>& command) {
    abase::get_child_value(command, "LAW_ID", law_id);
    abase::get_child_value(command, "EREF", Ec);
    abase::get_child_value(command, "N_RATIO", N_ratio);

    // single case description
    this->descriptions.clear();
    std::string title = ""; 
    abase::get_child_value(command, "DESCRIPTION", title);
    if (!title.empty()) this->descriptions["description"] = title;
    
    // code case description
    for (const auto& code : {"rccm", "asme"}) {
        auto code_cmd = command->get_child(str::uppercase(code));
        if (code_cmd == nullptr) continue;
        
        std::string description = "";
        abase::get_child_value(code_cmd, "DESCRIPTION", description);
        if (!description.empty()) this->descriptions[str::lowercase(code)] = description;

        std::vector<std::string> editions;
        abase::get_child_values(code_cmd, "EDITION", editions);
        if (editions.size() > 0) this->editions[str::lowercase(code)] = editions;
    }
    
}   

void FatigueLaw::copy_generic_parameters(std::shared_ptr<FatigueLaw> other) const {
    other->law_id = law_id;
    other->Ec = Ec;
    other->N_ratio = N_ratio;
    other->descriptions = descriptions;
    other->editions = editions;
}

//
// Tabular Fatigue Law
//

double TabularFatigueLaw::allowable_stress(std::size_t Na) const {
    double na = Na*N_ratio;
    if (na > table.get_xmax()) return 0.;
    if (na < table.get_xmin()) {
        std::string str_Na = std::to_string(Na);
        std::string str_Sa = std::to_string(table.get_ymax());
        warning(translate("FATIGUE_STRESS_INTERPOLATION_WARNING", {str_Na, str_Sa}));
        return table.get_ymax();
    }
    return table.get_yvalue(na, "logarithmic");    
}

std::size_t TabularFatigueLaw::allowable_cycles(double Sa) const {
    if (Sa < table.get_ymin()) return std::numeric_limits<std::size_t>::max();
    if (Sa > table.get_ymax()) {
        std::string str_Sa = std::to_string(Sa);
        warning(translate("FATIGUE_CYCLE_INTERPOLATION_WARNING", str_Sa));
        return 1.;
    }
    double na = table.get_xvalue(Sa, "logarithmic")/N_ratio;
    return std::size_t(std::ceil(na));
}

void TabularFatigueLaw::init(const std::shared_ptr<abase::BaseCommand>& command) {
    set_generic_parameters(command);
    
    std::vector<double> xvalues, yvalues;
    abase::get_child_values(command, "NA", xvalues);
    abase::get_child_values(command, "SA", yvalues);

    if (xvalues.size() > 0 && yvalues.size() > 0) table = amath::Table(xvalues, yvalues);
}

void TabularFatigueLaw::verify(const std::string& filecontext) const {
    general_verification(filecontext);
    if (table.size() < 2) {
        file_input_error(translate("ERROR_FATIGUE_LAW_TABLE_SIZE", law_id), filecontext);
    }
    if (table.get_xmin() <= 1.) {
        file_input_error(translate("ERROR_FATIGUE_LAW_TABLE_XMIN", law_id), filecontext);
    }
    if (table.get_ymin() <= 0.) {
        file_input_error(translate("ERROR_FATIGUE_LAW_TABLE_YMIN", law_id), filecontext);
    }
}

std::shared_ptr<FatigueLaw> TabularFatigueLaw::clone() const {
    std::shared_ptr<TabularFatigueLaw> a_clone = std::make_shared<TabularFatigueLaw>();
    // generic parameters
    copy_generic_parameters(a_clone);

    // specific parameters
    a_clone->table = table;

    return a_clone;
}

//
// Power Fatigue Law
//

double PowerFatigueLaw::allowable_stress(std::size_t Na) const {
    double na = Na*N_ratio;
    return std::pow(alpha/na, 1./beta);
}

std::size_t PowerFatigueLaw::allowable_cycles(double Sa) const {
    double na = alpha/std::pow(Sa, beta)/N_ratio;
    return std::max(na, 1.);
}

void PowerFatigueLaw::init(const std::shared_ptr<abase::BaseCommand>& command) {
    // Young modulus is not used in this law
    Ec = UNSET_YOUNG_MODULUS;

    set_generic_parameters(command);
    
    abase::get_child_value(command, "SEQ", Seq);
    abase::get_child_value(command, "ALPHA", alpha);
    abase::get_child_value(command, "BETA", beta);
}

void PowerFatigueLaw::verify(const std::string& filecontext) const {
    general_verification(filecontext);
    if (Seq <= 0.) {
        file_input_error(translate("ERROR_FATIGUE_LAW_PARAMETER", {"Seq", law_id}), filecontext);
    }
    if (alpha <= 0.) {
        file_input_error(translate("ERROR_FATIGUE_LAW_PARAMETER", {"alpha", law_id}), filecontext);
    }
    if (beta <= 0.) {
        file_input_error(translate("ERROR_FATIGUE_LAW_PARAMETER", {"beta", law_id}), filecontext);
    }
}

std::shared_ptr<FatigueLaw> PowerFatigueLaw::clone() const {
    std::shared_ptr<PowerFatigueLaw> a_clone = std::make_shared<PowerFatigueLaw>();
    // generic parameters
    copy_generic_parameters(a_clone);
    // specific parameters
    a_clone->Seq = Seq;
    a_clone->alpha = alpha;
    a_clone->beta = beta;

    return a_clone;
}


//
// Polynomial Fatigue Law
//

double PolynomialFatigueLaw::allowable_stress(std::size_t Na) const {
    double na = Na*N_ratio;
    double Sa = 0.;
    for (std::size_t i = 0; i < coefficients.size(); ++i) {
        Sa += coefficients[i]*std::pow(na, i);
    }
    return Sa;
}

std::size_t PolynomialFatigueLaw::allowable_cycles(double Sa) const {
    /*
    // Create the polynomial coefficients for the equation Sa = c0 + c1*na + c2*na^2 + ... + cn*na^n
    Eigen::VectorXd poly_coeffs(coefficients.size());
    for (std::size_t i = 0; i < coefficients.size(); ++i) {
        poly_coeffs[i] = coefficients[i];
    }
    // Subtract Sa from the constant term to set up the equation Sa - (c0 + c1*na + c2*na^2 + ... + cn*na^n) = 0
    poly_coeffs[0] -= Sa; 

    // Solve for the roots of the polynomial
    Eigen::PolynomialSolver<double, Eigen::Dynamic> solver;
    solver.compute(poly_coeffs);

    // Find the smallest positive root
    std::size_t allowable_cycles = 0;
    for (const auto& root : solver.roots()) {
        if (root.real() > 0 && std::abs(root.imag()) < std::numeric_limits<double>::epsilon()) {
            double na = root.real() / N_ratio;
            if (na > 0) {
                allowable_cycles = std::min(allowable_cycles, std::size_t(std::ceil(na)));
            }
        }
    }
    return allowable_cycles;    
    */
   return 0;
}

void PolynomialFatigueLaw::init(const std::shared_ptr<abase::BaseCommand>& command) {
    // Young modulus is not used in this law
    Ec = UNSET_YOUNG_MODULUS;

    set_generic_parameters(command);

    abase::get_child_value(command, "BETA", beta);
    abase::get_child_value(command, "SEQ", Seq);
    abase::get_child_values(command, "COEFFICIENTS", coefficients);
}

void PolynomialFatigueLaw::verify(const std::string& filecontext) const {
    general_verification(filecontext);
    if (Seq <= 0.) {
        file_input_error(translate("ERROR_FATIGUE_LAW_PARAMETER", {"Seq", law_id}), filecontext);
    }
}

std::shared_ptr<FatigueLaw> PolynomialFatigueLaw::clone() const {
    std::shared_ptr<PolynomialFatigueLaw> a_clone = std::make_shared<PolynomialFatigueLaw>();
    // generic parameters
    copy_generic_parameters(a_clone);
    // specific parameters
    a_clone->Seq = Seq;
    a_clone->coefficients = coefficients;

    return a_clone;
}