
TOP_DIR:=$(CURDIR)
Target:=main
SRC_PATH:=$(TOP_DIR) $(TOP_DIR)/source
#设置编译目
BUILD_PATH:=$(TOP_DIR)/build
#设置编译临时目录
OBJ_PATH:=$(BUILD_PATH)/temp
#设置编译最终文件目录
BIN_PATH:=$(BUILD_PATH)/bin
#获取源文件目录下所有c文件列表
SRC:=$(foreach dir,$(SRC_PATH),$(wildcard $(dir)/*.cpp))
#去掉c文件目录
SRC_WITHOUT_DIR:=$(notdir $(SRC))
#生成.o文件列表
OBJ_WITHOUT_DIR:=$(patsubst %.cpp,%.o,$(SRC_WITHOUT_DIR))
#为.o文件列表加上编译目录
OBJ_WITH_BUILD_DIR:=$(addprefix $(OBJ_PATH)/,$(OBJ_WITHOUT_DIR))
$(info "OBJ_WITH_BUILD_DIR:$(OBJ_WITH_BUILD_DIR)")
#添加头文件目录
CFLAGS=$(addprefix -I,$(SRC_PATH))
#为gcc添加源文件搜索目录
VPATH=$(SRC_PATH)
#编译目标
all:build_prepare $(Target)
#连接目标
$(Target):$(OBJ_WITH_BUILD_DIR) 
	cc -o $(BIN_PATH)/$@ $^
#编译生成.o文件
$(OBJ_PATH)/%.o:%.cpp
	cc -c $(CFLAGS) -o $@ $<
#创建编译目录
build_prepare:
	@if [ ! -d $(BUILD_PATH) ]; then \
	mkdir -p $(OBJ_PATH); \
	mkdir -p $(BIN_PATH); \
	fi

.PHONY:clean

clean:
	-rm -rf $(BIN_PATH)/$(Target) $(OBJ_WITH_BUILD_DIR)