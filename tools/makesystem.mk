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

ifndef DELPHINUS_MAKESYSTEM_MK
DELPHINUS_MAKESYSTEM_MK := 1

# Verify BASE_DIR and BUILD_DIR are set and bail out if not
ifeq ($(BASE_DIR),)
    $(error Variable $$BASE_DIR must be set, aborting...)
endif
ifeq ($(BUILD_ARCHS),)
    ifneq ($(BASE_DIR),.)
        $(error Variable $$BUILD_ARCHS must be set, aborting...)
    endif
endif
include $(BASE_DIR)/tools/toolchain.mk
include $(BASE_DIR)/tools/utils.mk

# Set the shell to bash
SHELL := /bin/bash -e

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

reverse = $(if $(1),$(call reverse,$(wordlist 2,$(words $(1)),$(1)))) $(firstword $(1))

EXPORT_TRIGGER := .export
EXPORTS_DIR := $(BASE_DIR)/dist
EXPORT_RELEASES_DIR := $(EXPORTS_DIR)/release

# Make sure all is the first target
all:

ifneq ($(ARCH),)
# When ARCH is set

# Setting up exports dir
EXPORT_HEADERS_BASE_DIR = $(EXPORTS_DIR)/$(ARCH)/include
ifneq ($(EXPORT_HEADERS_PREFIX_DIR),)
    EXPORT_HEADERS_DIR = $(EXPORT_HEADERS_BASE_DIR)/$(EXPORT_HEADERS_PREFIX_DIR)
    REMOVE_EXPORT_HEADERS_PREFIX_DIR = $(RMDIR) $(EXPORT_HEADERS_DIR)
else
    EXPORT_HEADERS_DIR = $(EXPORT_HEADERS_BASE_DIR)
endif
EXPORT_LIBS_DIR = $(EXPORTS_DIR)/$(ARCH)/lib
EXPORT_BINS_DIR = $(EXPORTS_DIR)/$(ARCH)/bin

ARCH_FLAGS :=

# Now choose the toolchain based on ARCH
ifeq ($(ARCH),$(ARCH_HOST))
    CC      := $(HOST_CC)
    CXX     := $(HOST_CXX)
    AS      := $(HOST_AS)
    AR      := $(HOST_AR)
    LD      := $(HOST_LD)
    RANLIB  := $(HOST_RANLIB)
    SIZE    := $(HOST_SIZE)
    STRIP   := $(HOST_STRIP)
    OBJCOPY := $(HOST_OBJCOPY)
    NM      := $(HOST_NM)
    ARCH_FLAGS += -fPIC
endif
ifeq ($(ARCH),$(ARCH_MINGW_W32))
    CC      := $(MINGW_W32_CC)
    CXX     := $(MINGW_W32_CXX)
    AS      := $(MINGW_W32_AS)
    AR      := $(MINGW_W32_AR)
    LD      := $(MINGW_W32_LD)
    RANLIB  := $(MINGW_W32_RANLIB)
    SIZE    := $(MINGW_W32_SIZE)
    STRIP   := $(MINGW_W32_STRIP)
    OBJCOPY := $(MINGW_W32_OBJCOPY)
    NM      := $(MINGW_W32_NM)
endif
ifeq ($(ARCH),$(ARCH_MINGW_W64))
    CC      := $(MINGW_W64_CC)
    CXX     := $(MINGW_W64_CXX)
    AS      := $(MINGW_W64_AS)
    AR      := $(MINGW_W64_AR)
    LD      := $(MINGW_W64_LD)
    RANLIB  := $(MINGW_W64_RANLIB)
    SIZE    := $(MINGW_W64_SIZE)
    STRIP   := $(MINGW_W64_STRIP)
    OBJCOPY := $(MINGW_W64_OBJCOPY)
    NM      := $(MINGW_W64_NM)
endif

# Setup compilation and linker flags
OPTIMIZATION_FLAGS := -O3
WARN_FLAGS := -W -Wall -Wextra -Wno-long-long -Winline -Winit-self -Wwrite-strings \
    -Wuninitialized -Wcast-align -Wcast-qual -Wpointer-arith -Wmissing-declarations \
    -Wmissing-include-dirs -Wshadow -Wwrite-strings
WARN_C_FLAGS := -Wold-style-declaration -Wstrict-prototypes -Wmissing-prototypes
COMMON_FLAGS := $(WARN_FLAGS) -D_REENTRANT -D__STDC_FORMAT_MACROS -pipe $(ARCH_FLAGS)
INCPATH = -I. -I$(EXPORT_HEADERS_BASE_DIR)

CFLAGS = $(OPTIMIZATION_FLAGS) $(COMMON_FLAGS) $(WARN_C_FLAGS)
CFLAGS += -std=gnu99
CXXFLAGS = $(OPTIMIZATION_FLAGS) $(COMMON_FLAGS)
CPPFLAGS = $(INCPATH)

LDFLAGS = -Wl,-O3 -Wl-z,defs
LDFLAGS += -L$(EXPORT_LIBS_DIR)

LINKER = $(if $(filter .cpp, $(suffix $(SOURCES))), $(CXX), $(CC))
LDARGS = -o $@ $(filter %.o,$^) $(LDFLAGS) $(LINKMAP)
LDARGS += $(if $(findstring $(CC), $(LINKER)),$(CFLAGS),$(CXXFLAGS))

# Commands for various types of linking
LINK = $(call ExecWithMsg, "Linking $@", $(LINKER) $(LDARGS))
LINK_C = $(call ExecWithMsg, "Linking $@", $(CC) $(LDARGS))
LINK_SHARED = $(call ExecWithMsg, "Linking Shared library $@", $(LINKER) $(LDARGS) -shared)
LINK_STATIC = $(call ExecWithMsg, "Linking Static library $@", $(AR) cru $@ $^)

# Commands for exporting, and adding exported files to cleanup list
EXPORT_ALL_CMD =
EXPORTED_FILES =
EXPORT_LIST =
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
    EXPORT_DIST := $(call ExecWithMsg, "Exporting $(EXPORT_LIST) ARCH=$(ARCH)", $(EXPORT_ALL_CMD))
endif

# Setup the build and dependecy directories
BUILD_DIR = $(ARCH)
DEP_DIR = $(BUILD_DIR)/.dep

# Clean-up files and directories
CLEANUP_FILES += $(BUILD_DIR) $(EXPORTED_FILES)

# TARGET comes in the very end after CLEANUP_TRIGGER .prereqs and
# EXPORT_TRIGGER
ifneq ($(TARGET),)
ifneq ($(EXPORT_DIST),)
local_all: $(EXPORT_TRIGGER)

$(EXPORT_TRIGGER): $(TARGET)
	$(EXPORT_DIST)
	$(silent)$(TOUCH) $@

CLEANUP_FILES += $(EXPORT_TRIGGER)
else
all: $(TARGET)
endif
endif

# Handle header file dependecies
MAKEDEPEND_C := $(CC)
MAKEDEPEND_CXX := $(CXX)
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

local_all: $(TARGET)

local_clean:
	$(call ExecWithMsg, "Cleaning `pwd` ARCH=$(ARCH)", $(RM_RECURSIVE) $(CLEANUP_FILES); $(REMOVE_EXPORT_HEADERS_PREFIX_DIR))

# End ARCH is set
else
# ARCH is not set

# Clean-up files and directories
CLEANUP_TRIGGER := .makefile
CLEANUP_FILES += $(CLEANUP_TRIGGER)
BASE_MAKEFILE := $(firstword $(MAKEFILE_LIST))

EXPORT_HEADERS_DIRS := $(foreach build_arch,$(BUILD_ARCHS),$(EXPORTS_DIR)/$(build_arch)/include)
ifneq ($(EXPORT_HEADERS_PREFIX_DIR),)
    EXPORT_HEADERS_DIRS += $(foreach build_arch,$(BUILD_ARCHS),$(EXPORTS_DIR)/$(build_arch)/include/$(EXPORT_HEADERS_PREFIX_DIR))
endif

DIRS_TO_CREATE := $(EXPORTS_DIR)
DIRS_TO_CREATE += $(foreach build_arch,$(BUILD_ARCHS),$(EXPORTS_DIR)/$(build_arch))
DIRS_TO_CREATE += $(EXPORT_HEADERS_DIRS)
DIRS_TO_CREATE += $(foreach build_arch,$(BUILD_ARCHS),$(EXPORTS_DIR)/$(build_arch)/lib)
DIRS_TO_CREATE += $(foreach build_arch,$(BUILD_ARCHS),$(EXPORTS_DIR)/$(build_arch)/bin)
DIRS_TO_CREATE += $(EXPORT_RELEASES_DIR)
$(shell $(MKDIR) $(DIRS_TO_CREATE))

BUILD_DIRS := $(BUILD_ARCHS)
DEP_DIRS := $(foreach build_arch,$(BUILD_ARCHS),$(build_arch)/.dep)

__local_all_archs = $(foreach build_arch,$(BUILD_ARCHS),.local_all__$(build_arch))
__local_clean_archs = $(foreach build_arch,$(BUILD_ARCHS),.local_clean__$(build_arch))

local_all: $(__local_all_archs)

local_clean: $(__local_clean_archs)
	$(silent)$(RM_RECURSIVE) $(CLEANUP_FILES)

.local_all__% .local_clean__%: ARCH = $(word 2,$(subst __, ,$@))

.local_all__%: $(CLEANUP_TRIGGER)
	$(silent)ARCH=$(ARCH) $(MAKE) local_all

.local_clean__%:
	$(silent)ARCH=$(ARCH) $(MAKE) local_clean

distclean: local_clean

# Make sure .prereqs is before TARGET
ifneq ($(PRE_REQS),)
all: .prereqs

.prereqs:
	$(silent)for dir in $(PRE_REQS);\
	    do $(MAKE) -C $(BASE_DIR)/$${dir};\
	    if [ $$? -ne 0 ]; then exit 1; fi\
	    done

clean:
	$(silent)for dir in $(PRE_REQS);\
	    do $(MAKE) -C $(BASE_DIR)/$${dir} local_clean;\
            if [ $$? -ne 0 ]; then exit 1; fi\
	    done

distclean: clean
	$(silent)$(RM_RECURSIVE) $(call reverse,$(DIRS_TO_CREATE))

else
distclean:
	$(silent)$(RM_RECURSIVE) $(call reverse,$(DIRS_TO_CREATE))
endif

all: local_all

$(CLEANUP_TRIGGER): $(BASE_MAKEFILE)
	$(silent)if [ -f $(CLEANUP_TRIGGER) ]; then $(MAKE) local_clean; fi
	$(silent)if [ "$(BASE_DIR)" != "." ]; then $(MKDIR) $(BUILD_DIRS) $(DEP_DIRS) $(EXPORT_HEADERS_DIRS); fi
	$(silent)$(TOUCH) $@

#End ARCH is not set
endif

# Settings for the release
RELEASE_TEMP_DIR := $(EXPORT_RELEASES_DIR)/.release_temp
DELPHINUS := delphinus
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
	@echo -e "make/make all     : Build the targets and the dependencies"
	@echo -e "make local_all    : Build the targets without checking for dependencies"
	@echo -e "make local_clean  : Clean the targets but not the dependencies"
	@echo -e "make clean        : Clean the targets and the dependencies"
	@echo -e "make distclean    : Clean the targets, the dependencies and the dist directory(completely)"
	@echo -e "make release      : Buidl the release tarballs (requires the codebase to be from an SVN repo)"
	@echo -e "make help         : Display this help message"


.PHONY: all local_all clean local_clean distclean help .prereqs release .local_all__* .local_clean__*

endif
