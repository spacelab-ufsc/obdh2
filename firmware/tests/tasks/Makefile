TARGET_STARTUP=startup_unit_test
TARGET_PROCESS_TC=process_tc_unit_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../../
FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC) -I$(INC)/app/libs -I$(INC)/app/libs/libcsp-1.5.16/include -I$(INC)/tests/freertos_sim/ -Wl,--wrap=sys_log_init,--wrap=sys_log_print_event,--wrap=sys_log_print_event_from_module,--wrap=sys_log_print_msg,--wrap=sys_log_print_str,--wrap=sys_log_new_line,--wrap=sys_log_print_uint,--wrap=sys_log_print_int,--wrap=sys_log_print_hex,--wrap=sys_log_dump_hex,--wrap=sys_log_print_float,--wrap=sys_log_print_byte,--wrap=sys_log_print_system_time,--wrap=sys_log_print_license_msg,--wrap=sys_log_print_splash_screen,--wrap=sys_log_print_firmware_version

STARTUP_TEST_FLAGS=$(FLAGS),--wrap=leds_init,--wrap=led_set,--wrap=led_clear,--wrap=led_toggle,--wrap=current_sensor_init,--wrap=current_sensor_read_raw,--wrap=current_sensor_raw_to_ma,--wrap=current_sensor_read_ma,--wrap=voltage_sensor_init,--wrap=voltage_sensor_read_raw,--wrap=voltage_sensor_raw_to_mv,--wrap=voltage_sensor_read_mv,--wrap=temp_sensor_init,--wrap=temp_sensor_read_raw,--wrap=temp_sensor_raw_to_c,--wrap=temp_sensor_raw_to_k,--wrap=temp_sensor_read_c,--wrap=temp_sensor_read_k,--wrap=eps_init,--wrap=eps_get_bat_voltage,--wrap=eps_get_bat_current,--wrap=eps_get_bat_charge,--wrap=eps_get_data,--wrap=ttc_init,--wrap=ttc_get_data,--wrap=ttc_send,--wrap=ttc_recv,--wrap=ttc_avail,--wrap=ttc_enter_hibernation,--wrap=ttc_leave_hibernation,--wrap=watchdog_init,--wrap=watchdog_reset,--wrap=radio_init,--wrap=radio_send,--wrap=radio_recv,--wrap=radio_available,--wrap=radio_sleep,--wrap=media_init,--wrap=media_write,--wrap=media_read,--wrap=media_erase,--wrap=media_get_info,--wrap=antenna_init,--wrap=antenna_get_status,--wrap=antenna_deploy,--wrap=payload_init,--wrap=payload_enable,--wrap=payload_disable,--wrap=payload_write_cmd,--wrap=payload_get_data

PROCESS_TC_TEST_FLAGS=$(FLAGS),--wrap=eps_init,--wrap=eps_get_bat_voltage,--wrap=eps_get_bat_current,--wrap=eps_get_bat_charge,--wrap=eps_get_data,--wrap=ttc_init,--wrap=ttc_get_data,--wrap=ttc_send,--wrap=ttc_recv,--wrap=ttc_avail,--wrap=ttc_enter_hibernation,--wrap=ttc_leave_hibernation,--wrap=media_init,--wrap=media_write,--wrap=media_read,--wrap=media_erase,--wrap=media_get_info,--wrap=payload_init,--wrap=payload_enable,--wrap=payload_disable,--wrap=payload_write_cmd,--wrap=payload_get_data

.PHONY: all
all: startup_test process_tc_test

.PHONY: startup_test
startup_test: $(BUILD_DIR)/startup.o $(BUILD_DIR)/startup_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/leds_wrap.o $(BUILD_DIR)/current_sensor_wrap.o $(BUILD_DIR)/temp_sensor_wrap.o $(BUILD_DIR)/eps_wrap.o $(BUILD_DIR)/ttc+wrap.o $(BUILD_DIR)/watchdog_wrap.o $(BUILD_DIR)/radio_wrap.o $(BUILD_DIR)/media_wrap.o $(BUILD_DIR)/antenna_wrap.o $(BUILD_DIR)/payload_wrap.o
	$(CC) $(STARTUP_TEST_FLAGS) $(BUILD_DIR)/startup.o $(BUILD_DIR)/startup_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/leds_wrap.o $(BUILD_DIR)/current_sensor_wrap.o $(BUILD_DIR)/temp_sensor_wrap.o $(BUILD_DIR)/eps_wrap.o $(BUILD_DIR)/ttc_wrap.o $(BUILD_DIR)/watchdog_wrap.o $(BUILD_DIR)/radio_wrap.o $(BUILD_DIR)/media_wrap.o $(BUILD_DIR)/antenna_wrap.o $(BUILD_DIR)/payload_wrap.o -o $(BUILD_DIR)/$(TARGET_STARTUP) -lcmocka

.PHONY: process_tc_test
process_tc_test: $(BUILD_DIR)/process_tc.o $(BUILD_DIR)/process_tc_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/eps_wrap.o $(BUILD_DIR)/ttc_wrap.o $(BUILD_DIR)/media_wrap.o $(BUILD_DIR)/payload_wrap.o
	$(CC) $(PROCESS_TC_TEST_FLAGS) $(BUILD_DIR)/process_tc.o $(BUILD_DIR)/process_tc_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/eps_wrap.o $(BUILD_DIR)/ttc_wrap.o $(BUILD_DIR)/media_wrap.o $(BUILD_DIR)/payload_wrap.o -o $(BUILD_DIR)/$(TARGET_PROCESS_TC) -lcmocka

# Tasks
$(BUILD_DIR)/startup.o: ../../app/tasks/startup.c
	$(CC) $(STARTUP_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/process_tc.o: ../../app/tasks/process_tc.c
	$(CC) $(PROCESS_TC_TEST_FLAGS) -c $< -o $@

# Tests
$(BUILD_DIR)/startup_test.o: startup_test.c
	$(CC) $(STARTUP_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/process_tc_test.o: process_tc_test.c
	$(CC) $(PROCESS_TC_TEST_FLAGS) -c $< -o $@

# Mockups
$(BUILD_DIR)/sys_log_wrap.o: ../mockups/sys_log_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/leds_wrap.o: ../mockups/leds_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/current_sensor_wrap.o: ../mockups/current_sensor_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/temp_sensor_wrap.o: ../mockups/temp_sensor_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/eps_wrap.o: ../mockups/eps_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/ttc_wrap.o: ../mockups/ttc_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/watchdog_wrap.o: ../mockups/watchdog_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/radio_wrap.o: ../mockups/radio_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/media_wrap.o: ../mockups/media_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/antenna_wrap.o: ../mockups/antenna_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/payload_wrap.o: ../mockups/payload_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/task.o: ../freertos_sim/task.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET_STARTUP) $(BUILD_DIR)/$(TARGET_PROCESS_TC) $(BUILD_DIR)/*.o
