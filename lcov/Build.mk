# 头文件目录
INCLUDES:=.
# 除了当前目录外，其他的源代码文件目录           
SRCDIR:=    
GTEST_INC_DIR = /usr/local/include
GMOCK_INC_DIR = /usr/local/include
GMOCK_LIB_DIR = /usr/local/lib
# 静态库目录
LIBDIR:=
# 静态库文件名
LIBS=
RM=-rm -f
CC=g++
CFLAGS=
# 依赖文件的搜索路径
VPATH= $(INCLUDES) $(SRCDIR)
# 要编译的源文件
SOURCE=fool.cpp
# C预处理器参数
CFLAGS=
# C++预处理器参数
CPPFLAGS = -I"$(GTEST_INC_DIR)" -I"$(GMOCK_INC_DIR)" -I"$(INCLUDES)" -I"$(SRCDIR)"
# C C++语言编译选项
CXXFLAGS = -g -Wall -Wextra -pthread 
# 连接选项:库,路径
LDFLAGS = -lrt -lpthread -lgmock -L"$(GMOCK_LIB_DIR)"
# Obj文件
OBJS=$(SOURCE:.cpp=.o)

# 可执行文件
TARGET=fool

all:$(TARGET)
	
$(TARGET) : $(SOURCE:.cpp=.o)
	$(CC) $^ $(LDFLAGS)  -o $@

$(SOURCE:.cpp=.o):$(SOURCE)
	$(CC) $(CPPFLAGS) $(CXXFLAGS)  -c -o $@ $<

# 头文件,依赖文件
include $(SOURCE:.cpp=.d)
%.d : %.cpp
	@ rm -f $@; \
		$(CC) -MM -Wall $<  > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$''
.PHONY : clean
clean:
	$(RM) $(TARGET) $(SOURCE:.cpp=.o)
