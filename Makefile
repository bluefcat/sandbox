CC = g++
CXXFLAGS = -O2 -std=c++17 -w

LDFLAGS = 

TARGET_DIR = ${target_dir}

SRCS = $(wildcard $(TARGET_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJECTS:.o=.d)

#automatic variable list $@ : [[$<] $^ ]
# $@ : target name 
# $< : first dependent file name 
# $^ : all dependent file
# $? : new dependent file more than target
# $+ : similar $^ but include reduplication file

$(OBJS) : $(SRCS)
	$(CC) $(CXXFLAGS) -c $< -o $@ -MD $(LDFLAGS)

.PHONY: clean all
clean:
	rm -f ./object/*

-include $(DEPS)