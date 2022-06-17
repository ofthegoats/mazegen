# ofthegoats C/C++ Makefile: 14/05/22

# PROJECT CONFIG
NAME=$(shell basename $(CURDIR))

CC=clang
CFLAGS=-std=c17 -Wall -Wextra
LINKER=$(CC)
LDFLAGS=

SRCDIR=.
OBJDIR=obj
BINDIR=bin
DEPDIR=.d

OUTPUT=$(BINDIR)/$(NAME)
# END PROJECT CONFIG

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC_C = $(call rwildcard,$(SRCDIR),*.c)
OBJ_C = $(SRC_C:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OBJ= $(OBJ_C)

DEBUG?=1 # by default, debug
ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g
else
	CFLAGS += -O3 -DNDEBUG
endif

.PHONY: all clean run

all: $(DEPDIR) $(OBJDIR) $(BINDIR) $(OUTPUT) 

clean:
	rm -rf $(BINDIR) $(OBJDIR) $(DEPDIR)

run:
	./$(OUTPUT)

$(BINDIR):
	mkdir -p $(BINDIR)
$(OBJDIR):
	mkdir -p $(OBJDIR)
$(DEPDIR):
	mkdir -p $(DEPDIR)

$(OUTPUT): $(OBJ)
	$(LINKER) -o $@ $^ $(LDFLAGS)

# dynamically generated dependency "graph"
-include $(OBJ:.o=.d)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ $< $(CFLAGS) -c -MMD
	@mv -f $(OBJDIR)/$*.d $(OBJDIR)/$*.d.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $(OBJDIR)/$*.d.tmp > $(OBJDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJDIR)/$*.d
	@rm -f $(OBJDIR)/$*.d.tmp
	@sed -i '/\\\:/d' $(OBJDIR)/$*.d
