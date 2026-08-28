# Makefile — compilation de l'application console sans Code::Blocks.
# Usage :
#   make            # build bin/election (debug, -g)
#   make release    # build optimise -O2
#   make run        # compile puis lance depuis src/ (les .dat sont ici)
#   make clean

CC      ?= gcc
CFLAGS  ?= -Wall -Wextra -g
TARGET   = bin/election

SRC = buffer.c velectro.c \
      ge_BV.c ge_candi.c ge_elect.c ge_parti.c ge_votes.c rapp.c \
      core/core_util.c core/core_verif.c core/core_parti.c core/core_bv.c \
      core/core_elect.c core/core_candid.c core/core_vote.c core/core_rapp.c

HEADERS = election.h election_core.h core/core_interne.h

$(TARGET): $(SRC) $(HEADERS)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $(SRC)

release: CFLAGS = -Wall -Wextra -O2
release: clean $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: release run clean
