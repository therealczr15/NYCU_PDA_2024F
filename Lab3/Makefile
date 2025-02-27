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
DRADIR = drawing
LAYDIR = layout
GIFDIR = gif
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Name of the executable
TARGET = Legalizer

all: $(TARGET) $(OUTDIR) $(DRADIR) $(GIFDIR) $(LAYDIR)

$(OBJDIR):
	@mkdir $(OBJDIR)

$(OUTDIR):
	@mkdir $(OUTDIR)
 
$(DRADIR):
	@mkdir $(DRADIR)

$(GIFDIR):
	@mkdir $(GIFDIR)

$(LAYDIR):
	@mkdir $(LAYDIR)

Legalizer: main.cpp $(OBJS)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $^ -o $@

exe:
	./Legalizer testcase/testcase1_16900.lg testcase/testcase1_16900.opt output/testcase1_16900_post.lg
	./Legalizer testcase/testcase1_ALL0_5000.lg testcase/testcase1_ALL0_5000.opt output/testcase1_ALL0_5000_post.lg
	./Legalizer testcase/testcase2_100.lg testcase/testcase2_100.opt output/testcase2_100_post.lg
	./Legalizer testcase/testcase1_MBFF_LIB_7000.lg testcase/testcase1_MBFF_LIB_7000.opt output/testcase1_MBFF_LIB_7000_post.lg
	./Legalizer testcase/testcase3_4579.lg testcase/testcase3_4579.opt output/testcase3_4579_post.lg

check:
	./Evaluator testcase/testcase1_16900.lg testcase/testcase1_16900.opt output/testcase1_16900_post.lg
	./Evaluator testcase/testcase1_ALL0_5000.lg testcase/testcase1_ALL0_5000.opt output/testcase1_ALL0_5000_post.lg
	./Evaluator testcase/testcase2_100.lg testcase/testcase2_100.opt output/testcase2_100_post.lg
	./Evaluator testcase/testcase1_MBFF_LIB_7000.lg testcase/testcase1_MBFF_LIB_7000.opt output/testcase1_MBFF_LIB_7000_post.lg
	./Evaluator testcase/testcase3_4579.lg testcase/testcase3_4579.opt output/testcase3_4579_post.lg

clean:
	rm -rf $(OBJDIR) $(TARGET) $(OUTDIR) $(DRADIR) $(GIFDIR)

# Compilation rule for object files with automatic dependency generation
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) Makefile
	$(CXX) $(WARNINGS) $(CXXFLAGS) -c $< -o $@