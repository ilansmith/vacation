CC=gcc
CPP=cpp
CFLAGS=-ansi -Werror -Wall -pedantic-errors
APP=vacation_days

OBJS=vacation_days.o

ifeq ($(DEBUG),y)
  CFLAGS+=-O0 -g
else
  CFLAGS+=-O3
endif
%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

%_pre.c: %.c
	$(CPP) -x c -o $@ $<

.PHONY: all clean cleanall

all: $(APP)

$(APP): $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	@echo "removing object files"
	@rm -f *.o
	@echo "removing pre-processed files"
	@rm -f *_pre.c

cleanall: clean
	@echo "removing tag file"
	@rm -f tags
	@echo "removing $(APP)"
	@rm -f $(APP)

