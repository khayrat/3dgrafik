#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lGLEW -std=c+11 -framework OpenGL

#g++ opengl.cpp  -lGLEW -lSDL2 -std=c++11 -o Test -framework OpenGL

#specifies the name of our exectuable
EXE = application

# add sources files here
SRCS :=

#OBJS specifies which files to compile as part of the project
OBJS = $(SRCS:.cpp=.o)

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXE)

clean:
		rm *.o && rm $(EXE)

