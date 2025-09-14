#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

CXX = clang++
CXXFLAGS = -glldb -Wall -std=c++17

TARGET = sim_viz
SOURCE_DIR = .
SOURCES = $(shell find $(SOURCE_DIR) -name '*.cpp')
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

#local Libs
INC_PATHS = ./Math ./Resources ./Rendering ./SimViz ./SimViz/MyOpenCL
INC_PATHS += /opt/homebrew/Cellar/freetype/2.13.3/include/freetype2
LIB_PATHS = 
#LDFLAGS = $(addprefix -L,$(LIB_PATHS))
#LIBS = -lAMDmath

#Platform includes and libs
ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework OpenCL
	LDFLAGS += -L/usr/local/lib -L/opt/homebrew/lib
	LIBS += -lglfw -lGLEW -lfreetype

	INC_PATHS += /usr/local/include /opt/local/include /opt/homebrew/include
endif

ifeq ($(OS), Windows_NT) #GROSS
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += `pkg-config --cflags glfw3`
endif
#
IFLAGS = $(addprefix -I,$(INC_PATHS))
CXXFLAGS += $(IFLAGS)
CFLAGS = $(CXXFLAGS)


%.o:./Math/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./Rendering/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./Resources/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./SimViz/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(TARGET)
	@echo Built Objects
$(TARGET): $(OBJS)
	@echo "$(ECHO_MESSAGE)"
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS) $(LIBS) > build_err.txt 2>&1

.PHONY: clean

clean:
	rm -f *.o $(TARGET)
