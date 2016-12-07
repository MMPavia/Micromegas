CFLAGS := -g -fPIC -std=c++0x
CFLAGS := -g -fPIC -std=gnu++11
LFLAGS := -pthread
SRC := .
OBJ := ../obj/prog
OBJCore := ../obj/iface
LIB := ../lib
BIN := ../bin
INC := ../include

all : $(BIN)/ambient $(BIN)/map_XY $(BIN)/getlist $(BIN)/map_diag $(BIN)/map_rettangolo $(BIN)/map_trap $(BIN)/pressmon $(BIN)/trap_XYS $(BIN)/test_arduinoX $(BIN)/glue_jig
	@chmod go-w *

XXall : $(LIB)/libpipe.so $(BIN)/ambient $(BIN)/pressmon $(BIN)/map_XY $(BIN)/map_rettangolo $(BIN)/map_trap  $(BIN)/getlist  $(BIN)/map_diag $(BIN)/trap_XYS $(BIN)/move_one $(BIN)/go2zero $(BIN)/zero_motor $(BIN)/remove_mutex $(BIN)/try $(BIN)/test_keithley $(BIN)/test_keithley2 $(BIN)/test_picoam $(BIN)/test_optical $(BIN)/test_motors $(BIN)/test_gauge $(BIN)/test_particlecounter $(BIN)/test_arduino $(BIN)/test_arduinoX $(BIN)/glue_jig
	@chmod go-w *

$(INC)/%.h : %.h
	@mkdir -p $(INC)
	@cp -p $^ $(INC)/

clean :
	rm -f  $(OBJ)/*.o $(BIN)/ambient_ard $(BIN)/map_XY_ard $(BIN)/getlist_ard $(BIN)/map_diag_ard $(BIN)/map_rettangolo_ard $(BIN)/map_trap_ard $(BIN)/pressmon_ard $(BIN)/trap_XYS_ard $(BIN)/test_arduinoX $(BIN)/glue_jig

XXclean :
	rm -f  $(OBJ)/*.o $(BIN)/remove_mutex $(BIN)/ambient $(BIN)/pressmon $(BIN)/map_XY $(BIN)/map_rettangolo $(BIN)/map_trap $(BIN)/getlist  $(BIN)/map_diag  $(BIN)/trap_XYS $(BIN)/go2zero $(BIN)/move_one $(BIN)/zero_motor $(BIN)/remove_mutex $(BIN)/try $(BIN)/test_keithley $(BIN)/test_keithley2 $(BIN)/test_picoam $(BIN)/test_optical $(BIN)/test_motors $(BIN)/test_gauge $(BIN)/test_particlecounter $(BIN)/test_arduino $(BIN)/test_arduino2 $(BIN)/test_arduinoX $(BIN)/glue_jig

$(BIN)/remove_mutex : $(OBJ)/remove_mutex.o
	g++ $(CFLAGS) $(LFLAGS) -o $@ $^

$(BIN)/% : $(OBJ)/%.o
	g++ $(CFLAGS) $(LFLAGS) -o $@ $^ -L$(LIB) -lRS232 -lpipe 

$(OBJ)/%.o : %.cxx
	@mkdir -p $(OBJ) $(LIB) $(BIN)
	g++ $(CFLAGS) -I$(INC) -c -o $@ $^

% : %.cxx
	g++ -std=c++0x -o $@ $^
