CPP := g++
javac := javac
JAVA_HOME ?= /usr/lib/jvm/java-11-openjdk-amd64
JDK_INCLUDE_DIR := $(JAVA_HOME)/include
JDK_LIB_DIR := $(JAVA_HOME)/lib
APP_NAME := $(shell basename "$(PWD)")
LIB_NAME := bin/$(APP_NAME)
CPPFLAGS := -g -fPIC -I "$(JDK_INCLUDE_DIR)" -I "$(JDK_INCLUDE_DIR)/linux" -I "$(JDK_INCLUDE_DIR)/windows" -I "$(PWD)/include"
LINK_FLAGS := -L "$(JDK_LIB_DIR)" -L "$(JDK_LIB_DIR)/server" -ljvm

src_list := $(shell find src/ -type f -name '*.cpp')
obj_list := $(patsubst src/%,obj/%,$(patsubst %.cpp,%.o,$(src_list)))

java_src_list := $(shell find src/ -type f -name '*.java')
classfile_list := $(patsubst src/%,obj/%,$(patsubst %.java,%.class,$(java_src_list)))
bin_classfile_list := $(patsubst obj/%,bin/%,$(classfile_list))

all: $(LIB_NAME) $(bin_classfile_list)

clean:
	rm -rf bin obj

rebuild: clean $(LIB_NAME) $(bin_classfile_list)

src/%.cpp : src/%.h
	@touch $@ #re-build the CPP file if the .H file has been modified.

obj/%.class : src/%.java
	@mkdir -p $(shell dirname $@)
	$(javac) -d "$(shell dirname $@)" -h "$(PWD)/include" $^

obj/%.o : src/%.cpp
	@mkdir -p $(shell dirname $@)
	$(CPP) -c $(CPPFLAGS) $< -o $@ #compile to object file as intermediate step.

bin/%.class : obj/%.class
	@mkdir -p $(shell dirname $@)
	@cp $< $@

$(LIB_NAME) : $(obj_list)
	@mkdir -p $(shell dirname $@)
	$(CPP) $(CPPFLAGS) $^ $(LINK_FLAGS) -o $@ 