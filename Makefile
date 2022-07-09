CC = g++
CXXFLAGS = -O2 -std=c++17

LDFLAGS = 

SRC_DIR = ./graphic
OBJ_DIR = ./object

TARGET = main

SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cpp))
OBJS = $(SRCS:.cpp=.o)

OBJECTS = $(patsubst %.o, $(OBJ_DIR)/%.o,$(OBJS))
DEPS = $(OBJECTS:.o=.d)

all: main

#automatic variable list $@ : [[$<] $^ ]
# $@ : target name 
# $< : first dependent file name 
# $^ : all dependent file
# $? : new dependent file more than target
# $+ : similar $^ but include reduplication file
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ -MD $(LDFLAGS)

$(TARGET) : $(OBJECTS)
	$(CC) $(CXXFLAGS) main.cpp $(OBJECTS) -o $(TARGET) $(LDFLAGS)

.PHONY: clean all
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)

-include $(DEPS)