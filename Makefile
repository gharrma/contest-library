TOPICS = algorithms data-structures graphs math strings
SRC = $(shell find $(TOPICS) template.cpp -name "*.cpp")
BIN = $(SRC:%.cpp=%.binary)
TEST = $(SRC:%.cpp=%.test)
TIME = $(SRC:%.cpp=%.time)

compile: $(BIN)

%.binary: %.cpp
	@echo "Compiling $*"
	@mkdir -p $(dir $@)
	@g++ -std=c++11 -Wall -O2 -o $@ $<

test: $(TEST)

%.test: %.binary
	@echo "Testing $*"
	@./$< | tee $@
	@echo

time: $(TIME)

%.time: %.binary
	@echo "Timing $*"
	@time ./$< | tee $@
	@echo

clean:
	@rm -rf $(BIN) $(TEST) $(TIME)
	@echo "Removed generated files"

.PHONY: compile test time clean
