
XFLAGS := -g -fPIC -std=c++0x
CFLAGS := -g -fPIC -std=gnu++11
LFLAGS := -pthread
SRC := .
OBJ := ../obj/iface
LIB := ../lib
BIN := ../bin
INC := ../include

MYHEADERS := bpipe.h arduinoZ.h
INST_HEADERS := $(patsubst %.h, $(INC)/%.h, $(MYHEADERS))

SOURCES := $(SRC)/arduinoserver.cxx $(SRC)/test_arduinoZ.cxx
LIBSOURCES := $(SRC)/arduinoZ.cxx $(SRC)/bpipe.cxx

MYBINS := $(patsubst $(SRC)/%.cxx, $(BIN)/%, $(SOURCES))
MYLIB := $(LIB)/libardZ.so

MYOBJS := $(patsubst $(SRC)/%.cxx, $(OBJ)/%.o, $(SOURCES))
MYLIBOBJS := $(patsubst $(SRC)/%.cxx, $(OBJ)/%.o, $(LIBSOURCES))

all : $(INST_HEADERS) $(MYLIB) $(MYBINS)
	@chmod go-w *

$(INC)/%.h : %.h
	@mkdir -p $(INC)
	@cp -p $^ $(INC)/

clean :
	rm -f $(INST_HEADERS) $(MYOBJS) $(MYLIBOBJS) $(MYBINS) $(MYLIB)

$(BIN)/test_arduinoZ : $(OBJ)/test_arduinoZ.o $(OBJ)/debug.o
	g++ $(CFLAGS) $(LFLAGS) -o $@ $^ -L$(LIB) -lRS232 -lardZ

$(BIN)/arduinoserver : $(OBJ)/arduinoserver.o $(OBJ)/debug.o
	g++ $(CFLAGS) $(LFLAGS) -o $@ $^ -L$(LIB) -lRS232 -lardZ

$(OBJ)/%.o : $(SRC)/%.cxx
	@mkdir -p $(OBJ) $(LIB) $(BIN)
	g++ $(CFLAGS) -I$(INC) -c -o $@ $^

$(MYLIB) : $(MYLIBOBJS)
	g++ $(CFLAGS) -shared -o $@ $^ -lc

% : %.cxx
	g++ -std=c++0x -o $@ $^
