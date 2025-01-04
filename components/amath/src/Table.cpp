#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "List.h"
#include "Table.h"

using namespace amath;

//
// Public methods
//

Table::Table(size_t size) {
    xvalues.resize(size);
    yvalues.resize(size);
}

Table::Table(const std::vector<double>& abciss, const std::vector<double>& ordinates) {
    xvalues = abciss;
    yvalues = ordinates;
    _check_();
}

Table::Table(const Table& table) {
    xvalues = table.xvalues;
    yvalues = table.yvalues;
}

Table& Table::operator=(const Table& table) {
    if (this != &table) {
        xvalues = table.xvalues;
        yvalues = table.yvalues;
    }
    return *this;
}

void Table::expand(const std::vector<double>& abciss, const std::vector<double>& ordinates) {
    std::vector<double> xvalues_tmp = amath::merge_and_sort_unique(xvalues, abciss);
    std::vector<double> yvalues_tmp(xvalues_tmp.size(), 0.);

    for (std::size_t i=0; i < xvalues_tmp.size(); ++i) {
        // find the index of the abciss value in the original table
        auto it_ori = std::find(xvalues.begin(), xvalues.end(), xvalues_tmp[i]);
        std::size_t index = std::distance(xvalues_tmp.begin(), it_ori);        
        if (index < xvalues.size()) {
            yvalues_tmp[i] = yvalues[index];
            continue;
        }

        // find the index of the abciss value in the new values
        auto it_new = std::find(abciss.begin(), abciss.end(), xvalues_tmp[i]);
        index = std::distance(abciss.begin(), it_new);
        if (index < abciss.size()) {
            yvalues_tmp[i] = ordinates[index];
        }
    }

    // update the table
    xvalues = xvalues_tmp;
    yvalues = yvalues_tmp;
    _check_();
}

std::vector<double> Table::get_xrange() const {
    std::vector<double> xrange;
    xrange.push_back(get_xmin());
    xrange.push_back(get_xmax());
    return xrange;
}

std::vector<double> Table::get_yrange() const {
    std::vector<double> yrange;
    yrange.push_back(get_ymin());
    yrange.push_back(get_ymax());
    return yrange;
}

double Table::get_yvalue(double x, const std::string& method) const {
    if (method == "linear") {
        return get_linear_yvalue(x);
    } else if (method == "logarithmic") {
        return get_logarithmic_yvalue(x);
    } else {
        throw std::runtime_error("Invalid interpolation method");
    }
}

double Table::get_xvalue(double y, const std::string& method) const {
    if (method == "linear") {
        return get_linear_xvalue(y);
    } else if (method == "logarithmic") {
        return get_logarithmic_xvalue(y);
    } else {
        throw std::runtime_error("Invalid interpolation method");
    }
}


//
// Private methods
//

double Table::get_max_value(const std::vector<double>& values) const {
    double vmax = std::numeric_limits<double>::min();
    for (auto v : values) {
        vmax = std::max(vmax, v);
    }
    return vmax;
}

double Table::get_min_value(const std::vector<double>& values) const {
    double vmin = std::numeric_limits<double>::max();
    for (auto v : values) {
        vmin = std::min(vmin, v);
    }
    return vmin;
}

double Table::get_linear_yvalue(double x) const {
    _check_();

    double xmin = get_xmin();
    double xmax = get_xmax();    
    if (x < xmin || x > xmax) {
        throw std::runtime_error("Invalid abciss value");
    }

    for (std::size_t i = 0; i < xvalues.size() - 1; ++i) {
        xmax = std::max(xvalues[i], xvalues[i + 1]);
        xmin = std::min(xvalues[i], xvalues[i + 1]);

        if (xmin <= x && x <= xmax) {
            if (xmax == xmin) { std::runtime_error("Infinite slope"); }
            double slope = (yvalues[i + 1] - yvalues[i]) / (xvalues[i + 1] - xvalues[i]);
            return yvalues[i] + slope * (x - xvalues[i]);
        }
    }
    throw std::runtime_error("Invalid abciss value");
}

double Table::get_linear_xvalue(double y) const {
    _check_();

    double ymin = get_ymin();
    double ymax = get_ymax();    
    if (y < ymin || y > ymax) {
        throw std::runtime_error("Invalid ordinate value");
    }

    for (std::size_t i = 0; i < yvalues.size() - 1; ++i) {
        ymax = std::max(yvalues[i], yvalues[i + 1]);
        ymin = std::min(yvalues[i], yvalues[i + 1]);

        if (ymin <= y && y <= ymax) {
            if (ymin == ymax) { std::runtime_error("Infinite slope"); }
            double slope = (xvalues[i + 1] - xvalues[i]) / (yvalues[i + 1] - yvalues[i]);
            return xvalues[i] + slope * (y - yvalues[i]);
        }
    }
    throw std::runtime_error("Invalid ordinate value");
}

double Table::get_logarithmic_yvalue(double x) const {
    _check_();

    double xmin = get_xmin();
    double xmax = get_xmax();
    if (x < std::max(xmin, 0.) || x > xmax) {
        throw std::runtime_error("Invalid abciss value");
    }

    for (std::size_t i = 0; i < xvalues.size() - 1; ++i) {
        xmax = std::max(xvalues[i], xvalues[i + 1]);
        xmin = std::min(xvalues[i], xvalues[i + 1]);

        if (xmin <= x && x <= xmax) {
            if (xmax == xmin) { std::runtime_error("Infinite slope"); }
            double slope = (std::log(yvalues[i + 1]) - std::log(yvalues[i]));
            slope /= (std::log(xvalues[i + 1]) - std::log(xvalues[i]));
            return std::exp(std::log(yvalues[i]) + slope * (std::log(x) - std::log(xvalues[i])));
        }
    }
    throw std::runtime_error("Invalid abciss value");
}

double Table::get_logarithmic_xvalue(double y) const {
    _check_();

    double ymin = get_ymin();
    double ymax = get_ymax();
    if (y < std::max(ymin, 0.) || y > ymax) {
        throw std::runtime_error("Invalid ordinate value");
    }

    for (std::size_t i = 0; i < yvalues.size() - 1; ++i) {
        ymax = std::max(yvalues[i], yvalues[i + 1]);
        ymin = std::min(yvalues[i], yvalues[i + 1]);

        if (ymin <= y && y <= ymax) {
            if (ymin == ymax) { std::runtime_error("Infinite slope"); }
            double slope = (std::log(xvalues[i + 1]) - std::log(xvalues[i]));
            slope /= (std::log(yvalues[i + 1]) - std::log(yvalues[i]));
            return std::exp(std::log(xvalues[i]) + slope * (std::log(y) - std::log(yvalues[i])));
        }
    }
    throw std::runtime_error("Invalid ordinate value");
}

void Table::_check_() const {
    if (xvalues.empty() or yvalues.empty()) {
        throw std::runtime_error("Table is empty");
    }
    if (xvalues.size() != yvalues.size()) {
        throw std::runtime_error("xvalues and yvalues must have the same size");
    }
}