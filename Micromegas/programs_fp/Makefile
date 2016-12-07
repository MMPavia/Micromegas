CFLAGS := -g -fPIC -std=c++0x
CFLAGS := -g -fPIC -std=gnu++11
LFLAGS := -pthread
SRC := .
OBJ := ../obj/prog
OBJCore := ../obj/iface
LIB := ../lib
BIN := ../bin
INC := ../include

all : $(LIB)/libpipe.so $(BIN)/ambient $(BIN)/pressmon $(BIN)/map_XY $(BIN)/map_rettangolo $(BIN)/map_trap  $(BIN)/getlist  $(BIN)/map_diag $(BIN)/trap_XYS $(BIN)/move_one $(BIN)/go2zero $(BIN)/zero_motor $(BIN)/remove_mutex $(BIN)/try $(BIN)/test_keithley $(BIN)/test_keithley2 $(BIN)/test_picoam $(BIN)/test_optical $(BIN)/test_motors $(BIN)/test_gauge $(BIN)/test_particlecounter $(BIN)/test_arduino $(BIN)/test_arduino2
	@chmod go-w *

$(INC)/%.h : %.h
	@mkdir -p $(INC)
	@cp -p $^ $(INC)/

clean :
	rm -f  $(OBJ)/*.o $(BIN)/remove_mutex $(BIN)/ambient $(BIN)/pressmon $(BIN)/map_XY $(BIN)/map_rettangolo $(BIN)/map_trap $(BIN)/getlist  $(BIN)/map_diag  $(BIN)/trap_XYS $(BIN)/go2zero $(BIN)/move_one $(BIN)/zero_motor $(BIN)/remove_mutex $(BIN)/try $(BIN)/test_keithley $(BIN)/test_keithley2 $(BIN)/test_picoam $(BIN)/test_optical $(BIN)/test_motors $(BIN)/test_gauge $(BIN)/test_particlecounter $(BIN)/test_arduino $(BIN)/test_arduino2

$(BIN)/remove_mutex : $(OBJ)/remove_mutex.o
	g++ $(CFLAGS) $(LFLAGS) -o $@ $^

$(BIN)/% : $(OBJ)/%.o
	g++ $(CFLAGS) $(LFLAGS) -o $@ $^ -L$(LIB) -lRS232 -lpipe 

$(OBJ)/%.o : %.cxx
	@mkdir -p $(OBJ) $(LIB) $(BIN)
	g++ $(CFLAGS) -I$(INC) -c -o $@ $^

% : %.cxx
	g++ -std=c++0x -o $@ $^

