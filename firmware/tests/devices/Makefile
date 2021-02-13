TARGET_CURRENT_SENSOR=current_sensor_unit_test
TARGET_VOLTAGE_SENSOR=voltage_sensor_unit_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../../
FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC) -Wl,--wrap=sys_log_print_event_from_module,--wrap=sys_log_new_line,--wrap=sys_log_print_msg,--wrap=sys_log_print_uint,--wrap=adc_init,--wrap=adc_read -lcmocka

all: current_sensor voltage_sensor

current_sensor: $(BUILD_DIR)/current_sensor.o $(BUILD_DIR)/current_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o
	$(CC) $(FLAGS) $(BUILD_DIR)/*.o -o $(BUILD_DIR)/$(TARGET_CURRENT_SENSOR)

voltage_sensor: $(BUILD_DIR)/voltage_sensor.o $(BUILD_DIR)/voltage_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o
	$(CC) $(FLAGS) $(BUILD_DIR)/*.o -o $(BUILD_DIR)/$(TARGET_VOLTAGE_SENSOR)

$(BUILD_DIR)/current_sensor.o: ../../devices/current_sensor/current_sensor.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/voltage_sensor.o: ../../devices/voltage_sensor/voltage_sensor.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/current_sensor_test.o: current_sensor_test.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/voltage_sensor_test.o: voltage_sensor_test.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/sys_log_wrap.o: ../mockups/sys_log_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/adc_wrap.o: ../mockups/adc_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm $(BUILD_DIR)/$(TARGET_CURRENT_SENSOR) $(BUILD_DIR)/$(TARGET_VOLTAGE_SENSOR) $(BUILD_DIR)/*.o
