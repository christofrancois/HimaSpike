CXX ?= g++
CC ?= gcc

OBJECTS_PATH = objects
SOURCES_PATH = ../src/
LOCAL_INCLUDES = ../include
LIBS = -L.
PACKAGES = 

SOURCES = $(SOURCES_PATH)/main.cpp

OBJECTS  = $(addprefix $(OBJECTS_PATH)/,$(patsubst %.cpp,%.o, $(abspath $(SOURCES))))
DEPS     = $(addprefix $(OBJECTS_PATH)/,$(patsubst %.cpp,%.d, $(abspath $(SOURCES))))
INCLUDES = $(addprefix -I,$(LOCAL_INCLUDES))
LIBS    += $(addprefix -framework ,$(FRAMEWORKS))

CFLAGS += -std=c++11 -Werror

define check-pkg
  ifneq ($$(shell pkg-config --exists $(1) && echo OK),OK)
    $$(error $(1) not found)
  endif
  INCLUDES += $$(shell pkg-config --cflags $(1))
  LIBS += $$(shell pkg-config --libs $(1))
endef

$(foreach pkg,$(PACKAGES),$(eval $(call check-pkg,$(pkg))))

all: HimaSpike

clean:
	@rm -fr $(OBJECTS_PATH)
	@rm -f HimaSpike

-include $(DEPS)

$(OBJECTS_PATH)/%.o: %.cpp
	@echo [C++] $<
	@mkdir -p $(@D)
	@$(CXX) -c $< -o $@ $(INCLUDES) $(CFLAGS) -MD

$(OBJECTS_PATH)/%.o: %.c
	@echo [C] $<
	@mkdir -p $(@D)
	@$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS) -MD

HimaSpike: $(OBJECTS)
	@echo [LINK] $@
	@$(CXX) -o $@ $(OBJECTS) $(LIBS)

