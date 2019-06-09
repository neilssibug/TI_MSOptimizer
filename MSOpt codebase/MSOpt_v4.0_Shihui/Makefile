# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

# $Id$

##########################################################################
#    You can modify this example makefile to fit for your own program.   #
#    Usually, you only need to change the five CHANGEME entries below.   #
##########################################################################
#Working Directory
CURDIR = C:/Users/x0226753/eclipse_workspace/MSOpt_v4.0_Shihui
BCPdir=$(CURDIR)/BCP32
PROJDIR = $(CURDIR)
BIN = $(CURDIR)
SQLITEDIR = $(CURDIR)/SQLITE
#CHANGEME: If you want to compile under Cygwin, uncomment the following
# line.
#CYGFLAG = -mno-cygwin

# CHANGEME: add extra definitions here
USER_DEFINES = -DCOIN_USE_CLP
#USER_DEFINES = -DCOIN_USE_CPX
# Possibilities: 
#                -DHEUR_SOL :      generate heuristic solutions
#                -DCUSTOM_BRANCH : branch on first non integer variable
#                -DUSER_DATA :     define data associated with each node
#

# CHANGEME: This should be the name of your executable
EXE = at_Shihui

# Add access to Cplex if available (see bac.pdf for installation)
# actual solver library the code should be linked against. In the third
# variable any additional libraries should be listed along with any other
# necessary flags, like -L or -rpath, etc. Note that no -L flag needs to be
# added for libraries from COIN.
USER_OSI_LIBS = -lOsiClp $(LIB_OsiCpx)
USER_SOLVER_LIBS = -lClp $(LIB_CPLEX)


#add extra libs
USER_EXTRA_INCL = $(SQLITEDIR)/include
USER_EXTRA_LIBS = -L$(SQLITEDIR)/lib 

# CHANGEME: Additional include directories for compilation. Note that no
# include directory needs to be listed here for headers from COIN.
USER_INCDIRS = $(PROJDIR)/include

##########################################################################
#  Usually, you don't have to change anything below.  Note that if you   #
#  change certain compiler options, you might have to recompile the      #
#  Bcp package.                                                          #
##########################################################################

# C++ Compiler command
CXX = g++

# C++ Compiler options

#CXXFLAGS = $(CYGFLAG) -O3 -fomit-frame-pointer -pipe -DNDEBUG -pedantic-errors -Wimplicit -Wparentheses -Wreturn-type -Wcast-qual -Wall -Wpointer-arith -Wwrite-strings -Wconversion   $(USER_DEFINEs) -lefence
CXXFLAGS = $(CYGFLAG) -O3 -fomit-frame-pointer -pipe -DNDEBUG -pedantic-errors -Wimplicit -Wparentheses -Wreturn-type -Wcast-qual -Wall -Wpointer-arith -Wwrite-strings -Wconversion   $(USER_DEFINES)


# additional C++ Compiler options for linking
#CXXLINKFLAGS =  -Wl,--rpath -Wl,$(BCPdir)/lib,$(BCPdir)/lib/libefence.a

# Directory with COIN header files
COININCDIR = $(BCPdir)/include/coin

# Directory with COIN libraries
COINLIBDIR = $(BCPdir)/lib

# CHANGEME: Here is the name of all object files corresponding to the source
#           code that you wrote in order to define the problem statement
OBJS = \
	$(BIN)/LP/AT_lp.o \
	$(BIN)/LP/AT_lp_branch.o \
	$(BIN)/LP/AT_lp_param.o \
	$(BIN)/Member/AT_GRASP.o\
	$(BIN)/Member/AT_init.o \
	$(BIN)/TM/AT_tm.o \
	$(BIN)/TM/AT_tm_param.o \

# Libraries necessary to link with Clp
LIBS = -L$(COINLIBDIR) \
	-lBcp \
	-lCgl \
	$(USER_OSI_LIBS) -lOsi $(USER_SOLVER_LIBS) \
	-lCoinUtils \
	`cat $ $(BCPdir)/Cgl/cgl_addlibs.txt` \
	`cat $ $(BCPdir)/Osi/osi_addlibs.txt` \
	`cat $ $(BCPdir)/Clp/clp_addlibs.txt` \
	`cat $ $(BCPdir)/CoinUtils/coinutils_addlibs.txt` \
	$(USER_EXTRA_LIBS) \
	-lsqlite3



# The following is necessary under cygwin, if native compilers are used
CYGPATH_W = echo

# Necessary Include dirs (we use the CYGPATH_W variables to allow
# compilation with Windows compilers)
INCL = `for i in $(USER_INCDIRS) $(COININCDIR) $(SQLITEDIR); do $(CYGPATH_W) $$i | sed -e 's/^/-I/'; done`

#CPX_PATH="-I/usr/local/ilog/cplex110/include -I/usr/local/ilog/concert25/include"
#CPX_FLAG="-m64 -c -O -fPIC -fexceptions -DNDEBUG -DIL_STD"
all: $(EXE)

.SUFFIXES: .cpp .c .o .obj

$(EXE): $(OBJS)
	bla=;\
	for file in $(OBJS); do bla="$$bla `$(CYGPATH_W) $$file`"; done; \
	$(CXX)  $(CXXLINKFLAGS) $(CXXFLAGS) `echo $(CPX_INCDIR)` -o $@ $$bla $(ADDLIBS) $(LIBS) `echo $(CPX_CONCERT)`

clean:
	rm -rf $(EXE) $(OBJS)

./Member/AT_GRASP.o: $(PROJDIR)/Member/AT_GRASP.cpp
	g++   -g3 -c -v -O -fexceptions -DNDEBUG -DIL_STD $(USER_DEFINES)  -Iinclude $(INCL) $(PROJDIR)/Member/AT_GRASP.cpp -o $(BIN)/Member/AT_GRASP.o

./TM/AT_tm.o: $(PROJDIR)/TM/AT_tm.cpp
	g++   -g3 -c -v -O  -fexceptions -DNDEBUG -DIL_STD $(USER_DEFINES)  -Iinclude $(INCL) $(PROJDIR)/TM/AT_tm.cpp -o $(BIN)/TM/AT_tm.o
	
./TM/AT_tm_param.o: $(PROJDIR)/TM/AT_tm_param.cpp
	g++   -g3 -c -v -O  -fexceptions -DNDEBUG -DIL_STD $(USER_DEFINES)  -Iinclude $(INCL) $(PROJDIR)/TM/AT_tm_param.cpp -o $(BIN)/TM/AT_tm_param.o

./Member/AT_init.o: $(PROJDIR)/Member/AT_init.cpp
	g++   -g3 -c -v -O  -fexceptions -DNDEBUG -DIL_STD $(USER_DEFINES) -Iinclude $(INCL) $(PROJDIR)/Member/AT_init.cpp -o $(BIN)/Member/AT_init.o

./LP/AT_lp.o: $(PROJDIR)/LP/AT_lp.cpp
	g++   -g3 -c -v -O  -fexceptions -DNDEBUG -DIL_STD $(USER_DEFINES) -Iinclude $(INCL) $(PROJDIR)/LP/AT_lp.cpp -o $(BIN)/LP/AT_lp.o

./LP/AT_lp_param.o: $(PROJDIR)/LP/AT_lp_param.cpp
	g++   -g3 -c -v -O  -fexceptions -DNDEBUG -DIL_STD $(USER_DEFINES) -Iinclude $(INCL) $(PROJDIR)/LP/AT_lp_param.cpp -o $(BIN)/LP/AT_lp_param.o

./LP/AT_lp_branch.o: $(PROJDIR)/LP/AT_lp_branch.cpp
	g++   -g3 -c -v -O  -fexceptions -DNDEBUG -DIL_STD $(USER_DEFINES) -Iinclude $(INCL) $(PROJDIR)/LP/AT_lp_branch.cpp -o $(BIN)/LP/AT_lp_branch.o

.cpp.o:
	$(CXX)  $(INCL)  -g3 -c -v -o $@ `test -f '$<' || echo '$(SRCDIR)/'`$<


.cpp.obj:
	$(CXX) $(CXXFLAGS) $(INCL)  -g3 -c -v -o $@ `if test -f '$<'; then $(CYGPATH_W) '$<'; else $(CYGPATH_W) '$(SRCDIR)/$<'; fi`

