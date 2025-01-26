.PHONY: main clean

PROFILE ?= debug
THREADS ?=
CMAKE_FLAGS ?=

main:
	@-mkdir build
	cmake . -Bbuild -DPROFILE=$(PROFILE) $(CMAKE_FLAGS)
	make -C build -j$(THREADS)

clean:
	rm -fdr build
