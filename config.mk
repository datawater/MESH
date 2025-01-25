CFLAGS_WARNINGS = -Wall -Wextra -Werror -pedantic -Wstrict-aliasing \
				  -Wno-unused-command-line-argument -Wno-reorder -Wold-style-cast \
				  -Wno-variadic-macros -Wno-deprecated-copy
CFLAGS = $(CFLAGS_WARNINGS) -std=c++20 -Isrc/
LIBS =

GLIBC_VERSION := $(shell getconf GNU_LIBC_VERSION | tail -c +7)
LRT_REQUIRED_VERSION := 2.17
LRT_IS_REQUIRED := $(shell printf "%s\n" $(LRT_REQUIRED_VERSION) $(GLIBC_VERSION) | sort | head -n1)
MOLD_EXISTS := $(shell which mold)

RELEASE_OPT_LEVEL ?= 3
PROFILE_DEBUG_CFLAGS := -ggdb -O0
PROFILE_RELEASE_CFLAGS := -O$(RELEASE_OPT_LEVEL) -s -flto -mtune=native -march=native -fgraphite-identity -D_FORTIFY_SOURCE
PROFILE_SIZE_CFLAGS := -Oz -s
PROFILE_CHECK_CFLAGS := -fanalyzer -fdiagnostics-path-format=separate-events -fno-diagnostics-show-caret -fno-lto -c -o /dev/null -Wno-error

PROFILE ?=debug

IS_ERROR = 0
ERROR_TEXT = ok

ifeq ($(LRT_IS_REQUIRED),$(GLIBC_VERSION))
	LIBS += -lrt
endif

ifeq ($(PROFILE),debug)
	CFLAGS += $(PROFILE_DEBUG_CFLAGS) -DMESH_DEBUG
else
	ifeq ($(PROFILE),release)
		CFLAGS += $(PROFILE_RELEASE_CFLAGS)
	else ifeq ($(PROFILE),size)
		CFLAGS += $(PROFILE_SIZE_CFLAGS)
	else ifeq ($(PROFILE),check)
		CFLAGS += $(PROFILE_CHECK_CFLAGS)
	else
		IS_ERROR =1
		ERROR_TEXT = [ERROR] Unknown profile "$(PROFILE)".
	endif
endif

ifneq ($(MOLD_EXISTS),)
	CFLAGS += -fuse-ld=mold
endif
