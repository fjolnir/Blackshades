CC := gcc
CXX := g++
LINKER := g++
ASM := nasm
SRCDIR := ./Source
BINDIR := ./objs
EXE := $(BINDIR)/blackshades

CFLAGS := -O2 -Wall -g $(shell sdl-config --cflags) -I$(SRCDIR) -I/usr/local/include -include Source/Support.h
CXXFLAGS := $(CFLAGS)
LDFLAGS := $(shell sdl-config --libs) -lalut -framework OpenAL -framework OpenGL -lsdl_image

# Don't want ogg?
#CFLAGS += -DNOOGG
# Got ogg?
LDFLAGS += -lvorbisfile -lvorbis

SRCS := Camera.cpp \
        Decals.cpp \
        Fog.cpp \
        Frustum.cpp \
        GameDraw.cpp \
	GameInitDispose.cpp \
	GameLoop.cpp \
	GameTick.cpp \
        Globals.cpp \
	MacInput.cpp \
	Main.cpp \
        Maths.cpp \
	Models.cpp \
	Person.cpp \
        Quaternions.cpp \
	Serialize.cpp \
	Skeleton.cpp \
        Sprites.cpp \
	Support.cpp \
        TGALoader.cpp \
        Text.cpp \
	Timer.cpp \
	Textures.cpp

OBJS1 := $(SRCS:.c=.o)
OBJS2 := $(OBJS1:.cpp=.o)
OBJS3 := $(OBJS2:.asm=.o)
OBJS := $(foreach f,$(OBJS3),$(BINDIR)/$(f))
SRCS := $(foreach f,$(SRCS),$(SRCDIR)/$(f))

CLEANUP = $(wildcard *.exe) $(wildcard *.obj) \
          $(wildcard $(BINDIR)/*.exe) $(wildcard $(BINDIR)/*.obj) \
          $(wildcard *~) $(wildcard *.err) \
          $(wildcard .\#*) core $(EXE)


# Rules for compiling individual source files...

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

.PHONY: all bindir blackshades
all: $(EXE)

$(EXE): $(BINDIR) $(OBJS)
	$(LINKER) -o $(EXE) $(OBJS) $(LDFLAGS)

$(BINDIR) :
	$(MAKE) bindir

bindir:
	mkdir -p $(BINDIR)

distclean: clean

clean:
	rm -f $(CLEANUP)
	rm -rf $(BINDIR)

# end of Makefile.


