
TOP_DIR:=$(CURDIR)
Target:=main
SRC_PATH:=$(TOP_DIR) $(TOP_DIR)/source
BUILD_PATH:=$(TOP_DIR)/build
OBJ_PATH:=$(BUILD_PATH)/temp
BIN_PATH:=$(BUILD_PATH)/bin
SRC:=$(foreach dir,$(SRC_PATH),$(wildcard $(dir)/*.cpp))
SRC_WITHOUT_DIR:=$(notdir $(SRC))
OBJ_WITHOUT_DIR:=$(patsubst %.cpp,%.o,$(SRC_WITHOUT_DIR))
OBJ_WITH_BUILD_DIR:=$(addprefix $(OBJ_PATH)/,$(OBJ_WITHOUT_DIR))
$(info "OBJ_WITH_BUILD_DIR:$(OBJ_WITH_BUILD_DIR)")
CFLAGS=$(addprefix ,-I $(SRC_PATH))
VPATH=$(SRC_PATH)
all:build_prepare $(Target)
$(Target):$(OBJ_WITH_BUILD_DIR) 
	cc -o $(BIN_PATH)/$@ $^
$(OBJ_PATH)/%.o:%.cpp
	cc -c $(CFLAGS) -o $@ $<
build_prepare:
	@if [ ! -d $(BUILD_PATH) ]; then \
	mkdir -p $(OBJ_PATH); \
	mkdir -p $(BIN_PATH); \
	fi

.PHONY:clean

clean:
	-rm -rf $(BIN_PATH)/$(Target) $(OBJ_WITH_BUILD_DIR)