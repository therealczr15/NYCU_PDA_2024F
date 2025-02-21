.PHONY: all clean

# Compiler settings
CXX = g++
CXXFLAGS = -I ./inc -std=c++11
WARNINGS = -g -Wall -O3

# Source files and object files
SRCDIR = src
OBJDIR = obj
INCDIR = inc
OUTDIR = output
LAYDIR = layout
DRADIR = drawing
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Name of the executable
TARGET = Lab2

all: $(TARGET) $(OUTDIR) $(LAYDIR) $(DRADIR)

$(OBJDIR):
	@mkdir $(OBJDIR)
 
$(OUTDIR):
	@mkdir $(OUTDIR)

$(LAYDIR):
	@mkdir $(LAYDIR)
 
$(DRADIR):
	@mkdir $(DRADIR)

Lab2: main.cpp $(OBJS)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $^ -o $@

exe:
	./$(TARGET) 0.5 testcase/ami33/ami33.block testcase/ami33/ami33.nets output/ami33.txt layout/ami33.txt
	./$(TARGET) 0.5 testcase/ami49/ami49.block testcase/ami49/ami49.nets output/ami49.txt layout/ami49.txt
	./$(TARGET) 0.5 testcase/vda317b/vda317b.block testcase/vda317b/vda317b.nets output/vda317b.txt layout/vda317b.txt

check:
	./verifier 0.5 testcase/ami33/ami33.block testcase/ami33/ami33.nets output/ami33.txt
	./verifier 0.5 testcase/ami49/ami49.block testcase/ami49/ami49.nets output/ami49.txt
	./verifier 0.5 testcase/vda317b/vda317b.block testcase/vda317b/vda317b.nets output/vda317b.txt

clean:
	rm -rf $(OBJDIR) $(TARGET) $(OUTDIR) $(DRADIR)

# Compilation rule for object files with automatic dependency generation
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) Makefile
	$(CXX) $(WARNINGS) $(CXXFLAGS) -c $< -o $@