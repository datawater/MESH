include config.mk

MESH_SOURCE_FILES = $(wildcard src/*/*.cpp) $(wildcard src/*.cpp)
MESH_OBJECT_FILES = $(patsubst %.cpp,%.o,$(MESH_SOURCE_FILES))

.PHONY: main
main: check_error mesh

mesh: $(MESH_OBJECT_FILES)
	$(CXX) $(CFLAGS) $^ -o $@ $(LIBS)

libmesh.a: $(MESH_OBJECT_FILES)
	ar -crs $@ $^
	
src/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

# workshy:
# 	make -C ./src/workshy

.PHONY: check_error
check_error:
ifeq ($(IS_ERROR),1)
	$(info $(ERROR_TEXT))
else
endif

.PHONY: clean
clean:
	-find . -type f -name "*.o" -delete
	-rm mesh
	-rm *.a

	# -make -C ./src/workshy clean
	-make -C ./src/tests clean