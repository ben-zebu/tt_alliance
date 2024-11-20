# read compiler flags from config.mk
include ../../config.mk

# program name
PROG_NAME = alliance.exe

# Directories
SRC_DIR = src

# Eigen header files
EIGEN_INCLUDE = -I$(PROJECT_DIR)/externals/eigen

# Libraries
LIBS = -ltinyxml2 -labase -lamath

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
debug: CXXFLAGS = $(CXXFLAGS_DEBUG) $(EIGEN_INCLUDE)
debug: LDFLAGS = -L$(DEBUG_LIB_DIR) 
debug: $(DEBUG_DIR)/$(PROG_NAME)

$(DEBUG_DIR)/$(PROG_NAME): $(OBJ_DIR_DEBUG) $(OBJS_DEBUG)
	$(CXX) -o $(DEBUG_DIR)/$(PROG_NAME) $(OBJS_DEBUG) $(LDFLAGS) $(LIBS) 

# Release target
release: CXXFLAGS = $(CXXFLAGS_RELEASE) $(EIGEN_INCLUDE)
release: LDFLAGS = -L$(RELEASE_LIB_DIR) 
release: $(RELEASE_DIR)/$(PROG_NAME)

$(RELEASE_DIR)/$(PROG_NAME): $(OBJ_DIR_RELEASE) $(OBJS_RELEASE)
	$(CXX) -o $(RELEASE_DIR)/$(PROG_NAME) $(OBJS_RELEASE) $(LDFLAGS) $(LIBS) 

# Debug object files
$(OBJ_DIR_DEBUG)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ && $(CP) ${<:.cpp=.h} $(DEBUG_INC_DIR) || true

# Release object files
$(OBJ_DIR_RELEASE)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@  && $(CP) ${<:.cpp=.h} $(RELEASE_INC_DIR) || true

# Dependancy rules for Debug
-include $(OBJS_DEBUG:.o=.d)

# Dependancy rules for Debug
-include $(OBJS_RELEASE:.o=.d)

# Clean target
clean:
	$(RM) $(OBJ_DIR_DEBUG)    $(DEBUG_LIB_DIR)/$(SONAME)
	$(RM) $(OBJ_DIR_RELEASE)  $(RELEASE_LIB_DIR)/$(SONAME)

.PHONY: all debug release clean
