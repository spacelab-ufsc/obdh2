TARGET_CY15X102QN=cy15x102qn_unit_test
TARGET_TPS382X=tps382x_unit_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../../
FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC) -I../../tests/freertos_sim/ -Wl,--wrap=sys_log_init,--wrap=sys_log_print_event,--wrap=sys_log_print_event_from_module,--wrap=sys_log_print_msg,--wrap=sys_log_print_str,--wrap=sys_log_new_line,--wrap=sys_log_print_uint,--wrap=sys_log_print_int,--wrap=sys_log_print_hex,--wrap=sys_log_dump_hex,--wrap=sys_log_print_float,--wrap=sys_log_print_byte,--wrap=sys_log_print_system_time,--wrap=sys_log_print_license_msg,--wrap=sys_log_print_splash_screen,--wrap=sys_log_print_firmware_version

CY15X102QN_TEST_FLAGS=$(FLAGS),--wrap=spi_init,--wrap=spi_select_slave,--wrap=spi_write,--wrap=spi_read,--wrap=spi_transfer,--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle

TPS382X_TEST_FLAGS=$(FLAGS),--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle

.PHONY: all
all: cy15x102qn_test tps382x_test

.PHONY: cy15x102qn_test
cy15x102qn_test: $(BUILD_DIR)/cy15x102qn.o $(BUILD_DIR)/cy15x102qn_gpio.o $(BUILD_DIR)/cy15x102qn_spi.o $(BUILD_DIR)/cy15x102qn_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/spi_wrap.o $(BUILD_DIR)/gpio_wrap.o
	$(CC) $(CY15X102QN_TEST_FLAGS) $(BUILD_DIR)/cy15x102qn.o $(BUILD_DIR)/cy15x102qn_gpio.o $(BUILD_DIR)/cy15x102qn_spi.o $(BUILD_DIR)/cy15x102qn_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/spi_wrap.o $(BUILD_DIR)/gpio_wrap.o -o $(BUILD_DIR)/$(TARGET_CY15X102QN) -lcmocka

.PHONY: tps382x_test
tps382x_test: $(BUILD_DIR)/tps382x.o $(BUILD_DIR)/tps382x_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o
	$(CC) $(TPS382X_TEST_FLAGS) $(BUILD_DIR)/tps382x.o $(BUILD_DIR)/tps382x_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o -o $(BUILD_DIR)/$(TARGET_TPS382X) -lcmocka

# Drivers
$(BUILD_DIR)/cy15x102qn.o: ../../drivers/cy15x102qn/cy15x102qn.c
	$(CC) $(CY15X102QN_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/cy15x102qn_gpio.o: ../../drivers/cy15x102qn/cy15x102qn_gpio.c
	$(CC) $(CY15X102QN_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/cy15x102qn_spi.o: ../../drivers/cy15x102qn/cy15x102qn_spi.c
	$(CC) $(CY15X102QN_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/tps382x.o: ../../drivers/tps382x/tps382x.c
	$(CC) $(TPS382X_TEST_FLAGS) -c $< -o $@

# Tests
$(BUILD_DIR)/cy15x102qn_test.o: cy15x102qn_test.c
	$(CC) $(CY15X102QN_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/tps382x_test.o: tps382x_test.c
	$(CC) $(TPS382X_TEST_FLAGS) -c $< -o $@

# Mockups
$(BUILD_DIR)/sys_log_wrap.o: ../mockups/system/sys_log_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/adc_wrap.o: ../mockups/drivers/adc_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/gpio_wrap.o: ../mockups/drivers/gpio_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/wdt_wrap.o: ../mockups/drivers/wdt_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/flash_wrap.o: ../mockups/drivers/flash_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/spi_wrap.o: ../mockups/drivers/spi_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/task.o: ../freertos_sim/task.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET_CY15X102QN) $(BUILD_DIR)/$(TARGET_TPS382X) $(BUILD_DIR)/*.o
