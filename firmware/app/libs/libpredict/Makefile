LIB_PREDICT_VERSION = 0.5

STATIC_TARGET := libpredict.a
SHARED_TARGET := libpredict.so.$(LIB_PREDICT_VERSION)

BUILD_DIR ?= $(CURDIR)

BUILD_DIR_ABS = $(abspath $(BUILD_DIR))

INSTALL_DIR ?= /usr/lib/

TOOLCHAIN_PREFIX ?= 
AR_FLAGS_APPEND ?= 
LD_FLAGS_APPEND ?= 
CC_FLAGS_APPEND ?= 

AR_FLAGS := rcs
LD_FLAGS := -shared -soname $(SHARED_TARGET) -lc -lm

AR_FLAGS += $(AR_FLAGS_APPEND)
LD_FLAGS += $(LD_FLAGS_APPEND)

AR := $(TOOLCHAIN_PREFIX)ar
LD := $(TOOLCHAIN_PREFIX)ld

.PHONY: all
all:
	@echo "Compiling libpredict..."
	$(MAKE) -C src BUILD_DIR=$(BUILD_DIR_ABS) TOOLCHAIN_PREFIX=$(TOOLCHAIN_PREFIX) CC_FLAGS_APPEND="$(CC_FLAGS_APPEND)"
	$(AR) $(AR_FLAGS) $(BUILD_DIR_ABS)/$(STATIC_TARGET) $(BUILD_DIR_ABS)/*.o
	$(LD) $(LD_FLAGS) -o $(BUILD_DIR_ABS)/$(SHARED_TARGET) $(BUILD_DIR_ABS)/*.o

.PHONY: install
install:
	@echo "Installing libpredict..."
	cp -r $(BUILD_DIR_ABS)/$(SHARED_TARGET) $(INSTALL_DIR)
	cp -r $(BUILD_DIR_ABS)/$(STATIC_TARGET) $(INSTALL_DIR)
	cp -r include/predict /usr/include/

.PHONY: uninstall
uninstall:
	@echo "Uninstalling libpredict..."
	rm $(INSTALL_DIR)/$(STATIC_TARGET)
	rm $(INSTALL_DIR)/$(SHARED_TARGET)
	rm -r /usr/include/predict/

.PHONY: clean
clean:
	rm $(BUILD_DIR_ABS)/*.o $(BUILD_DIR_ABS)/*.a $(BUILD_DIR_ABS)/*.so*
