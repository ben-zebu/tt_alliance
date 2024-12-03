#pragma once

// Standard usual include 
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#define STRING_DELIMITER " \t\r\n"
#define COMMENT_DELIMITER "#;"
#define QUOTATION_DELIMITER "'\""

#include "ConfigParser.h"
#include "ErrorManager.h"
#include "TranslationManager.h"