# Compiler and flags
CXX = g++
CXXFLAGS_COMMON = -fPIC -Wall -Wextra
CXXFLAGS_DEBUG = $(CXXFLAGS_COMMON) -g -DDEBUG
CXXFLAGS_RELEASE = $(CXXFLAGS_COMMON) -O3 -DNDEBUG

# Library name and version
LIB_NAME = libtinyxml2
VERSION = 10.0.0
SONAME = $(LIB_NAME).so.$(VERSION)

# Directories
SRC_DIR = src
OBJ_DIR_DEBUG = obj/debug
OBJ_DIR_RELEASE = obj/release
LIB_DIR = lib

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS_DEBUG = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR_DEBUG)/%.o)
OBJS_RELEASE = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR_RELEASE)/%.o)

# Default target
all: release

# Create necessary directories
$(OBJ_DIR_DEBUG) $(OBJ_DIR_RELEASE) $(LIB_DIR):
	mkdir -p $@

# Debug target
debug: CXXFLAGS = $(CXXFLAGS_DEBUG)
debug: $(OBJ_DIR_DEBUG) $(LIB_DIR) $(OBJS_DEBUG)
	$(CXX) -shared -Wl,-soname,$(SONAME) -o $(LIB_DIR)/$(SONAME).debug $(OBJS_DEBUG)
	cd $(LIB_DIR) && ln -sf $(SONAME).debug $(LIB_NAME).so

# Release target
release: CXXFLAGS = $(CXXFLAGS_RELEASE)
release: $(OBJ_DIR_RELEASE) $(LIB_DIR) $(OBJS_RELEASE)
	$(CXX) -shared -Wl,-soname,$(SONAME) -o $(LIB_DIR)/$(SONAME) $(OBJS_RELEASE)
	cd $(LIB_DIR) && ln -sf $(SONAME) $(LIB_NAME).so

# Debug object files
$(OBJ_DIR_DEBUG)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Release object files
$(OBJ_DIR_RELEASE)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(OBJ_DIR_DEBUG) $(OBJ_DIR_RELEASE) $(LIB_DIR)

.PHONY: all debug release clean
