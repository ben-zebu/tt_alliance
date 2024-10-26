#
# Main Makefile for ALLIANCE project
# 
include config.mk

XML_DIR = $(PROJECT_DIR)/externals/xml
ABASE_DIR = $(PROJECT_DIR)/components/abase
MAIN_DIR = $(PROJECT_DIR)/components/main

# Default target
all: release debug

release: $(RELEASE_DIR) $(RELEASE_LIB_DIR) $(RELEASE_INC_DIR)
	@(cd $(XML_DIR) && $(MAKE) release)
	@(cd $(ABASE_DIR) && $(MAKE) release)
	@(cd $(MAIN_DIR) && $(MAKE) release)

debug: $(DEBUG_DIR) $(DEBUG_LIB_DIR) $(DEBUG_INC_DIR)
	@(cd $(XML_DIR) && $(MAKE) debug)
	@(cd $(ABASE_DIR) && $(MAKE) debug)
	@(cd $(MAIN_DIR) && $(MAKE) debug)

clean:
	@(cd $(XML_DIR) && $(MAKE) clean)
	@(cd $(ABASE_DIR) && $(MAKE) clean)
	$(RM) $(DEBUG_DIR) $(RELEASE_DIR)

# Create necessary directories
$(RELEASE_DIR):
	$(MKDIR) $(RELEASE_DIR)

$(RELEASE_LIB_DIR): $(RELEASE_DIR)
	$(MKDIR) $(RELEASE_LIB_DIR)

$(RELEASE_INC_DIR): $(RELEASE_DIR)
	$(MKDIR) $(RELEASE_INC_DIR)

$(DEBUG_DIR):
	$(MKDIR) $(DEBUG_DIR)

$(DEBUG_LIB_DIR): $(DEBUG_DIR)
	$(MKDIR) $(DEBUG_LIB_DIR)

$(DEBUG_INC_DIR): $(DEBUG_DIR)
	$(MKDIR) $(DEBUG_INC_DIR)
