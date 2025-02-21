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
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Name of the executable
TARGET = Lab1

all: $(TARGET) $(OUTDIR) $(LAYDIR)

$(OBJDIR):
	@mkdir $(OBJDIR)

$(OUTDIR):
	@mkdir $(OUTDIR)

$(LAYDIR):
	@mkdir $(LAYDIR)

Lab1: main.cpp $(OBJS)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $^ -o $@

exe:
	./$(TARGET) testcase/case0.txt output/output0.txt layout/layout0.txt
	./$(TARGET) testcase/case1.txt output/output1.txt layout/layout1.txt
	./$(TARGET) testcase/case2.txt output/output2.txt layout/layout2.txt
	./$(TARGET) testcase/case3.txt output/output3.txt layout/layout3.txt
	./$(TARGET) testcase/case4.txt output/output4.txt layout/layout4.txt
	./$(TARGET) testcase/case5.txt output/output5.txt layout/layout5.txt
	./$(TARGET) testcase/case6.txt output/output6.txt layout/layout6.txt
	./$(TARGET) testcase/case7.txt output/output7.txt layout/layout7.txt

clean:
	rm -rf $(OBJDIR) $(TARGET) $(OUTDIR) $(LAYDIR)

# Compilation rule for object files with automatic dependency generation
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) Makefile
	$(CXX) $(WARNINGS) $(CXXFLAGS) -c $< -o $@
