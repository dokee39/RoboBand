RED := \033[0;31m
RED_BLOD := \033[1;31m
GREEN := \033[0;32m
GREEN_BLOD := \033[1;32m
YELLOW := \033[0;33m
YELLOW_BLOD := \033[1;33m
BLUE := \033[0;34m
BLUE_BLOD := \033[1;34m
MAGENTA := \033[0;35m
MAGENTA_BLOD := \033[1;35m
CYAN := \033[0;36m
CYAN_BLOD := \033[1;36m
END := \033[0m

TARGET := $(shell basename "$$PWD")

DEBUG := 1

CC := g++
OPT := -std=c++17 -og -g
DEF := \

BUILD_DIR := ./build
SRC_DIRS := \
src
LDFLAGS := \
-lm \
-ldl \
-lrt \
-lpthread \
-lboost_system \
-lboost_thread

SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.cc' -or -name '*.cpp')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIRS) -type d)

INC_DIRS += $(WEBOTS_HOME)/include/controller/cpp
LDFLAGS += -L$(WEBOTS_HOME)/lib/controller -lCppController

CPPFLAGS := $(addprefix -D, $(DEF)) $(addprefix -I, $(INC_DIRS)) $(OPT) -Wall

.PHONY: all run clean

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS) | $(BUILD_DIR)
	@echo -e "$(YELLOW_BLOD)  ->$(END) $(CYAN)LD$(END) $@"
	@$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.c.o: %.c Makefile | $(BUILD_DIR)
	@echo -e "$(YELLOW_BLOD)  ->$(END) $(GREEN)CC$(END) $<"
	@mkdir -p $(dir $@) 
	@$(CC) -o $@ -c $< $(CPPFLAGS)
$(BUILD_DIR)/%.cc.o: %.cc Makefile | $(BUILD_DIR)
	@echo -e "$(YELLOW_BLOD)  ->$(END) $(GREEN)CC$(END) $<"
	@mkdir -p $(dir $@) 
	@$(CC) -c $(CPPFLAGS) $< -o $@
$(BUILD_DIR)/%.cpp.o: %.cpp Makefile | $(BUILD_DIR)
	@echo -e "$(YELLOW_BLOD)  ->$(END) $(GREEN)CC$(END) $<"
	@mkdir -p $(dir $@) 
	@$(CC) -o $@ -c $< $(CPPFLAGS)

$(BUILD_DIR):
	@mkdir $@

run:
	@echo -e "$(YELLOW_BLOD)  ->$(END) $(YELLOW)starting...$(END)"
	@-$(BUILD_DIR)/$(TARGET)

clean:
	@echo -e "$(YELLOW_BLOD)  ->$(END) $(RED)cleaning...$(END)"
	@-rm -fR $(BUILD_DIR)

-include $(DEPS)


