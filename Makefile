CC=gcc
FLAGS = -Wall -Wextra -Werror

SRC_DIR=src
INCL_DIR=include
O_DIR=object

EXE=hw1

C_FILES=$(wildcard $(SRC_DIR)/*)
O_FILES=$(C_FILES:$(SRC_DIR)/%.c=$(O_DIR)/%.o)
O_FILES+=$(O_DIR)/$(EXE).o


all: $(EXE)

$(O_DIR)/$(EXE).o: $(EXE).c
	gcc -c $(FLAGS) -I$(INCL_DIR) -o $@ $<

$(O_DIR)/%.o: $(SRC_DIR)/%.c $(INCL_DIR)/%.h
	@mkdir -p $(@D)	
	gcc -c $(FLAGS) -I$(INCL_DIR) -o $@ $<


$(EXE): $(O_FILES)
	gcc -o $(EXE) $^


clean:
	rm -rf $(O_DIR)
	rm $(EXE)