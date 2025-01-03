# Makefile by S739_Lucie - January 18th 2023
# ====== Everything Command Calls related ======

MAKEFLAGS += --no-print-directory

USED_OS := ERROR
CLS     := clear

ifdef OS
	USED_OS := Windows
endif

ifeq ($(shell uname), Linux)
	USED_OS := Linux
endif

# ========= Everything project related =========

PROJ    := libLuEngineC
TARGET  := $(PROJ).a
DTARGET := $(PROJ)_debug.a

STATIC_PATH := errorLib

ifeq ($(USED_OS), Windows)
	STATIC_PATH := C:/CustomLibs
else ifeq ($(USED_OS), Linux)
	STATIC_PATH := /usr/local
endif

STATIC_LIB_PATH := $(STATIC_PATH)/lib
STATIC_H_PATH   := $(STATIC_PATH)/include/LuEngineC

EXT     := c
COMP    := gcc
LINK    := ar -rcs

# ========== Everything files related ==========

INC_DIR   := include
SRC_DIR   := src
OBJ_DIR   := objs

SRC_FILES := $(wildcard $(SRC_DIR)/*.$(EXT)) $(wildcard $(SRC_DIR)/**/*.$(EXT)) 
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.$(EXT)=$(OBJ_DIR)/$(USED_OS)/%.o)
INC_FILES := $(wildcard $(INC_DIR)/*.h)

# ========== Everything flags related ==========

O_FLAGS := -O3 -std=c2x -Wall -Wextra -Werror -Wfatal-errors -pedantic -pedantic-errors -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option
O_FLAGS += -I include

ifeq ($(USED_OS), Windows)
	O_FLAGS += -I C:/CustomLibs/include
endif

# =========== Every usable functions ===========

# Basic Build

all:
	@$(MAKE) --silent build

reinstall:
	@$(MAKE) --silent clean
	@$(MAKE) --silent install

install:
	@$(MAKE) --silent build
ifeq ($(USED_OS), Linux)
	@mkdir -p $(STATIC_LIB_PATH)
	@echo Installing $(TARGET) at /usr/local/lib/
	@cp --update $(TARGET) $(STATIC_LIB_PATH)/$(notdir $(TARGET))

	@mkdir -p $(STATIC_H_PATH)/
	@echo Installing the headers at $(STATIC_H_PATH)/
	@cp --update LuEngine.h $(STATIC_PATH)/include/LuEngine.h
	@$(MAKE) --silent installHeaders

else ifeq ($(USED_OS), Windows)
	@mkdir -p $(STATIC_LIB_PATH)
	@echo Installing $(TARGET) at C:/CustomLibs/lib/
	@cp --update $(TARGET) $(STATIC_LIB_PATH)/$(notdir $(TARGET))

	@mkdir -p $(STATIC_H_PATH)/
	@echo Installing the headers at $(STATIC_H_PATH)/
	@echo Installing LuEngine.h
	@cp --update LuEngine.h $(STATIC_PATH)/include/LuEngine.h
	@$(MAKE) --silent installHeaders

else
	@echo Can't automatically install this lib on your system
endif


installHeaders: $(patsubst $(INC_DIR)/%, $(STATIC_H_PATH)/%, $(INC_FILES))
$(STATIC_H_PATH)/%.h: $(INC_DIR)/%.h
	@echo installing $(patsubst %.o, %,$(notdir $@))
	@cp --update $< $@

libHeader: 
	@$(RM) LuEngine.h
	@touch LuEngine.h
	@echo "#ifndef LU_ENGINE_H" >> LuEngine.h
	@echo "#define LU_ENGINE_H" >> LuEngine.h
	@echo "" >> LuEngine.h
	@./createLibHeader.sh
	@echo "" >> LuEngine.h
	@echo "#endif  // LU_ENGINE_H" >> LuEngine.h

build: $(TARGET) 
	@$(MAKE) --silent libHeader

$(TARGET): $(OBJ_FILES)
	@echo Linking the static library
	@$(LINK) $@ $^ 

# Debug Build

debug: $(DTARGET)

$(DTARGET) : $(OBJ_FILES)
	@echo Linking the static library
	@$(LINK) $@ $^ 

# Obj and Header files compiling

objects: $(OBJ_FILES)
$(OBJ_DIR)/$(USED_OS)/%.o: $(SRC_DIR)/%.$(EXT)
	@echo Compiling $(patsubst %.o, %,$(notdir $@))
	@$(COMP) -c $< -o $@ $(O_FLAGS)


# File Cleaners

clean:
	$(RM) $(OBJ_DIR)/$(USED_OS)/*.o
	$(RM) $(OBJ_DIR)/$(USED_OS)/**/*.o
	$(RM) $(PROJ).a
	$(RM) $(PROJ).lib
	$(RM) $(PROJ)_debug.a
	$(RM) $(PROJ)_debug.lib
	$(RM) LuEngine.h
	$(CLS)

# Makefile Debugging/Usefull Functions

subDir:
	mkdir -p $(sort $(dir $(OBJ_FILES)))
	touch -f $(addsuffix DO_NOT_REMOVE,$(sort $(dir $(OBJ_FILES))))

rebuild:
	@$(MAKE) --silent clean
	@$(MAKE) --silent build

# Makefile Debugging/Usefull Functions
showOS: 
	@echo $(USED_OS)

showFiles:
	@echo Source files
	@echo $(SRC_FILES)
	@echo 
	@echo Object files
	@echo $(OBJ_FILES)
	@echo 
	@echo Header files
	@echo $(INC_FILES)
	@echo 
	@echo Install paths
	@echo "lib path     :" $(STATIC_LIB_PATH)
	@echo "headers path :" $(STATIC_H_PATH)
	@echo 
	@echo "O Flags : " $(O_FLAGS)
