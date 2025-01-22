CFLAGS_WARNINGS = -Wall -Wextra -Werror -pedantic -Wstrict-aliasing \
				  -Wno-pointer-arith -Wno-variadic-macros -Wno-unused-command-line-argument \
				  -Wno-reorder -Wno-deprecated-copy -Wno-implicit-fallthrough
CFLAGS = $(CFLAGS_WARNINGS) -std=c++11 -fopenmp
LIBS =

GLIBC_VERSION := $(shell getconf GNU_LIBC_VERSION | tail -c +7)
LRT_REQUIRED_VERSION := 2.17
LRT_IS_REQUIRED := $(shell printf "%s\n" $(LRT_REQUIRED_VERSION) $(GLIBC_VERSION) | sort | head -n1)
MOLD_EXISTS := $(shell which mold)

OPT_LEVEL ?= 3
PROFILE_DEBUG_CFLAGS := -ggdb -O0
PROFILE_RELEASE_CFLAGS := -O$(OPT_LEVEL) -s -flto -mtune=native -march=native -fgraphite-identity
PROFILE_SIZE_CFLAGS := -Oz -s

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
	else
		ifeq ($(PROFILE),size)
			CFLAGS += $(PROFILE_SIZE_CFLAGS)
		else
			IS_ERROR =1
			ERROR_TEXT = [ERROR] Unknown profile "$(PROFILE)".
		endif
	endif
endif

ifneq ($(MOLD_EXISTS),)
	CFLAGS += -fuse-ld=mold
endif