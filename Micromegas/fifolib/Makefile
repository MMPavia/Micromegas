
CFLAGS := -g -fPIC -std=c++0x
CFLAGS := -g -fPIC -std=gnu++11
LFLAGS := -pthread
SRC := .
OBJ := ../obj/iface
LIB := ../lib
BIN := ../bin
INC := ../include

SOURCES := $(SRC)/pipe_cnt.cxx $(SRC)/pipe_srv.cxx
LIBSOURCES := $(SRC)/my_pipe.cxx  

MYBINS := $(patsubst $(SRC)/%.cxx, $(BIN)/%, $(SOURCES))
MYLIB := $(LIB)/libpipe.so
#libRS232.so

MYOBJS := $(patsubst $(SRC)/%.cxx, $(OBJ)/%.o, $(SOURCES))
MYLIBOBJS := $(patsubst $(SRC)/%.cxx, $(OBJ)/%.o, $(LIBSOURCES))

MYHEADERS := my_pipe.h
INST_HEADERS := $(patsubst %.h, $(INC)/%.h, $(MYHEADERS))

all : $(INST_HEADERS) $(MYLIB) $(MYBINS)
	@chmod go-w *

$(INC)/%.h : %.h
	@mkdir -p $(INC)
	@cp -p $^ $(INC)/

clean :
	rm -f $(INST_HEADERS) $(MYOBJS) $(MYLIBOBJS) $(MYBINS) $(MYLIB)

$(BIN)/pipe_cnt : $(OBJ)/pipe_cnt.o
	g++ $(CFLAGS) $(LFLAGS) -o $@ $^ -L$(LIB) -lpipe

$(BIN)/pipe_srv : $(OBJ)/pipe_srv.o
	g++ $(CFLAGS) $(LFLAGS) -o $@ $^ -L$(LIB) -lpipe

$(OBJ)/%.o : $(SRC)/%.cxx
	@mkdir -p $(OBJ) $(LIB) $(BIN)
	g++ $(CFLAGS) -c -o $@ $^

$(MYLIB) : $(MYLIBOBJS)
	g++ $(CFLAGS) -shared -o $@ $^ -lc

% : %.cxx
	g++ -std=c++0x -o $@ $^
