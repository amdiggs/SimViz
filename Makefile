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

TARGET = SimViz
SOURCE_DIR = .
SOURCES = $(shell find $(SOURCE_DIR) -name '*.cpp')
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

#local Libs
INC_PATHS = ./Math ./Resources ./Rendering ./SimViz ./SimViz/MyOpenCL
LIB_PATHS = \
	./Resources/imgui/examples/libs/glfw/lib-vc2010-64 \
	./Resources/imgui/examples/libs/glfw/lib-vc2010-32 \
	./Math
LDFLAGS = $(addprefix -L,$(LIB_PATHS))
LIBS = -lAMDmath

#Platform includes and libs
ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework OpenCL
	LDFLAGS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
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


%.o:./Rendering/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./Resources/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./SimViz/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(TARGET)
	+$(MAKE) -C ./Math
$(TARGET): $(OBJS)
	@echo "$(ECHO_MESSAGE)"
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(TARGET)
