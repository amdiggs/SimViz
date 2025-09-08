CXX = clang++
CXXFLAGS = -Wall -std=c++17

TARGET = Test
SOURCE_DIR = .
SOURCES = testing.cpp
MLIB_PATH=../../Math
UNAME_S := $(shell uname -s)
#local Libs
INC_PATHS = ../../Math
#INC_PATHS += /opt/homebrew/Cellar/freetype/2.13.3/include/freetype2
LIB_PATHS = ../../Math
LDFLAGS = $(addprefix -L,$(LIB_PATHS))

#Platform includes and libs
ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	#LIBS += -framework OpenGL -framework OpenCL
	#LDFLAGS += -L/usr/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw -lGLEW -lfreetype

	INC_PATHS += /usr/local/include /opt/local/include /opt/homebrew/include
endif

ifeq ($(OS), Windows_NT) #GROSS
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += `pkg-config --cflags glfw3`
endif
#
LIBS =-lAMDmath
IFLAGS = $(addprefix -I,$(INC_PATHS))
CXXFLAGS += $(IFLAGS)
CFLAGS = $(CXXFLAGS)

#clang++ -dynamiclib -o libmylibrary.dylib mylibrary.cpp -install_name @rpath/libmylibrary.dylib

all: $(TARGET)
$(TARGET): $(SOURCES)
	@echo "$(ECHO_MESSAGE)"
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS) $(LIBS) -Wl,-rpath,$(MLIB_PATH)

.PHONY: clean

clean:
	rm -f $(TARGET)
