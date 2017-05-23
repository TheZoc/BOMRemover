CXX=g++
CXXFLAGS=-c -Wall -std=c++14
LDFLAGS=

SOURCEDIR=BOMRemover
SOURCES=$(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
OUTPUT=out/BOMRemover
# A directory creation utility
create_output_dir=@mkdir -p $(@D)

.PHONY: all clean BOMRemoverObjects

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(create_output_dir)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJECTS): %.o: %.cpp
	$(create_output_dir)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf out/
	rm -rf BOMRemover/*.o
