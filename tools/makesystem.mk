#
#   makesystem.mk - common makesystem for delphinus
#
#   This file is part of delphinus - a stream analyzer for various multimedia
#   stream formats and containers.
#
#   Copyright (C) 2011 Ash (Tuxdude)
#   tuxdude.github@gmail.com
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

ifndef DELPHINUS_MAKESYSTEM_MK
DELPHINUS_MAKESYSTEM_MK := 1

# Verify BASE_DIR is set and bail out if not
ifeq ($(BASE_DIR),)
    $(error Variable $$BASE_DIR must be set, aborting...)
endif

# BUILD_ARCHS should be set for all directories other than BASE_DIR
ifeq ($(BUILD_ARCHS),)
    ifneq ($(BASE_DIR),.)
        $(error Variable $$BUILD_ARCHS must be set, aborting...)
    endif
endif

# Read the toolchain, utils and config makefiles
include $(BASE_DIR)/tools/config.mk
include $(BASE_DIR)/tools/toolchain.mk
include $(BASE_DIR)/tools/utils.mk
include $(BASE_DIR)/tools/colors.mk

# If config.mk is not proper, bail out
ifneq ($(DELPHINUS_CONFIGURED),yes)
    $(error config.mk is invalid, aborting...)
endif

# Set the shell to bash
SHELL := /bin/bash -e

# Parse the config related options
ifneq ($(VERBOSE),yes)
    silent := @
else
    silent :=
endif
ifeq ($(PEDANTIC),yes)
WARN_FLAGS  += -pedantic
endif

# Helpful functions
# ExecWithMsg
# $(1) - Message
# $(2) - Command to be executed
define ExecWithMsg
    $(silent)$(ECHO) "\n===  $(1)  ==="
    $(silent)$(2)
endef
# ColorizeExecWithMsg
# $(1) - Message
# $(2) - Command
# $(3) - Color for normal messages
define ColorizeExecWithMsg
    $(silent)$(2) 2>$(TEMP_BUILD_LOG) || $(TOUCH) $(TEMP_BUILD_ERROR)
    $(silent)if $(TEST) -e $(TEMP_BUILD_ERROR); then STATUS="$(COLOR_ERRORS)" && RC=1;\
    elif $(TEST) -s $(TEMP_BUILD_LOG); then STATUS="$(COLOR_WARNINGS)" && RC=0;\
    else STATUS="$(3)" && RC=0; fi &&\
    $(ECHO) "\n===  $${STATUS}$(1)$(COLOR_RESET)  ===" &&\
    $(CAT) $(TEMP_BUILD_LOG) 1>&2 && $(CLEANUP_TEMP_BUILD_FILES) && exit $${RC};
endef
reverse = $(if $(1),$(call reverse,$(wordlist 2,$(words $(1)),$(1)))) $(firstword $(1))

# Exports Base directory and release directory
EXPORT_BASE_DIR := $(BASE_DIR)/$(EXPORT_BASE_DIR_NAME)
EXPORT_RELEASES_DIR := $(EXPORT_BASE_DIR)/$(EXPORT_RELEASES_DIR_NAME)

# To colorize or not
ifeq ($(DISABLE_COLORS),yes)
    EXEC_USING := ExecWithMsg
else
    EXEC_USING := ColorizeExecWithMsg
endif

# Remove duplicates in BUILD_ARCHS
BUILD_ARCHS := $(sort $(BUILD_ARCHS))
# Remove any archs not supported
BUILD_ARCHS := $(filter $(ALL_ARCHS),$(BUILD_ARCHS))

# Make sure all is the first target
all:

ifneq ($(ARCH),)
# When ARCH is set

# Build and dependecy directories
BUILD_DIR := $(ARCH)
DEP_DIR := $(BUILD_DIR)/$(DEP_DIR_NAME)

# Setting up exports dir
EXPORT_TRIGGER := .export_$(ARCH)
EXPORT_HEADERS_BASE_DIR := $(EXPORT_BASE_DIR)/$(ARCH)/$(EXPORT_HEADERS_DIR_NAME)
ifneq ($(EXPORT_HEADERS_PREFIX_DIR),)
    EXPORT_HEADERS_DIR := $(EXPORT_HEADERS_BASE_DIR)/$(EXPORT_HEADERS_PREFIX_DIR)
    REMOVE_EXPORT_HEADERS_PREFIX_DIR := $(RM_RECURSIVE) $(EXPORT_HEADERS_DIR)
else
    EXPORT_HEADERS_DIR := $(EXPORT_HEADERS_BASE_DIR)
endif
EXPORT_LIBS_DIR := $(EXPORT_BASE_DIR)/$(ARCH)/$(EXPORT_LIBS_DIR_NAME)
EXPORT_BINS_DIR := $(EXPORT_BASE_DIR)/$(ARCH)/$(EXPORT_BINS_DIR_NAME)

# Temporary build log file used for colorizing the output
TEMP_BUILD_LOG := .build.log
TEMP_BUILD_ERROR := .build.error
CLEANUP_TEMP_BUILD_FILES := $(RM_RECURSIVE) $(TEMP_BUILD_LOG) $(TEMP_BUILD_ERROR)
$(shell $(CLEANUP_TEMP_BUILD_FILES))

# Setup compilation and linker flags
ARCH_FLAGS         += $(TOOLCHAIN_ARCH_FLAGS)
OPTIMIZATION_FLAGS += -O3
WARN_FLAGS         += -W -Wall -Wextra -Wno-long-long -Winline -Winit-self
WARN_FLAGS         += -Wwrite-strings -Wuninitialized -Wcast-align -Wcast-qual
WARN_FLAGS         += -Wpointer-arith -Wmissing-declarations
WARN_FLAGS         += -Wmissing-include-dirs -Wshadow -Wwrite-strings
WARN_C_FLAGS       += -Wold-style-declaration -Wstrict-prototypes -Wmissing-prototypes
COMMON_FLAGS       += $(WARN_FLAGS) -D_REENTRANT -D__STDC_FORMAT_MACROS -pipe
INC_FLAGS            += -I. -I$(EXPORT_HEADERS_BASE_DIR)

CFLAGS   += $(OPTIMIZATION_FLAGS) $(COMMON_FLAGS) $(WARN_C_FLAGS) $(TOOLCHAIN_ARCH_CFLAGS)
CFLAGS   += -std=gnu99
CXXFLAGS += $(OPTIMIZATION_FLAGS) $(COMMON_FLAGS) $(WARN_CXX_FLAGS) $(TOOLCHAIN_ARCH_CXXFLAGS)
CXXFLAGS += -std=gnu++0x
CPPFLAGS += $(INC_FLAGS)
LDFLAGS  += -Wl,-O3 -Wl-z,defs $(TOOLCHAIN_ARCH_LDFLAGS)
LDFLAGS  += -L$(EXPORT_LIBS_DIR)

LINKER := $(if $(filter .cpp, $(suffix $(SOURCES))), $(CXX), $(CC))
LDARGS = -o $@ $(filter %.o,$^) $(LDFLAGS) $(LINKMAP)
#LDARGS += $(if $(findstring $(CC), $(LINKER)),$(CFLAGS),$(CXXFLAGS))

# Commands for various types of compilation and linking
COMPILE_CXX_CMD = $(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -MMD -MF $(DEP_DIR)/$*.dep -MT $@ -o $@
COMPILE_C_CMD   = $(CC) -c $(CXXFLAGS) $(CPPFLAGS) $< -MMD -MF $(DEP_DIR)/$*.dep -MT $@ -o $@
COMPILE_CXX     = $(call $(EXEC_USING),$< -> $@,$(COMPILE_CXX_CMD),$(COLOR_COMPILE_OK))
COMPILE_C       = $(call $(EXEC_USING),$< -> $@,$(COMPILE_C_CMD),$(COLOR_COMPILE_OK))
LINK            = $(call $(EXEC_USING),Linking $@, $(LINKER) $(LDARGS),$(COLOR_LINK_OK))
LINK_C          = $(call $(EXEC_USING),Linking $@, $(CC) $(LDARGS),$(COLOR_LINK_OK))
LINK_SHARED     = $(call $(EXEC_USING),Linking Shared library $@, $(LINKER) $(LDARGS) -shared,$(COLOR_LINK_OK))
LINK_STATIC     = $(call $(EXEC_USING),Linking Static library $@, $(AR) cru $@ $^,$(COLOR_LINK_OK))

# Handle header file dependecies
# The sed commands help to remove recipes which do not have any dependencies
# at all, and few more not-very-much-required lines from the dependency file
# generated by gcc
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

# Commands for exporting, and adding exported files to cleanup list
EXPORT_ALL_CMD :=
EXPORTED_FILES :=
EXPORT_LIST :=
ifneq ($(EXPORT_HEADERS),)
    EXPORT_ALL_CMD += $(CP) $(EXPORT_HEADERS) $(EXPORT_HEADERS_DIR);
    EXPORTED_FILES += $(addprefix $(EXPORT_HEADERS_DIR)/, $(notdir $(EXPORT_HEADERS)))
    EXPORT_LIST += $(notdir $(EXPORT_HEADERS))
endif
ifneq ($(EXPORT_LIBS),)
    EXPORT_ALL_CMD += $(CP) $(EXPORT_LIBS) $(EXPORT_LIBS_DIR);
    EXPORTED_FILES += $(addprefix $(EXPORT_LIBS_DIR)/, $(notdir $(EXPORT_LIBS)))
    EXPORT_LIST += $(notdir $(EXPORT_LIBS))
endif
ifneq ($(EXPORT_BINS),)
    EXPORT_ALL_CMD += $(CP) $(EXPORT_BINS) $(EXPORT_BINS_DIR);
    EXPORTED_FILES += $(addprefix $(EXPORT_BINS_DIR)/, $(notdir $(EXPORT_BINS)))
    EXPORT_LIST += $(notdir $(EXPORT_BINS))
endif
ifneq ($(EXPORT_ALL_CMD),)
    EXPORT_DIST := $(call $(EXEC_USING),Exporting $(EXPORT_LIST) ARCH=$(ARCH),$(EXPORT_ALL_CMD),$(COLOR_EXPORT))
endif

# Clean-up files and directories
CLEANUP_FILES += $(BUILD_DIR) $(EXPORTED_FILES)

# Set up exporting files
ifneq ($(EXPORT_DIST),)
local_all: $(EXPORT_TRIGGER)

$(EXPORT_TRIGGER): $(TARGET)
	$(EXPORT_DIST)
	$(silent)$(TOUCH) $@

CLEANUP_FILES += $(EXPORT_TRIGGER)

# Exports should happen even if no target is defined which should
# already be handled by the above rule, but re-export (without a clean)
# wont happen if any of the dependencies are modified. The following rule
# should fix this problem
ifeq ($(TARGET),)
$(EXPORT_TRIGGER): $(EXPORT_HEADERS) $(EXPORT_BINS) $(EXPORT_LIBS)
endif
else
all: $(TARGET)
endif

# Rules for building object files
$(BUILD_DIR)/%.o: %.cpp
	$(COMPILE_CXX)
	$(silent)$(GENERATE_DEP_FILES)

$(BUILD_DIR)/%.o: %.c
	$(COMPILE_C)
	$(silent)$(GENERATE_DEP_FILES)

# Rules for building/cleaning only the current dir
local_all: $(TARGET)

local_clean:
	$(call $(EXEC_USING),Cleaning `pwd` ARCH=$(ARCH),$(RM_RECURSIVE) $(CLEANUP_FILES); $(REMOVE_EXPORT_HEADERS_PREFIX_DIR),$(COLOR_CLEAN))

# End ARCH is set
else
# ARCH is not set

# Clean-up files and directories
CLEANUP_TRIGGER := .makefile
CLEANUP_FILES += $(CLEANUP_TRIGGER)
BASE_MAKEFILE := $(firstword $(MAKEFILE_LIST))

# Filter out architectures which have not been configured for build
ifneq ($(ONLY_BUILD_ARCHS),)
    BUILD_ARCHS := $(filter $(ONLY_BUILD_ARCHS),$(BUILD_ARCHS))
endif

# Directories to create
EXPORT_HEADERS_DIRS := $(foreach build_arch,$(BUILD_ARCHS),$(EXPORT_BASE_DIR)/$(build_arch)/include)
ifneq ($(EXPORT_HEADERS_PREFIX_DIR),)
    EXPORT_HEADERS_DIRS += $(foreach build_arch,$(BUILD_ARCHS),$(EXPORT_BASE_DIR)/$(build_arch)/include/$(EXPORT_HEADERS_PREFIX_DIR))
endif

DIRS_TO_CREATE := $(EXPORT_BASE_DIR)
DIRS_TO_CREATE += $(foreach build_arch,$(BUILD_ARCHS),$(EXPORT_BASE_DIR)/$(build_arch))
DIRS_TO_CREATE += $(EXPORT_HEADERS_DIRS)
DIRS_TO_CREATE += $(foreach build_arch,$(BUILD_ARCHS),$(EXPORT_BASE_DIR)/$(build_arch)/lib)
DIRS_TO_CREATE += $(foreach build_arch,$(BUILD_ARCHS),$(EXPORT_BASE_DIR)/$(build_arch)/bin)
DIRS_TO_CREATE += $(EXPORT_RELEASES_DIR)
$(shell $(MKDIR) $(DIRS_TO_CREATE))

BUILD_DIRS := $(BUILD_ARCHS)
DEP_DIRS := $(foreach build_arch,$(BUILD_ARCHS),$(build_arch)/.dep)

# Rules for building for all the required architectures
__local_all_archs := $(foreach build_arch,$(BUILD_ARCHS),.local_all__$(build_arch))
__local_clean_archs := $(foreach build_arch,$(BUILD_ARCHS),.local_clean__$(build_arch))

local_all: $(__local_all_archs)

local_clean: $(__local_clean_archs)
	$(silent)$(RM_RECURSIVE) $(CLEANUP_FILES)

.local_all__% .local_clean__%: ARCH = $(word 2,$(subst __, ,$@))

.local_all__%: $(CLEANUP_TRIGGER)
	$(silent)ARCH=$(ARCH) $(MAKE) local_all

.local_clean__%:
	$(silent)ARCH=$(ARCH) $(MAKE) local_clean

# Make sure .prereqs is the first
ifneq ($(PRE_REQS),)
all: .prereqs

.prereqs:
	$(silent)for dir in $(PRE_REQS);\
	    do $(MAKE) -C $(BASE_DIR)/$${dir};\
	    if [ $$? -ne 0 ]; then exit 1; fi\
	    done

clean:
	$(silent)for dir in $(PRE_REQS);\
	    do $(MAKE) -C $(BASE_DIR)/$${dir} clean;\
            if [ $$? -ne 0 ]; then exit 1; fi\
	    done
endif

clean: local_clean

distclean: clean
	$(silent)$(RM_RECURSIVE) $(call reverse,$(DIRS_TO_CREATE))

all: local_all

$(CLEANUP_TRIGGER): $(BASE_MAKEFILE)
	$(silent)if [ -f $(CLEANUP_TRIGGER) ]; then $(MAKE) local_clean; fi
	$(silent)if [ "$(BASE_DIR)" != "." ]; then $(MKDIR) $(BUILD_DIRS) $(DEP_DIRS) $(EXPORT_HEADERS_DIRS); fi
	$(silent)$(TOUCH) $@

#End ARCH is not set
endif

ifeq ($(BASE_DIR),.)
# Settings for the release - it can be done only from the root of the tree
RELEASE_TEMP_DIR := $(EXPORT_RELEASES_DIR)/.release_temp
ifeq ($(MAKECMDGOALS),release)
    SVN_INFO_CMD := $(SVN) info $(BASE_DIR)
    SVN_INFO := $(shell $(SVN_INFO_CMD) 1>/dev/null 2>/dev/null; echo $$?)
    ifneq ($(SVN_INFO),0)
        $(error "Cannot build the release files from a non-svn repo, aborting...")
    endif
    BRANCH_NAME := $(shell $(SVN_INFO_CMD) | $(GREP) URL | $(SED) 's/^.*\/\([a-zA-Z0-9.\-]*\)$$/\1/')
endif

# The target release only builds if the entire build goes through although it
# doesn't require any files to be exported from the build itself
release: all
	$(silent)$(RM_RECURSIVE) $(RELEASE_TEMP_DIR)
	$(silent)$(MKDIR) $(RELEASE_TEMP_DIR)
	$(silent)$(SVN) export -q $(BASE_DIR) $(RELEASE_TEMP_DIR)/$(DELPHINUS)-$(BRANCH_NAME)
	@$(ECHO) "\n===  Creating release archive - $(EXPORT_RELEASES_DIR)/$(DELPHINUS)-$(BRANCH_NAME).tar.gz  ==="
	$(silent)$(CD) $(RELEASE_TEMP_DIR) && $(TAR) czf $(DELPHINUS)-$(BRANCH_NAME).tar.gz $(DELPHINUS)-$(BRANCH_NAME)
	$(silent)$(MV) $(RELEASE_TEMP_DIR)/$(DELPHINUS)-$(BRANCH_NAME).tar.gz $(EXPORT_RELEASES_DIR)
	@$(ECHO) "\n===  Creating release archive - $(EXPORT_RELEASES_DIR)/$(DELPHINUS)-$(BRANCH_NAME).tar.bz2  ==="
	$(silent)$(CD) $(RELEASE_TEMP_DIR) && $(TAR) cjf $(DELPHINUS)-$(BRANCH_NAME).tar.bz2 $(DELPHINUS)-$(BRANCH_NAME)
	$(silent)$(MV) $(RELEASE_TEMP_DIR)/$(DELPHINUS)-$(BRANCH_NAME).tar.bz2 $(EXPORT_RELEASES_DIR)
	@$(ECHO) "\n===  Creating release archive - $(EXPORT_RELEASES_DIR)/$(DELPHINUS)-$(BRANCH_NAME).tar.xz  ==="
	$(silent)$(CD) $(RELEASE_TEMP_DIR) && $(TAR) cJf $(DELPHINUS)-$(BRANCH_NAME).tar.xz $(DELPHINUS)-$(BRANCH_NAME)
	$(silent)$(MV) $(RELEASE_TEMP_DIR)/$(DELPHINUS)-$(BRANCH_NAME).tar.xz $(EXPORT_RELEASES_DIR)
	$(silent)$(RM_RECURSIVE) $(RELEASE_TEMP_DIR)

# doxy target also exists only for the root of the tree
doxy:
	$(silent)$(DOXYGEN)

# clocinfo target also exists only for the root of the tree
clocinfo:
	$(silent)$(MAKE) distclean
	$(silent)$(CLOC) --by-file-by-lang --force-lang=make,mk .

else
release doxy clocinfo:
	@$(ECHO) "Target $@ can be run only when from the root of the source tree, aborting..."
	$(silent)exit 1

endif

help:
	@$(ECHO) "make/make all     : Build the targets and the dependencies"
	@$(ECHO) "make local_all    : Build the targets without checking for dependencies"
	@$(ECHO) "make local_clean  : Clean the targets but not the dependencies"
	@$(ECHO) "make clean        : Clean the targets and the dependencies"
	@$(ECHO) "make distclean    : Clean the targets, the dependencies and the dist directory(completely)"
	@$(ECHO) "make release      : Create the release tarballs (requires the codebase to be from an SVN repo)"
	@$(ECHO) "make help         : Display this help message"


.PHONY: all local_all clean local_clean distclean help
.PHONY: release doxy clocinfo
.PHONY: .prereqs .local_all__* .local_clean__*

endif
