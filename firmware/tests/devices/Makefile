TARGET_CURRENT_SENSOR=current_sensor_unit_test
TARGET_VOLTAGE_SENSOR=voltage_sensor_unit_test
TARGET_TEMP_SENSOR=temp_sensor_unit_test
TARGET_LEDS=leds_unit_test
TARGET_WATCHDOG=watchdog_unit_test
TARGET_RADIO=radio_unit_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../../
FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC) -Wl,--wrap=sys_log_print_event_from_module,--wrap=sys_log_new_line,--wrap=sys_log_print_msg,--wrap=sys_log_print_uint,--wrap=sys_log_print_int,--wrap=sys_log_print_float,--wrap=adc_init,--wrap=adc_read,--wrap=adc_temp_get_mref,--wrap=adc_temp_get_nref,--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle,--wrap=wdt_init,--wrap=wdt_reset,--wrap=tps382x_init,--wrap=tps382x_trigger

RADIO_TEST_FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC) -I../../config/ -I../../tests/freertos_sim/ -Wl,--wrap=sys_log_print_event_from_module,--wrap=sys_log_new_line,--wrap=sys_log_print_msg,--wrap=sys_log_print_uint,--wrap=sys_log_print_int,--wrap=sys_log_print_hex,--wrap=sys_log_print_float,--wrap=si446x_reset,--wrap=si446x_part_info,--wrap=si446x_part_info,--wrap=si446x_configuration_init,--wrap=si446x_set_property,--wrap=si446x_set_property,--wrap=si446x_fifo_info,--wrap=si446x_write_tx_fifo,--wrap=si446x_spi_init,--wrap=si446x_get_int_status,--wrap=si446x_start_tx,--wrap=si446x_get_int_status,--wrap=si446x_delay_ms,--wrap=si446x_change_state,--wrap=si446x_read_rx_fifo,--wrap=si446x_start_rx

.PHONY: all
all: current_sensor_test voltage_sensor_test temp_sensor_test leds_test watchdog_test radio_test

.PHONY: current_sensor_test
current_sensor_test: $(BUILD_DIR)/current_sensor.o $(BUILD_DIR)/current_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o
	$(CC) $(FLAGS) $(BUILD_DIR)/current_sensor.o $(BUILD_DIR)/current_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o -o $(BUILD_DIR)/$(TARGET_CURRENT_SENSOR) -lcmocka

.PHONY: voltage_sensor_test
voltage_sensor_test: $(BUILD_DIR)/voltage_sensor.o $(BUILD_DIR)/voltage_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o
	$(CC) $(FLAGS) $(BUILD_DIR)/voltage_sensor.o $(BUILD_DIR)/voltage_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o -o $(BUILD_DIR)/$(TARGET_VOLTAGE_SENSOR) -lcmocka

.PHONY: temp_sensor_test
temp_sensor_test: $(BUILD_DIR)/temp_sensor.o $(BUILD_DIR)/temp_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o
	$(CC) $(FLAGS) $(BUILD_DIR)/temp_sensor.o $(BUILD_DIR)/temp_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o -o $(BUILD_DIR)/$(TARGET_TEMP_SENSOR) -lcmocka

.PHONY: leds_test
leds_test: $(BUILD_DIR)/leds.o $(BUILD_DIR)/leds_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o
	$(CC) $(FLAGS) $(BUILD_DIR)/leds.o $(BUILD_DIR)/leds_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o -o $(BUILD_DIR)/$(TARGET_LEDS) -lcmocka

.PHONY: watchdog_test
watchdog_test: $(BUILD_DIR)/watchdog.o $(BUILD_DIR)/watchdog_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/wdt_wrap.o $(BUILD_DIR)/tps382x_wrap.o
	$(CC) $(FLAGS) $(BUILD_DIR)/watchdog.o $(BUILD_DIR)/watchdog_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/wdt_wrap.o $(BUILD_DIR)/tps382x_wrap.o -o $(BUILD_DIR)/$(TARGET_WATCHDOG) -lcmocka

.PHONY: radio_test
radio_test: $(BUILD_DIR)/radio.o $(BUILD_DIR)/radio_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/si446x_wrap.o $(BUILD_DIR)/task.o
	$(CC) $(RADIO_TEST_FLAGS) $(BUILD_DIR)/radio.o $(BUILD_DIR)/radio_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/si446x_wrap.o $(BUILD_DIR)/task.o -o $(BUILD_DIR)/$(TARGET_RADIO) -lm -lcmocka

$(BUILD_DIR)/current_sensor.o: ../../devices/current_sensor/current_sensor.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/voltage_sensor.o: ../../devices/voltage_sensor/voltage_sensor.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/temp_sensor.o: ../../devices/temp_sensor/temp_sensor.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/leds.o: ../../devices/leds/leds.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/watchdog.o: ../../devices/watchdog/watchdog.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/radio.o: ../../devices/radio/radio.c
	$(CC) $(RADIO_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/current_sensor_test.o: current_sensor_test.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/voltage_sensor_test.o: voltage_sensor_test.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/temp_sensor_test.o: temp_sensor_test.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/leds_test.o: leds_test.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/watchdog_test.o: watchdog_test.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/radio_test.o: radio_test.c
	$(CC) $(RADIO_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/sys_log_wrap.o: ../mockups/sys_log_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/adc_wrap.o: ../mockups/adc_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/gpio_wrap.o: ../mockups/gpio_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/wdt_wrap.o: ../mockups/wdt_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/tps382x_wrap.o: ../mockups/tps382x_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x_wrap.o: ../mockups/si446x_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/task.o: ../freertos_sim/task.c
	$(CC) $(RADIO_TEST_FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET_CURRENT_SENSOR) $(BUILD_DIR)/$(TARGET_VOLTAGE_SENSOR) $(BUILD_DIR)/$(TARGET_TEMP_SENSOR) $(BUILD_DIR)/$(TARGET_LEDS) $(BUILD_DIR)/$(TARGET_WATCHDOG) $(BUILD_DIR)/$(TARGET_RADIO) $(BUILD_DIR)/*.o

