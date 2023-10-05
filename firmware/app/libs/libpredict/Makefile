TARGET=libpredict.a

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

AR=ar
FLAGS=rcs

.PHONY: all
all:
	@echo "Compiling libpredict..."
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C src
	$(AR) $(FLAGS) $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/*.o

.PHONY: install
install:
	@echo "Installing libpredict driver..."
	cp -r $(BUILD_DIR)/$(TARGET) /usr/lib/
	cp -r include/predict /usr/include/

.PHONY: uninstall
uninstall:
	@echo "Uninstalling libpredict driver..."
	rm /usr/lib/$(TARGET)
	rm -r /usr/include/predict/

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o $(BUILD_DIR)/*.a
