# ref: https://stackoverflow.com/a/30602701
SRC_DIR := src
GRAMMAR_DIR := grammar
OBJ_DIR := obj
BIN_DIR := bin
INCLUDE_DIR := src/include

EXE := $(BIN_DIR)/main
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
bison:= /usr/local/Cellar/bison/3.8.2/bin/bison
CXX:= clang++ 
CXXFLAGS := -isysroot `xcrun --show-sdk-path` -Wno-register -DPROD 
CPPFLAGS := -I$(INCLUDE_DIR) -MMD -MP
endif

.PHONY: all clean check

all: $(EXE)


$(EXE): $(OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/Scanner1.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/Parser1.cpp $(SRC_DIR)/Parser1.hpp: $(GRAMMAR_DIR)/grammar1.y
	$(bison) $^
	mv Parser1.cpp $(SRC_DIR)
	mv Parser1.hpp $(INCLUDE_DIR)/

$(SRC_DIR)/Scanner1.cpp: $(GRAMMAR_DIR)/lexer1.l $(SRC_DIR)/Parser1.hpp 
	flex $<
	mv Scanner1.cpp $(SRC_DIR)/
	mv Scanner1.hpp $(INCLUDE_DIR)/

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

TEST_CMD = ./bin/main

check: tests/test.txt $(BIN_DIR) 
	@echo "Running tests..."
	@while read -r line; do \
	    test_case=$$line; \
	    args=$$(echo "$$test_case" | cut -d' ' -f1,2); \
	    arg1=$$(echo "$$test_case" | cut -d' ' -f1); \
	    arg2=$$(echo "$$test_case" | cut -d' ' -f2); \
	    arg3=$$(echo "$$test_case" | cut -d' ' -f3); \
	    expected_result="$$arg1($$arg2) RESULT: $$arg3"; \
	    actual_result=$$($(TEST_CMD) $$args); \
	    if [ "$$actual_result" = "$$expected_result" ]; then \
	        echo "Test $$args passed"; \
	    else \
	        echo "Test $$args failed"; \
	    fi; \
	done < $<

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)

