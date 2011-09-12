#
#   makesystem.mk - common makesystem for delphinus
#
#   This file is part of delphinus - a stream analyzer for various multimedia
#   stream formats and containers.
#
#   Copyright (C) 2011 Ashwin (Tuxdude)
#   tuxdude@users.sourceforge.net
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Verify BASE_DIR and BUILD_DIR are set and bail out if not
ifeq ($(BASE_DIR),)
    $(error Variable $$BASE_DIR must be set, aborting...)
endif
ifeq ($(BUILD_DIR),)
    ifneq ($(BASE_DIR),.)
        $(error Variable $$BUILD_DIR must be set, aborting...)
    endif
endif

include $(BASE_DIR)/tools/toolchain.mk
include $(BASE_DIR)/tools/utils.mk

# Handle Verbose mode
ifeq ($(VERBOSE),)
    silent := @
else
    silent :=
endif

define ExecWithMsg
  @echo -e "\n=== $(1)  ==="
  $(silent)$(2)
endef

# Set the shell to sh
SHELL = /bin/sh

# Setting up exports dir
EXPORTS_DIR := $(BASE_DIR)/dist
EXPORT_HEADERS_BASE_DIR := $(EXPORTS_DIR)/include
ifneq ($(EXPORT_HEADERS_PREFIX_DIR),)
    EXPORT_HEADERS_DIR := $(EXPORT_HEADERS_BASE_DIR)/$(EXPORT_HEADERS_PREFIX_DIR)
    REMOVE_EXPORT_HEADERS_PREFIX_DIR := $(RMDIR) $(EXPORT_HEADERS_DIR)
else
    EXPORT_HEADERS_DIR := $(EXPORT_HEADERS_BASE_DIR)
endif
EXPORT_LIBS_DIR := $(EXPORTS_DIR)/lib
EXPORT_BINS_DIR := $(EXPORTS_DIR)/bin
EXPORT_RELEASES_DIR := $(EXPORTS_DIR)/releases
$(shell $(MKDIR) $(EXPORTS_DIR))
$(shell $(MKDIR) $(EXPORT_HEADERS_BASE_DIR))
$(shell $(MKDIR) $(EXPORT_HEADERS_DIR))
$(shell $(MKDIR) $(EXPORT_LIBS_DIR))
$(shell $(MKDIR) $(EXPORT_BINS_DIR))
$(shell $(MKDIR) $(EXPORT_RELEASES_DIR))

# Settings for the release
RELEASE_TEMP_DIR := $(EXPORT_RELEASES_DIR)/.release_temp
DELPHINUS := delphinus
ifeq ($(MAKECMDGOALS),release)
    SVN_INFO := $(shell $(SVN) info $(BASE_DIR) 2> /dev/null)
    ifeq ($(SVN_INFO),)
        $(error "Cannot build the release files from a non-svn repo, aborting...")
    endif
    BRANCH_NAME := $(shell echo $(SVN_INFO) | $(GREP) URL | $(SED) 's/^.*\/\([a-zA-Z0-9.\-]*\)$$/\1/')
endif

# Setup compilation and linker flags
OPTIMIZATION_FLAGS := -O3
COMMON_FLAGS := -Wall -W -Wno-long-long -D_REENTRANT -pipe -fPIC
INCPATH := -I. -I$(EXPORT_HEADERS_BASE_DIR)

CFLAGS := $(OPTIMIZATION_FLAGS) $(COMMON_FLAGS)
CFLAGS += -std=gnu99
CXXFLAGS := $(OPTIMIZATION_FLAGS) $(COMMON_FLAGS)
CPPFLAGS := $(INCPATH)

LDFLAGS := -Wl,-O3 -Wl-z,defs
# Check out what RPATH is
LDFLAGS += -L$(EXPORT_LIBS_DIR)

LINKER := $(if $(filter .cpp, $(suffix $(SOURCES))), $(CXX), $(CC))
LDARGS = -o $@ $(filter %.o,$^) $(LDFLAGS) $(LINKMAP)
LDARGS += $(if $(findstring $(CC), $(LINKER)),$(CFLAGS),$(CXXFLAGS))

# Commands for various types of linking
LINK = $(call ExecWithMsg, "Linking $@", $(LINKER) $(LDARGS))
LINK_C = $(call ExecWithMsg, "Linking $@", $(CC) $(LDARGS))
LINK_SHARED = $(call ExecWithMsg, "Linking Shared library $@", $(LINKER) $(LDARGS) -shared)
LINK_STATIC = $(call ExecWithMsg, "Linking Static library $@", $(AR) cru $@ $^)

# Commands for exporting, and adding exported files to cleanup list
EXPORT_ALL_CMD :=
EXPORTED_FILES :=
ifneq ($(EXPORT_HEADERS),)
    EXPORT_ALL_CMD += $(CP) $(EXPORT_HEADERS) $(EXPORT_HEADERS_DIR);
    EXPORTED_FILES += $(addprefix $(EXPORT_HEADERS_DIR)/, $(notdir $(EXPORT_HEADERS)))
endif
ifneq ($(EXPORT_LIBS),)
    EXPORT_ALL_CMD += $(CP) $(EXPORT_LIBS) $(EXPORT_LIBS_DIR);
    EXPORTED_FILES += $(addprefix $(EXPORT_LIBS_DIR)/, $(notdir $(EXPORT_LIBS)))
endif
ifneq ($(EXPORT_BINS),)
    EXPORT_ALL_CMD += $(CP) $(EXPORT_BINS) $(EXPORT_BINS_DIR);
    EXPORTED_FILES += $(addprefix $(EXPORT_BINS_DIR)/, $(notdir $(EXPORT_BINS)))
endif
ifneq ($(EXPORT_ALL_CMD),)
    EXPORT_DIST := $(call ExecWithMsg, "Exporting", $(EXPORT_ALL_CMD))
endif

# Setup the build and dependecy directories
DEFAULT_DEP_DIR := $(BUILD_DIR)/.dep
ifeq ($(DEP_DIR),)
    DEP_DIR := $(DEFAULT_DEP_DIR)
endif

# Clean-up files and directories
CLEANUP_TRIGGER := .makefile
CLEANUP_FILES += $(BUILD_DIR) $(EXPORTED_FILES) $(CLEANUP_TRIGGER)
BASE_MAKEFILE := $(firstword $(MAKEFILE_LIST))

# Make sure all is the first target
all: $(CLEANUP_TRIGGER)

# Make sure .prereqs is before TARGET
ifneq ($(PRE_REQS),)
all: .prereqs

.prereqs:
	$(silent)for dir in $(PRE_REQS); do $(MAKE) -C $(BASE_DIR)/$${dir}; done

distclean:
	$(silent)for dir in $(PRE_REQS); do $(MAKE) -C $(BASE_DIR)/$${dir} local_clean; done
	$(MAKE) local_clean
else
distclean:
	$(MAKE) local_clean
endif

# TARGET comes after CLEANUP_TRIGGER and .prereqs
ifneq ($(TARGET),)
ifneq ($(EXPORT_DIST),)
all: .export_dist $(TARGET)

.export_dist: $(TARGET)
	$(EXPORT_DIST)
else
all: $(TARGET)
endif
endif

# Handle header file dependecies
MAKEDEPEND_C := gcc
MAKEDEPEND_CXX := g++
GENERATE_DEP_FILES = $(CP) $(DEP_DIR)/$*.dep $(DEP_DIR)/$*.d; sed -e 's/\#.*//'\
    -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' <\
    $(DEP_DIR)/$*.dep >> $(DEP_DIR)/$*.d; $(RM) $(DEP_DIR)/$*.dep

ifneq ($(MAKECMDGOALS),local_clean)
ifneq ($(MAKECMDGOALS),dist_clean)
    CDEPS := $(patsubst %.c, $(DEP_DIR)/%.d, $(filter %.c, $(SOURCES)))
    CXXDEPS := $(patsubst %.cpp, $(DEP_DIR)/%.d, $(filter %.cpp, $(SOURCES)))
    ifneq ($(CDEPS),)
        -include $(CDEPS)
    endif
    ifneq ($(CXXDEPS),)
        -include $(CXXDEPS)
    endif
endif
endif

# Dependencies and rules
$(BUILD_DIR)/%.o: %.cpp
	@echo -e "\n===  $< -> $@  ==="
	$(silent)$(MAKEDEPEND_CXX) $(CXXFLAGS) $(CPPFLAGS) -MM -MF $(DEP_DIR)/$*.dep -MT $@ $<
	$(silent)$(GENERATE_DEP_FILES)
	$(silent)$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.c
	@echo -e "\n===  $< -> $@  ==="
	$(silent)$(MAKEDEPEND_C) $(CFLAGS) $(CPPFLAGS) -MM -o $(DEP_DIR)/$*.dep $<
	$(silent)$(GENERATE_DEP_FILES)
	$(silent)$(CC) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

$(CLEANUP_TRIGGER): $(BASE_MAKEFILE)
	$(silent)if [ -f $(CLEANUP_TRIGGER) ]; then $(MAKE) local_clean; fi
	$(silent)if [ "$(BASE_DIR)" != "." ]; then $(MKDIR) $(BUILD_DIR);\
	    $(MKDIR) $(DEP_DIR); fi
	$(silent)$(MKDIR) $(EXPORT_HEADERS_DIR)
	$(silent)$(TOUCH) $(CLEANUP_TRIGGER)

local_all: $(TARGET)

local_clean:
	$(call ExecWithMsg, "Cleaning `pwd`", $(RM_RECURSIVE) $(CLEANUP_FILES); $(REMOVE_EXPORT_HEADERS_PREFIX_DIR))

# The target release only builds if the entire build goes through although it
# doesn't require any files to be exported from the build itself
release:
	$(silent)$(RM_RECURSIVE) $(RELEASE_TEMP_DIR)
	$(silent)$(MAKE) -C $(BASE_DIR)
	$(silent)$(MKDIR) $(RELEASE_TEMP_DIR)
	$(silent)$(SVN) export -q $(BASE_DIR) $(RELEASE_TEMP_DIR)/$(DELPHINUS)-$(BRANCH_NAME)
	@echo -e "\n===  Creating release archive - $(EXPORT_RELEASES_DIR)/$(DELPHINUS)-$(BRANCH_NAME).tar.gz  ==="
	$(silent)$(CD) $(RELEASE_TEMP_DIR) && $(TAR) czf $(DELPHINUS)-$(BRANCH_NAME).tar.gz $(DELPHINUS)-$(BRANCH_NAME)
	$(silent)$(MV) $(RELEASE_TEMP_DIR)/$(DELPHINUS)-$(BRANCH_NAME).tar.gz $(EXPORT_RELEASES_DIR)
	@echo -e "\n===  Creating release archive - $(EXPORT_RELEASES_DIR)/$(DELPHINUS)-$(BRANCH_NAME).tar.bz2  ==="
	$(silent)$(CD) $(RELEASE_TEMP_DIR) && $(TAR) cjf $(DELPHINUS)-$(BRANCH_NAME).tar.bz2 $(DELPHINUS)-$(BRANCH_NAME)
	$(silent)$(MV) $(RELEASE_TEMP_DIR)/$(DELPHINUS)-$(BRANCH_NAME).tar.bz2 $(EXPORT_RELEASES_DIR)
	@echo -e "\n===  Creating release archive - $(EXPORT_RELEASES_DIR)/$(DELPHINUS)-$(BRANCH_NAME).tar.xz  ==="
	$(silent)$(CD) $(RELEASE_TEMP_DIR) && $(TAR) cJf $(DELPHINUS)-$(BRANCH_NAME).tar.xz $(DELPHINUS)-$(BRANCH_NAME)
	$(silent)$(MV) $(RELEASE_TEMP_DIR)/$(DELPHINUS)-$(BRANCH_NAME).tar.xz $(EXPORT_RELEASES_DIR)
	$(silent)$(RM_RECURSIVE) $(RELEASE_TEMP_DIR)

help:
	@echo -e "Buildable targets: $(TARGET)"
	@echo -e "make/make all     : Build the targets and the dependencies"
	@echo -e "make local_all    : Build the targets without checking for dependencies"
	@echo -e "make local_clean  : Clean the targets but not the dependencies"
	@echo -e "make distclean    : Clean the targets and the dependencies"
	@echo -e "make help         : Display this help message"


.PHONY: all local_all local_clean distclean help .prereqs .export_dist release

