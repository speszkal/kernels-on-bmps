OBJS = main.cpp src/Image.cpp src/globals.cpp

OBJS_NAME = main.exe

INCLUDE_PATHS = 

LIBRARY_PATHS = 

LINKER_FLAGS = #-lmingw32 -fconcepts

all:
	g++ $(INCLUDE_PATHS) $(LIBRARY_PATHS) -o $(OBJS_NAME) $(OBJS) $(LINKER_FLAGS)