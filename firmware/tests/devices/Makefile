TARGET_CURRENT_SENSOR=current_sensor_unit_test
TARGET_VOLTAGE_SENSOR=voltage_sensor_unit_test
TARGET_TEMP_SENSOR=temp_sensor_unit_test
TARGET_LEDS=leds_unit_test
TARGET_WATCHDOG=watchdog_unit_test
TARGET_RADIO=radio_unit_test
TARGET_TTC=ttc_unit_test
TARGET_EPS=eps_unit_test
TARGET_ANTENNA=antenna_unit_test
TARGET_MEDIA=media_unit_test
TARGET_PAYLOAD=payload_unit_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../../
FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC) -I../../tests/freertos_sim/ -Wl,--wrap=sys_log_init,--wrap=sys_log_print_event,--wrap=sys_log_print_event_from_module,--wrap=sys_log_print_msg,--wrap=sys_log_print_str,--wrap=sys_log_new_line,--wrap=sys_log_print_uint,--wrap=sys_log_print_int,--wrap=sys_log_print_hex,--wrap=sys_log_dump_hex,--wrap=sys_log_print_float,--wrap=sys_log_print_byte,--wrap=sys_log_print_system_time,--wrap=sys_log_print_license_msg,--wrap=sys_log_print_splash_screen,--wrap=sys_log_print_firmware_version,--wrap=system_get_time

CURRENT_SENSOR_TEST_FLAGS=$(FLAGS),--wrap=adc_init,--wrap=adc_read,--wrap=adc_temp_get_mref,--wrap=adc_temp_get_nref

VOLTAGE_SENSOR_TEST_FLAGS=$(FLAGS),--wrap=adc_init,--wrap=adc_read,--wrap=adc_temp_get_mref,--wrap=adc_temp_get_nref

TEMP_SENSOR_TEST_FLAGS=$(FLAGS),--wrap=adc_init,--wrap=adc_read,--wrap=adc_temp_get_mref,--wrap=adc_temp_get_nref

LEDS_TEST_FLAGS=$(FLAGS),--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle

WATCHDOG_TEST_FLAGS=$(FLAGS),--wrap=wdt_init,--wrap=wdt_reset,--wrap=tps382x_init,--wrap=tps382x_trigger

RADIO_TEST_FLAGS=$(FLAGS),--wrap=si446x_reset,--wrap=si446x_part_info,--wrap=si446x_part_info,--wrap=si446x_configuration_init,--wrap=si446x_set_property,--wrap=si446x_set_property,--wrap=si446x_fifo_info,--wrap=si446x_write_tx_fifo,--wrap=si446x_spi_init,--wrap=si446x_get_int_status,--wrap=si446x_start_tx,--wrap=si446x_delay_ms,--wrap=si446x_change_state,--wrap=si446x_read_rx_fifo,--wrap=si446x_start_rx

TTC_TEST_FLAGS=$(FLAGS),--wrap=sl_ttc2_init,--wrap=sl_ttc2_check_device,--wrap=sl_ttc2_write_reg,--wrap=sl_ttc2_read_reg,--wrap=sl_ttc2_read_hk_data,--wrap=sl_ttc2_read_device_id,--wrap=sl_ttc2_read_hardware_version,--wrap=sl_ttc2_read_firmware_version,--wrap=sl_ttc2_read_time_counter,--wrap=sl_ttc2_read_reset_counter,--wrap=sl_ttc2_read_reset_cause,--wrap=sl_ttc2_read_voltage,--wrap=sl_ttc2_read_current,--wrap=sl_ttc2_read_temp,--wrap=sl_ttc2_read_last_valid_tc,--wrap=sl_ttc2_read_rssi,--wrap=sl_ttc2_read_antenna_status,--wrap=sl_ttc2_read_antenna_deployment_status,--wrap=sl_ttc2_read_antenna_deployment_hibernation_status,--wrap=sl_ttc2_read_tx_enable,--wrap=sl_ttc2_set_tx_enable,--wrap=sl_ttc2_read_pkt_counter,--wrap=sl_ttc2_read_fifo_pkts,--wrap=sl_ttc2_read_len_rx_pkt_in_fifo,--wrap=sl_ttc2_check_pkt_avail,--wrap=sl_ttc2_transmit_packet,--wrap=sl_ttc2_read_packet,--wrap=sl_ttc2_delay_ms

EPS_TEST_FLAGS=$(FLAGS),--wrap=sl_eps2_init,--wrap=sl_eps2_check_device,--wrap=sl_eps2_write_reg,--wrap=sl_eps2_read_reg,--wrap=sl_eps2_read_data,--wrap=sl_eps2_read_time_counter,--wrap=sl_eps2_read_temp,--wrap=sl_eps2_read_current,--wrap=sl_eps2_read_reset_cause,--wrap=sl_eps2_read_reset_counter,--wrap=sl_eps2_read_solar_panel_voltage,--wrap=sl_eps2_read_solar_panel_current,--wrap=sl_eps2_read_mppt_duty_cycle,--wrap=sl_eps2_read_main_bus_voltage,--wrap=sl_eps2_read_rtd_temperature,--wrap=sl_eps2_read_battery_voltage,--wrap=sl_eps2_read_battery_current,--wrap=sl_eps2_read_battery_charge,--wrap=sl_eps2_read_battery_monitor_temp,--wrap=sl_eps2_read_battery_monitor_status,--wrap=sl_eps2_read_battery_monitor_protection,--wrap=sl_eps2_read_battery_monitor_cycle_counter,--wrap=sl_eps2_read_battery_monitor_raac,--wrap=sl_eps2_read_battery_monitor_rsac,--wrap=sl_eps2_read_battery_monitor_rarc,--wrap=sl_eps2_read_battery_monitor_rsrc,--wrap=sl_eps2_read_heater_duty_cycle,--wrap=sl_eps2_read_hardware_version,--wrap=sl_eps2_read_firmware_version,--wrap=sl_eps2_set_mppt_mode,--wrap=sl_eps2_get_mppt_mode,--wrap=sl_eps2_set_heater_mode,--wrap=sl_eps2_get_heater_mode,--wrap=sl_eps2_delay_ms

ANTENNA_TEST_FLAGS=$(FLAGS),--wrap=isis_antenna_init,--wrap=isis_antenna_arm,--wrap=isis_antenna_disarm,--wrap=isis_antenna_start_sequential_deploy,--wrap=isis_antenna_start_independent_deploy,--wrap=isis_antenna_read_deployment_status_code,--wrap=isis_antenna_read_deployment_status,--wrap=isis_antenna_get_data,--wrap=isis_antenna_get_antenna_status,--wrap=isis_antenna_get_antenna_timeout,--wrap=isis_antenna_get_burning,--wrap=isis_antenna_get_arming_status,--wrap=isis_antenna_get_raw_temperature,--wrap=isis_antenna_raw_to_temp_c,--wrap=isis_antenna_get_temperature_c,--wrap=isis_antenna_get_temperature_k,--wrap=isis_antenna_delay_s,--wrap=isis_antenna_delay_ms

MEDIA_TEST_FLAGS=$(FLAGS),--wrap=flash_init,--wrap=flash_write,--wrap=flash_write_single,--wrap=flash_read_single,--wrap=flash_write_long,--wrap=flash_read_long,--wrap=flash_erase,--wrap=mt25q_init,--wrap=mt25q_reset,--wrap=mt25q_read_device_id,--wrap=mt25q_read_flash_description,--wrap=mt25q_clear_flag_status_register,--wrap=mt25q_read_status,--wrap=mt25q_enter_deep_power_down,--wrap=mt25q_release_from_deep_power_down,--wrap=mt25q_write_enable,--wrap=mt25q_write_disable,--wrap=mt25q_is_busy,--wrap=mt25q_die_erase,--wrap=mt25q_sector_erase,--wrap=mt25q_sub_sector_erase,--wrap=mt25q_write,--wrap=mt25q_read,--wrap=mt25q_get_max_address,--wrap=mt25q_enter_4_byte_address_mode,--wrap=mt25q_read_flag_status_register,--wrap=mt25q_get_flash_description,--wrap=mt25q_spi_init,--wrap=mt25q_spi_write,--wrap=mt25q_spi_read,--wrap=mt25q_spi_transfer,--wrap=mt25q_spi_select,--wrap=mt25q_spi_unselect,--wrap=mt25q_spi_write_only,--wrap=mt25q_spi_read_only,--wrap=mt25q_spi_transfer_only,--wrap=mt25q_gpio_init,--wrap=mt25q_gpio_set_hold,--wrap=mt25q_gpio_set_reset,--wrap=mt25q_delay_ms,--wrap=cy15x102qn_init,--wrap=cy15x102qn_set_write_enable,--wrap=cy15x102qn_reset_write_enable,--wrap=cy15x102qn_read_status_reg,--wrap=cy15x102qn_write_status_reg,--wrap=cy15x102qn_write,--wrap=cy15x102qn_read,--wrap=cy15x102qn_fast_read,--wrap=cy15x102qn_special_sector_write,--wrap=cy15x102qn_special_sector_read,--wrap=cy15x102qn_read_device_id,--wrap=cy15x102qn_read_unique_id,--wrap=cy15x102qn_write_serial_number,--wrap=cy15x102qn_read_serial_number,--wrap=cy15x102qn_deep_power_down_mode,--wrap=cy15x102qn_hibernate_mode,--wrap=cy15x102qn_spi_init,--wrap=cy15x102qn_spi_write,--wrap=cy15x102qn_spi_read,--wrap=cy15x102qn_spi_transfer,--wrap=cy15x102qn_spi_select,--wrap=cy15x102qn_spi_unselect,--wrap=cy15x102qn_spi_write_only,--wrap=cy15x102qn_spi_read_only,--wrap=cy15x102qn_spi_transfer_only,--wrap=cy15x102qn_gpio_init,--wrap=cy15x102qn_gpio_set_write_protect,--wrap=cy15x102qn_gpio_clear_write_protect

PAYLOAD_TEST_FLAGS=$(FLAGS),--wrap=edc_init,--wrap=edc_enable,--wrap=edc_disable,--wrap=edc_write_cmd,--wrap=edc_read,--wrap=edc_check_device,--wrap=edc_set_rtc_time,--wrap=edc_pop_ptt_pkg,--wrap=edc_pause_ptt_task,--wrap=edc_resume_ptt_task,--wrap=edc_start_adc_task,--wrap=edc_get_state_pkg,--wrap=edc_get_ptt_pkg,--wrap=edc_get_hk_pkg,--wrap=edc_get_adc_seq,--wrap=edc_echo,--wrap=edc_calc_checksum,--wrap=edc_get_state,--wrap=edc_get_ptt,--wrap=edc_get_hk,--wrap=edc_delay_ms,--wrap=phj_init_i2c,--wrap=phj_init_gpio,--wrap=phj_read,--wrap=phj_check_converter,--wrap=phj_check_message

.PHONY: all
all: current_sensor_test voltage_sensor_test temp_sensor_test leds_test watchdog_test radio_test ttc_test eps_test antenna_test media_test payload_test

.PHONY: current_sensor_test
current_sensor_test: $(BUILD_DIR)/current_sensor.o $(BUILD_DIR)/current_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o
	$(CC) $(CURRENT_SENSOR_TEST_FLAGS) $(BUILD_DIR)/current_sensor.o $(BUILD_DIR)/current_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o -o $(BUILD_DIR)/$(TARGET_CURRENT_SENSOR) -lcmocka

.PHONY: voltage_sensor_test
voltage_sensor_test: $(BUILD_DIR)/voltage_sensor.o $(BUILD_DIR)/voltage_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o
	$(CC) $(VOLTAGE_SENSOR_TEST_FLAGS) $(BUILD_DIR)/voltage_sensor.o $(BUILD_DIR)/voltage_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o -o $(BUILD_DIR)/$(TARGET_VOLTAGE_SENSOR) -lcmocka

.PHONY: temp_sensor_test
temp_sensor_test: $(BUILD_DIR)/temp_sensor.o $(BUILD_DIR)/temp_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o
	$(CC) $(TEMP_SENSOR_TEST_FLAGS) $(BUILD_DIR)/temp_sensor.o $(BUILD_DIR)/temp_sensor_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/adc_wrap.o -o $(BUILD_DIR)/$(TARGET_TEMP_SENSOR) -lcmocka

.PHONY: leds_test
leds_test: $(BUILD_DIR)/leds.o $(BUILD_DIR)/leds_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o
	$(CC) $(LEDS_TEST_FLAGS) $(BUILD_DIR)/leds.o $(BUILD_DIR)/leds_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o -o $(BUILD_DIR)/$(TARGET_LEDS) -lcmocka

.PHONY: watchdog_test
watchdog_test: $(BUILD_DIR)/watchdog.o $(BUILD_DIR)/watchdog_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/wdt_wrap.o $(BUILD_DIR)/tps382x_wrap.o
	$(CC) $(WATCHDOG_TEST_FLAGS) $(BUILD_DIR)/watchdog.o $(BUILD_DIR)/watchdog_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/wdt_wrap.o $(BUILD_DIR)/tps382x_wrap.o -o $(BUILD_DIR)/$(TARGET_WATCHDOG) -lcmocka

.PHONY: radio_test
radio_test: $(BUILD_DIR)/radio.o $(BUILD_DIR)/radio_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/si446x_wrap.o $(BUILD_DIR)/task.o
	$(CC) $(RADIO_TEST_FLAGS) $(BUILD_DIR)/radio.o $(BUILD_DIR)/radio_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/si446x_wrap.o $(BUILD_DIR)/task.o -o $(BUILD_DIR)/$(TARGET_RADIO) -lm -lcmocka

.PHONY: ttc_test
ttc_test: $(BUILD_DIR)/ttc.o $(BUILD_DIR)/ttc_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/sl_ttc2_wrap.o
	$(CC) $(TTC_TEST_FLAGS) $(BUILD_DIR)/ttc.o $(BUILD_DIR)/ttc_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/sl_ttc2_wrap.o -o $(BUILD_DIR)/$(TARGET_TTC) -lm -lcmocka

.PHONY: eps_test
eps_test: $(BUILD_DIR)/eps.o $(BUILD_DIR)/eps_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/sl_eps2_wrap.o
	$(CC) $(EPS_TEST_FLAGS) $(BUILD_DIR)/eps.o $(BUILD_DIR)/eps_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/sl_eps2_wrap.o -o $(BUILD_DIR)/$(TARGET_EPS) -lm -lcmocka

.PHONY: antenna_test
antenna_test: $(BUILD_DIR)/antenna.o $(BUILD_DIR)/antenna_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/isis_antenna_wrap.o
	$(CC) $(ANTENNA_TEST_FLAGS) $(BUILD_DIR)/antenna.o $(BUILD_DIR)/antenna_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/isis_antenna_wrap.o -o $(BUILD_DIR)/$(TARGET_ANTENNA) -lm -lcmocka

.PHONY: media_test
media_test: $(BUILD_DIR)/media.o $(BUILD_DIR)/media_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/flash_wrap.o $(BUILD_DIR)/mt25q_wrap.o $(BUILD_DIR)/cy15x102qn_wrap.o
	$(CC) $(MEDIA_TEST_FLAGS) $(BUILD_DIR)/media.o $(BUILD_DIR)/media_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/flash_wrap.o $(BUILD_DIR)/mt25q_wrap.o $(BUILD_DIR)/cy15x102qn_wrap.o -o $(BUILD_DIR)/$(TARGET_MEDIA) -lm -lcmocka

.PHONY: payload_test
payload_test: $(BUILD_DIR)/payload.o $(BUILD_DIR)/payload_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/system_wrap.o $(BUILD_DIR)/edc_wrap.o $(BUILD_DIR)/phj_wrap.o
	$(CC) $(PAYLOAD_TEST_FLAGS) $(BUILD_DIR)/payload.o $(BUILD_DIR)/payload_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/system_wrap.o $(BUILD_DIR)/edc_wrap.o $(BUILD_DIR)/phj_wrap.o -o $(BUILD_DIR)/$(TARGET_PAYLOAD) -lm -lcmocka

# Devices
$(BUILD_DIR)/current_sensor.o: ../../devices/current_sensor/current_sensor.c
	$(CC) $(CURRENT_SENSOR_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/voltage_sensor.o: ../../devices/voltage_sensor/voltage_sensor.c
	$(CC) $(VOLTAGE_SENSOR_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/temp_sensor.o: ../../devices/temp_sensor/temp_sensor.c
	$(CC) $(TEMP_SENSOR_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/leds.o: ../../devices/leds/leds.c
	$(CC) $(LEDS_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/watchdog.o: ../../devices/watchdog/watchdog.c
	$(CC) $(WATCHDOG_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/radio.o: ../../devices/radio/radio.c
	$(CC) $(RADIO_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/ttc.o: ../../devices/ttc/ttc.c
	$(CC) $(TTC_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/eps.o: ../../devices/eps/eps.c
	$(CC) $(EPS_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/antenna.o: ../../devices/antenna/antenna.c
	$(CC) $(ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/media.o: ../../devices/media/media.c
	$(CC) $(MEDIA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/payload.o: ../../devices/payload/payload.c
	$(CC) $(PAYLOAD_TEST_FLAGS) -c $< -o $@

# Tests
$(BUILD_DIR)/current_sensor_test.o: current_sensor_test.c
	$(CC) $(CURRENT_SENSOR_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/voltage_sensor_test.o: voltage_sensor_test.c
	$(CC) $(VOLTAGE_SENSOR_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/temp_sensor_test.o: temp_sensor_test.c
	$(CC) $(TEMP_SENSOR_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/leds_test.o: leds_test.c
	$(CC) $(LEDS_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/watchdog_test.o: watchdog_test.c
	$(CC) $(WATCHDOG_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/radio_test.o: radio_test.c
	$(CC) $(RADIO_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/ttc_test.o: ttc_test.c
	$(CC) $(TTC_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/eps_test.o: eps_test.c
	$(CC) $(EPS_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/antenna_test.o: antenna_test.c
	$(CC) $(ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/media_test.o: media_test.c
	$(CC) $(MEDIA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/payload_test.o: payload_test.c
	$(CC) $(PAYLOAD_TEST_FLAGS) -c $< -o $@

# Mockups
$(BUILD_DIR)/sys_log_wrap.o: ../mockups/system/sys_log_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/system_wrap.o: ../mockups/system/system_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/adc_wrap.o: ../mockups/drivers/adc_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/gpio_wrap.o: ../mockups/drivers/gpio_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/wdt_wrap.o: ../mockups/drivers/wdt_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/tps382x_wrap.o: ../mockups/drivers/tps382x_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x_wrap.o: ../mockups/drivers/si446x_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/sl_ttc2_wrap.o: ../mockups/drivers/sl_ttc2_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/sl_eps2_wrap.o: ../mockups/drivers/sl_eps2_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/isis_antenna_wrap.o: ../mockups/drivers/isis_antenna_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/flash_wrap.o: ../mockups/drivers/flash_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/mt25q_wrap.o: ../mockups/drivers/mt25q_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/cy15x102qn_wrap.o: ../mockups/drivers/cy15x102qn_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/edc_wrap.o: ../mockups/drivers/edc_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/phj_wrap.o: ../mockups/drivers/phj_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/task.o: ../freertos_sim/task.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET_CURRENT_SENSOR) $(BUILD_DIR)/$(TARGET_VOLTAGE_SENSOR) $(BUILD_DIR)/$(TARGET_TEMP_SENSOR) $(BUILD_DIR)/$(TARGET_LEDS) $(BUILD_DIR)/$(TARGET_WATCHDOG) $(BUILD_DIR)/$(TARGET_RADIO) $(BUILD_DIR)/$(TARGET_TTC) $(BUILD_DIR)/$(TARGET_EPS) $(BUILD_DIR)/$(TARGET_ANTENNA) $(BUILD_DIR)/$(TARGET_MEDIA) $(BUILD_DIR)/$(TARGET_PAYLOAD) $(BUILD_DIR)/*.o
