#
# Main options for ALLIANCE project
# 

# Main project directory:
PROJECT_DIR = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# Release directories:
RELEASE_DIR = $(PROJECT_DIR)/release
RELEASE_LIB_DIR = $(RELEASE_DIR)/lib
RELEASE_INC_DIR = $(RELEASE_DIR)/include

# Debug directories:
DEBUG_DIR = $(PROJECT_DIR)/debug
DEBUG_LIB_DIR = $(DEBUG_DIR)/lib
DEBUG_INC_DIR = $(DEBUG_DIR)/include

# Compiler and flags
CXX = g++
CXXFLAGS_COMMON = -fPIC -Wall -Wextra -MD
CXXFLAGS_DEBUG = $(CXXFLAGS_COMMON) -g -DDEBUG -I$(DEBUG_INC_DIR)
CXXFLAGS_RELEASE = $(CXXFLAGS_COMMON) -O3 -DNDEBUG -I$(RELEASE_INC_DIR)

OBJ_DIR_RELEASE = o_build
OBJ_DIR_DEBUG = og_build

# Standard shell commands
MKDIR = mkdir -p
RM = rm -rf
CP = cp -f
MV = mv
LN = ln -sf