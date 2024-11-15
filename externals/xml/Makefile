# read compiler flags from config.mk
include ../../config.mk

# Library name and version
LIB_NAME = libtinyxml2
SONAME = $(LIB_NAME).so

# Directories
SRC_DIR = src

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
OBJS_DEBUG = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR_DEBUG)/%.o)
OBJS_RELEASE = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR_RELEASE)/%.o)

# Default target
all: release debug

# Create necessary directories
$(OBJ_DIR_DEBUG) $(OBJ_DIR_RELEASE):
	$(MKDIR) $@

# Debug target
debug: CXXFLAGS = $(CXXFLAGS_DEBUG)
debug: $(DEBUG_LIB_DIR)/$(SONAME)

$(DEBUG_LIB_DIR)/$(SONAME): $(OBJ_DIR_DEBUG) $(OBJS_DEBUG)
	$(CXX) -shared -Wl,-soname,$(SONAME) -o $(DEBUG_LIB_DIR)/$(SONAME) $(OBJS_DEBUG)

# Release target
release: CXXFLAGS = $(CXXFLAGS_RELEASE)
release: $(RELEASE_LIB_DIR)/$(SONAME)

$(RELEASE_LIB_DIR)/$(SONAME): $(OBJ_DIR_RELEASE) $(OBJS_RELEASE)
	$(CXX) -shared -Wl,-soname,$(SONAME) -o $(RELEASE_LIB_DIR)/$(SONAME) $(OBJS_RELEASE)

# Debug object files
$(OBJ_DIR_DEBUG)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ && $(CP) ${<:.cpp=.h} $(DEBUG_INC_DIR) || true

# Release object files
$(OBJ_DIR_RELEASE)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@  && $(CP) ${<:.cpp=.h} $(RELEASE_INC_DIR) || true

# Clean target
clean:
	$(RM) $(OBJ_DIR_DEBUG)    $(DEBUG_LIB_DIR)/$(SONAME)
	$(RM) $(OBJ_DIR_RELEASE)  $(RELEASE_LIB_DIR)/$(SONAME)

.PHONY: all debug release clean
