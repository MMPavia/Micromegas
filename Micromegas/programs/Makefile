CFLAGS := -g -fPIC -std=c++0x
CFLAGS := -g -fPIC -std=gnu++11
LFLAGS := -pthread
SRC := .
OBJ := ../obj/prog
OBJCore := ../obj/iface
LIB := ../lib
BIN := ../bin
INC := ../include

XXBINARIES := $(LIB)/libpipe.so $(BIN)/ambient $(BIN)/pressmon $(BIN)/map_XY $(BIN)/map_rettangolo $(BIN)/map_trap  $(BIN)/getlist  $(BIN)/map_diag $(BIN)/trap_XYS $(BIN)/move_one $(BIN)/go2zero $(BIN)/zero_motor $(BIN)/remove_mutex $(BIN)/try $(BIN)/test_keithley $(BIN)/test_keithley2 $(BIN)/test_picoam $(BIN)/test_optical $(BIN)/test_motors $(BIN)/test_gauge $(BIN)/test_particlecounter $(BIN)/test_arduino $(BIN)/test_arduinoX $(BIN)/glue_jig

BINARIES := $(BIN)/ambient $(BIN)/map_XY $(BIN)/getlist $(BIN)/map_diag $(BIN)/map_rettangolo $(BIN)/map_trap $(BIN)/pressmon $(BIN)/trap_XYS $(BIN)/glue_jig  $(BIN)/test_mtx $(BIN)/test_arduinoX $(BIN)/test_keithley $(BIN)/test_keithley2 $(BIN)/test_picoam $(BIN)/test_optical $(BIN)/test_trigger $(BIN)/read_laser $(BIN)/go2zero $(BIN)/move_one $(BIN)/test_gauge $(BIN)/test_motors $(BIN)/test_particlecounter $(BIN)/zero_motor  $(BIN)/map_laser 

OBJECTS := $(patsubst $(BIN)/%, $(OBJ)/%.o, $(BINARIES))

all : $(BINARIES)
	@chmod go-w *

$(INC)/%.h : %.h
	@mkdir -p $(INC)
	@cp -p $^ $(INC)/

clean :
	rm -f $(BINARIES) $(OBJECTS)

$(BIN)/% : $(OBJ)/%.o
	g++ $(CFLAGS) $(LFLAGS) -o $@ $^ -L$(LIB) -lRS232 -lpipe 

$(OBJ)/%.o : %.cxx
	@mkdir -p $(OBJ) $(LIB) $(BIN)
	g++ $(CFLAGS) -I$(INC) -c -o $@ $^

