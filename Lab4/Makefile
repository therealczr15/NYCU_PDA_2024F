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
TARGET = D2DGRter

all: $(TARGET) $(OUTDIR) $(LAYDIR) $(DRADIR)

$(OBJDIR):
	@mkdir $(OBJDIR)
 
$(OUTDIR):
	@mkdir $(OUTDIR)

$(LAYDIR):
	@mkdir $(LAYDIR)
 
$(DRADIR):
	@mkdir $(DRADIR)

D2DGRter: main.cpp $(OBJS)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $^ -o $@

exe:
	./$(TARGET) publicCase/testcase0/testcase0.gmp publicCase/testcase0/testcase0.gcl publicCase/testcase0/testcase0.cst publicCase/testcase0/testcase0.lg output/testcase0.lg layout/testcase0.txt
	./$(TARGET) publicCase/testcase1/testcase1.gmp publicCase/testcase1/testcase1.gcl publicCase/testcase1/testcase1.cst publicCase/testcase1/testcase1.lg output/testcase1.lg layout/testcase1.txt
	./$(TARGET) publicCase/testcase2/testcase2.gmp publicCase/testcase2/testcase2.gcl publicCase/testcase2/testcase2.cst publicCase/testcase2/testcase2.lg output/testcase2.lg layout/testcase2.txt


check:
	./Evaluator publicCase/testcase0 testcase0
	./Evaluator publicCase/testcase1 testcase1
	./Evaluator publicCase/testcase2 testcase2

clean:
	rm -rf $(OBJDIR) $(TARGET) $(OUTDIR) $(DRADIR)

# Compilation rule for object files with automatic dependency generation
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) Makefile
	$(CXX) $(WARNINGS) $(CXXFLAGS) -c $< -o $@