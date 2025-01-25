#include "Environment.h"

namespace abase {

    /// @brief Return the current date. The format can be YYYY-MM-DD or DD-MM-YYYY depending on language output
    /// @return current date as a string
    std::string current_date();

    /// @brief Return the computer's hostname 
    /// @return hostname as a string
    std::string get_hostname();

}