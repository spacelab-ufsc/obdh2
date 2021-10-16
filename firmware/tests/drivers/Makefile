TARGET_CY15X102QN=cy15x102qn_unit_test
TARGET_TPS382X=tps382x_unit_test
TARGET_TCA4311A=tca4311a_unit_test
TARGET_EDC=edc_unit_test
TARGET_ISIS_ANTENNA=isis_antenna_unit_test
TARGET_SL_EPS2=sl_eps2_unit_test
TARGET_SI446X=si446x_unit_test
TARGET_MT25Q=mt25q_unit_test
TARGET_SL_TTC2=sl_ttc2_unit_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../../
FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC) -I../../tests/freertos_sim/ -Wl,--wrap=sys_log_init,--wrap=sys_log_print_event,--wrap=sys_log_print_event_from_module,--wrap=sys_log_print_msg,--wrap=sys_log_print_str,--wrap=sys_log_new_line,--wrap=sys_log_print_uint,--wrap=sys_log_print_int,--wrap=sys_log_print_hex,--wrap=sys_log_dump_hex,--wrap=sys_log_print_float,--wrap=sys_log_print_byte,--wrap=sys_log_print_system_time,--wrap=sys_log_print_license_msg,--wrap=sys_log_print_splash_screen,--wrap=sys_log_print_firmware_version

CY15X102QN_TEST_FLAGS=$(FLAGS),--wrap=spi_init,--wrap=spi_select_slave,--wrap=spi_write,--wrap=spi_read,--wrap=spi_transfer,--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle

TPS382X_TEST_FLAGS=$(FLAGS),--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle

TCA4311A_TEST_FLAGS=$(FLAGS),--wrap=i2c_init,--wrap=i2c_write,--wrap=i2c_read,--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle

EDC_TEST_FLAGS=$(FLAGS),--wrap=i2c_init,--wrap=i2c_write,--wrap=i2c_read

ISIS_ANTENNA_TEST_FLAGS=$(FLAGS),--wrap=tca4311a_init,--wrap=tca4311a_enable,--wrap=tca4311a_disable,--wrap=tca4311a_is_ready,--wrap=tca4311a_write,--wrap=tca4311a_read,--wrap=tca4311a_write_byte,--wrap=tca4311a_read_byte

SL_EPS2_TEST_FLAGS=$(FLAGS),--wrap=tca4311a_init,--wrap=tca4311a_enable,--wrap=tca4311a_disable,--wrap=tca4311a_is_ready,--wrap=tca4311a_write,--wrap=tca4311a_read,--wrap=tca4311a_write_byte,--wrap=tca4311a_read_byte

SI446X_TEST_FLAGS=$(FLAGS),--wrap=spi_init,--wrap=spi_select_slave,--wrap=spi_write,--wrap=spi_read,--wrap=spi_transfer,--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle

MT25Q_TEST_FLAGS=$(FLAGS),--wrap=spi_init,--wrap=spi_select_slave,--wrap=spi_write,--wrap=spi_read,--wrap=spi_transfer,--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle

SL_TTC2_TEST_FLAGS=$(FLAGS),--wrap=spi_init,--wrap=spi_select_slave,--wrap=spi_write,--wrap=spi_read,--wrap=spi_transfer,--wrap=gpio_init,--wrap=gpio_set_state,--wrap=gpio_get_state,--wrap=gpio_toggle

.PHONY: all
.PHONY: all
all: cy15x102qn_test tps382x_test tca4311a_test edc_test isis_antenna_test sl_eps2_test si446x_test mt25q_test sl_ttc2_test

.PHONY: cy15x102qn_test
cy15x102qn_test: $(BUILD_DIR)/cy15x102qn.o $(BUILD_DIR)/cy15x102qn_gpio.o $(BUILD_DIR)/cy15x102qn_spi.o $(BUILD_DIR)/cy15x102qn_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/spi_wrap.o $(BUILD_DIR)/gpio_wrap.o
	$(CC) $(CY15X102QN_TEST_FLAGS) $(BUILD_DIR)/cy15x102qn.o $(BUILD_DIR)/cy15x102qn_gpio.o $(BUILD_DIR)/cy15x102qn_spi.o $(BUILD_DIR)/cy15x102qn_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/spi_wrap.o $(BUILD_DIR)/gpio_wrap.o -o $(BUILD_DIR)/$(TARGET_CY15X102QN) -lcmocka

.PHONY: tps382x_test
tps382x_test: $(BUILD_DIR)/tps382x.o $(BUILD_DIR)/tps382x_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o
	$(CC) $(TPS382X_TEST_FLAGS) $(BUILD_DIR)/tps382x.o $(BUILD_DIR)/tps382x_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/gpio_wrap.o -o $(BUILD_DIR)/$(TARGET_TPS382X) -lcmocka

.PHONY: tca4311a_test
tca4311a_test: $(BUILD_DIR)/tca4311a.o $(BUILD_DIR)/tca4311a_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/i2c_wrap.o $(BUILD_DIR)/gpio_wrap.o
	$(CC) $(TCA4311A_TEST_FLAGS) $(BUILD_DIR)/tca4311a.o $(BUILD_DIR)/tca4311a_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/i2c_wrap.o $(BUILD_DIR)/gpio_wrap.o -o $(BUILD_DIR)/$(TARGET_TCA4311A) -lm -lcmocka

.PHONY: edc_test
edc_test: $(BUILD_DIR)/edc.o $(BUILD_DIR)/edc_delay.o $(BUILD_DIR)/edc_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/i2c_wrap.o $(BUILD_DIR)/task.o
	$(CC) $(EDC_TEST_FLAGS) $(BUILD_DIR)/edc.o $(BUILD_DIR)/edc_delay.o $(BUILD_DIR)/edc_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/i2c_wrap.o $(BUILD_DIR)/task.o -o $(BUILD_DIR)/$(TARGET_EDC) -lm -lcmocka

.PHONY: isis_antenna_test
isis_antenna_test: $(BUILD_DIR)/isis_antenna.o $(BUILD_DIR)/isis_antenna_delay.o $(BUILD_DIR)/isis_antenna_i2c.o $(BUILD_DIR)/isis_antenna_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/tca4311a_wrap.o $(BUILD_DIR)/task.o
	$(CC) $(ISIS_ANTENNA_TEST_FLAGS) $(BUILD_DIR)/isis_antenna.o $(BUILD_DIR)/isis_antenna_delay.o $(BUILD_DIR)/isis_antenna_i2c.o $(BUILD_DIR)/isis_antenna_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/tca4311a_wrap.o $(BUILD_DIR)/task.o -o $(BUILD_DIR)/$(TARGET_ISIS_ANTENNA) -lm -lcmocka

.PHONY: sl_eps2_test
sl_eps2_test: $(BUILD_DIR)/sl_eps2.o $(BUILD_DIR)/sl_eps2_i2c.o $(BUILD_DIR)/sl_eps2_delay.o $(BUILD_DIR)/sl_eps2_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/tca4311a_wrap.o $(BUILD_DIR)/task.o
	$(CC) $(SL_EPS2_TEST_FLAGS) $(BUILD_DIR)/sl_eps2.o $(BUILD_DIR)/sl_eps2_i2c.o $(BUILD_DIR)/sl_eps2_delay.o $(BUILD_DIR)/sl_eps2_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/tca4311a_wrap.o $(BUILD_DIR)/task.o -o $(BUILD_DIR)/$(TARGET_SL_EPS2) -lm -lcmocka

.PHONY: si446x_test
si446x_test: $(BUILD_DIR)/si446x.o $(BUILD_DIR)/si446x_delay.o $(BUILD_DIR)/si446x_gpio.o $(BUILD_DIR)/si446x_spi.o $(BUILD_DIR)/si446x_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/spi_wrap.o $(BUILD_DIR)/gpio_wrap.o $(BUILD_DIR)/task.o
	$(CC) $(SI446X_TEST_FLAGS) $(BUILD_DIR)/si446x.o $(BUILD_DIR)/si446x_delay.o $(BUILD_DIR)/si446x_gpio.o $(BUILD_DIR)/si446x_spi.o $(BUILD_DIR)/si446x_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/spi_wrap.o $(BUILD_DIR)/gpio_wrap.o $(BUILD_DIR)/task.o -o $(BUILD_DIR)/$(TARGET_SI446X) -lm -lcmocka

.PHONY: mt25q_test
mt25q_test: $(BUILD_DIR)/mt25q.o $(BUILD_DIR)/mt25q_delay.o $(BUILD_DIR)/mt25q_gpio.o $(BUILD_DIR)/mt25q_spi.o $(BUILD_DIR)/mt25q_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/spi_wrap.o $(BUILD_DIR)/gpio_wrap.o $(BUILD_DIR)/task.o
	$(CC) $(MT25Q_TEST_FLAGS) $(BUILD_DIR)/mt25q.o $(BUILD_DIR)/mt25q_delay.o $(BUILD_DIR)/mt25q_gpio.o $(BUILD_DIR)/mt25q_spi.o $(BUILD_DIR)/mt25q_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/spi_wrap.o $(BUILD_DIR)/gpio_wrap.o $(BUILD_DIR)/task.o -o $(BUILD_DIR)/$(TARGET_MT25Q) -lm -lcmocka

.PHONY: sl_ttc2_test
sl_ttc2_test: $(BUILD_DIR)/sl_ttc2.o $(BUILD_DIR)/sl_ttc2_spi.o $(BUILD_DIR)/sl_ttc2_delay.o $(BUILD_DIR)/sl_ttc2_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/spi_wrap.o $(BUILD_DIR)/gpio_wrap.o $(BUILD_DIR)/task.o
	$(CC) $(SL_TTC2_TEST_FLAGS) $(BUILD_DIR)/sl_ttc2.o $(BUILD_DIR)/sl_ttc2_spi.o $(BUILD_DIR)/sl_ttc2_delay.o $(BUILD_DIR)/sl_ttc2_test.o $(BUILD_DIR)/sys_log_wrap.o $(BUILD_DIR)/spi_wrap.o $(BUILD_DIR)/gpio_wrap.o $(BUILD_DIR)/task.o -o $(BUILD_DIR)/$(TARGET_SL_TTC2) -lm -lcmocka

# Drivers
$(BUILD_DIR)/cy15x102qn.o: ../../drivers/cy15x102qn/cy15x102qn.c
	$(CC) $(CY15X102QN_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/cy15x102qn_gpio.o: ../../drivers/cy15x102qn/cy15x102qn_gpio.c
	$(CC) $(CY15X102QN_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/cy15x102qn_spi.o: ../../drivers/cy15x102qn/cy15x102qn_spi.c
	$(CC) $(CY15X102QN_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/tps382x.o: ../../drivers/tps382x/tps382x.c
	$(CC) $(TPS382X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/tca4311a.o: ../../drivers/tca4311a/tca4311a.c
	$(CC) $(TCA4311A_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/edc.o: ../../drivers/edc/edc.c
	$(CC) $(EDC_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/edc_delay.o: ../../drivers/edc/edc_delay.c
	$(CC) $(EDC_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/isis_antenna.o: ../../drivers/isis_antenna/isis_antenna.c
	$(CC) $(ISIS_ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/isis_antenna_delay.o: ../../drivers/isis_antenna/isis_antenna_delay.c
	$(CC) $(ISIS_ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/isis_antenna_i2c.o: ../../drivers/isis_antenna/isis_antenna_i2c.c
	$(CC) $(ISIS_ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/sl_eps2.o: ../../drivers/sl_eps2/sl_eps2.c
	$(CC) $(SL_EPS2_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/sl_eps2_i2c.o: ../../drivers/sl_eps2/sl_eps2_i2c.c
	$(CC) $(SL_EPS2_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/sl_eps2_delay.o: ../../drivers/sl_eps2/sl_eps2_delay.c
	$(CC) $(SL_EPS2_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x.o: ../../drivers/si446x/si446x.c
	$(CC) $(SI446X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x_delay.o: ../../drivers/si446x/si446x_delay.c
	$(CC) $(SI446X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x_gpio.o: ../../drivers/si446x/si446x_gpio.c
	$(CC) $(SI446X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x_spi.o: ../../drivers/si446x/si446x_spi.c
	$(CC) $(SI446X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/mt25q.o: ../../drivers/mt25q/mt25q.c
	$(CC) $(MT25Q_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/mt25q_delay.o: ../../drivers/mt25q/mt25q_delay.c
	$(CC) $(MT25Q_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/mt25q_gpio.o: ../../drivers/mt25q/mt25q_gpio.c
	$(CC) $(MT25Q_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/mt25q_spi.o: ../../drivers/mt25q/mt25q_spi.c
	$(CC) $(MT25Q_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/sl_ttc2.o: ../../drivers/sl_ttc2/sl_ttc2.c
	$(CC) $(SL_TTC2_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/sl_ttc2_spi.o: ../../drivers/sl_ttc2/sl_ttc2_spi.c
	$(CC) $(SL_TTC2_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/sl_ttc2_delay.o: ../../drivers/sl_ttc2/sl_ttc2_delay.c
	$(CC) $(SL_TTC2_TEST_FLAGS) -c $< -o $@

# Tests
$(BUILD_DIR)/cy15x102qn_test.o: cy15x102qn_test.c
	$(CC) $(CY15X102QN_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/tps382x_test.o: tps382x_test.c
	$(CC) $(TPS382X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/tca4311a_test.o: tca4311a_test.c
	$(CC) $(TCA4311A_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/edc_test.o: edc_test.c
	$(CC) $(EDC_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/isis_antenna_test.o: isis_antenna_test.c
	$(CC) $(ISIS_ANTENNA_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/sl_eps2_test.o: sl_eps2_test.c
	$(CC) $(SL_EPS2_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/si446x_test.o: si446x_test.c
	$(CC) $(SI446X_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/mt25q_test.o: mt25q_test.c
	$(CC) $(MT25Q_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/sl_ttc2_test.o: sl_ttc2_test.c
	$(CC) $(SL_TTC2_TEST_FLAGS) -c $< -o $@

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

$(BUILD_DIR)/i2c_wrap.o: ../mockups/drivers/i2c_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/tca4311a_wrap.o: ../mockups/drivers/tca4311a_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/task.o: ../freertos_sim/task.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET_CY15X102QN) $(BUILD_DIR)/$(TARGET_TPS382X) $(BUILD_DIR)/$(TARGET_TCA4311A) $(BUILD_DIR)/$(TARGET_EDC) $(BUILD_DIR)/$(TARGET_ISIS_ANTENNA) $(BUILD_DIR)/$(TARGET_SL_EPS2) $(BUILD_DIR)/$(TARGET_SI446X) $(BUILD_DIR)/$(TARGET_MT25Q) $(BUILD_DIR)/$(TARGET_SL_TTC2) $(BUILD_DIR)/*.o
