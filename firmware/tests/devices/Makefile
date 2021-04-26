TARGET_CURRENT_SENSOR=current_sensor_unit_test
TARGET_VOLTAGE_SENSOR=voltage_sensor_unit_test
TARGET_TEMP_SENSOR=temp_sensor_unit_test
TARGET_LEDS=leds_unit_test
TARGET_WATCHDOG=watchdog_unit_test
TARGET_RADIO=radio_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../../
FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC) -Wl,--wrap=sys_log_print_event_from_module,--wrap=sys_log_new_line,--wrap=sys_log_print_msg,--wrap=sys_log_print_uint,--wrap=sys_log_print_float,--wrap=adc_init,--wrap=adc_read,--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle,--wrap=wdt_init,--wrap=wdt_reset,--wrap=tps382x_init,--wrap=tps382x_trigger

all: current_sensor_test voltage_sensor_test temp_sensor_test leds_test watchdog_test

current_sensor_test: $(BUILD_DIR)/current_sensor.o $(BUILD_DIR)/current_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o
	$(CC) $(FLAGS) $(BUILD_DIR)/current_sensor.o $(BUILD_DIR)/current_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o -o $(BUILD_DIR)/$(TARGET_CURRENT_SENSOR) -lcmocka

voltage_sensor_test: $(BUILD_DIR)/voltage_sensor.o $(BUILD_DIR)/voltage_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o
	$(CC) $(FLAGS) $(BUILD_DIR)/voltage_sensor.o $(BUILD_DIR)/voltage_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o -o $(BUILD_DIR)/$(TARGET_VOLTAGE_SENSOR) -lcmocka

temp_sensor_test: $(BUILD_DIR)/temp_sensor.o $(BUILD_DIR)/temp_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o
	$(CC) $(FLAGS) $(BUILD_DIR)/temp_sensor.o $(BUILD_DIR)/temp_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o -o $(BUILD_DIR)/$(TARGET_TEMP_SENSOR) -lcmocka

leds_test: $(BUILD_DIR)/leds.o $(BUILD_DIR)/leds_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o
	$(CC) $(FLAGS) $(BUILD_DIR)/leds.o $(BUILD_DIR)/leds_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o -o $(BUILD_DIR)/$(TARGET_LEDS) -lcmocka

watchdog_test: $(BUILD_DIR)/watchdog.o $(BUILD_DIR)/watchdog_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/wdt_wrap.o $(BUILD_DIR)/tps382x_wrap.o
	$(CC) $(FLAGS) $(BUILD_DIR)/watchdog.o $(BUILD_DIR)/watchdog_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/wdt_wrap.o $(BUILD_DIR)/tps382x_wrap.o -o $(BUILD_DIR)/$(TARGET_WATCHDOG) -lcmocka

radio_test: $(BUILD_DIR)/.o
	$(CC) $(FLAGS) $(BUILD_DIR)/*.o -o $(BUILD_DIR)/$(TARGET_RADIO)

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
	$(CC) $(FLAGS) -c $< -o $@

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
	$(CC) $(FLAGS) -c $< -o $@

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

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET_CURRENT_SENSOR) $(BUILD_DIR)/$(TARGET_VOLTAGE_SENSOR) $(BUILD_DIR)/$(TARGET_TEMP_SENSOR) $(BUILD_DIR)/$(TARGET_LEDS) $(BUILD_DIR)/$(TARGET_WATCHDOG) $(BUILD_DIR)/$(TARGET_RADIO) $(BUILD_DIR)/*.o

