CC = gcc
LDFLAGS = -lwiringPi -lpthread
BLDDIR = .
LIBDIR = $(BLDDIR)/lib
SRCDIR = $(BLDDIR)/src
OBJDIR = $(BLDDIR)/obj
CFLAGS = -c -w -I$(LIBDIR)
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
EXE = bin/bin

all: clean $(EXE) 
    
$(EXE): $(OBJ) 
	$(CC) $(OBJDIR)/*.o -o $@ $(LDFLAGS)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f $(OBJDIR)/*.o $(EXE)