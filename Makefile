CC      = gcc
CFLAGS  = -std=c11 -O2 -Wall -Wextra -fopenmp
LDFLAGS = -fopenmp
LDLIBS_MATH = -lm

BIN = bin
SRC = src

PROGS = hello_openmp data_sharing vector_add vector_add_alpha \
        vector_add_separate sum_methods sum_squares sum_timing \
        schedules phases phases_nowait tasks
PROGS_MATH = pi_benchmark matmul

all: $(PROGS:%=$(BIN)/%) $(PROGS_MATH:%=$(BIN)/%)

$(BIN)/%: $(SRC)/%.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(BIN)/pi_benchmark $(BIN)/matmul: $(BIN)/%: $(SRC)/%.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS_MATH)

$(BIN):
	mkdir -p $(BIN)

clean:
	rm -f $(BIN)/*

.PHONY: all clean
