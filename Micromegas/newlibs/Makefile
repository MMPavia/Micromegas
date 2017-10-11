CFLAGS := -g -fPIC -std=c++0x
CFLAGS := -g -fPIC -std=gnu++11
LFLAGS := -pthread
SRC := .
OBJ := ../obj/iface
LIB := ../lib
BIN := ../bin
INC := ../include

MYHEADERS := myrs232.h rs232ard.h mutex_lock.h arduinoX.h gauge.h motors.h optline.h optline3D.h keithley.h keithley2.h picoam.h particlecounter.h rs232laser.h chauvinarnoux.h
INST_HEADERS := $(patsubst %.h, $(INC)/%.h, $(MYHEADERS))
LIB_OBJECTS := $(patsubst %.h, $(OBJ)/%.o, $(MYHEADERS))

all : $(INST_HEADERS) $(LIB)/libRS232.so
	@chmod go-w *

$(INC)/%.h : %.h
	@mkdir -p $(INC)
	@cp -p $^ $(INC)/

clean :
	rm -f $(INST_HEADERS) $(LIB_OBJECTS) $(LIB)/libRS232.so

$(LIB)/libRS232.so : $(LIB_OBJECTS)
	g++ $(CFLAGS) -shared -o $@ $^ -lc


$(BIN)/% : $(OBJ)/%.o
	g++ $(CFLAGS) $(LFLAGS) -o $@ $^ -L$(LIB) -lRS232

$(OBJ)/%.o : %.cxx
	@mkdir -p $(OBJ) $(LIB) $(BIN)
	g++ $(CFLAGS) -I$(INC) -c -o $@ $^

% : %.cxx
	g++ $(CFLAGS) $(LFLAGS) -o $@ $^ -L$(LIB) -lRS232

