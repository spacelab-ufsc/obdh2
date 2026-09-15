/*
 * ro_test.c
 * Copyright The OBDH 2.0 Contributors.
 *
 * Unit tests of the Radio Occultation payload driver.
 */

/** \defgroup ro_unit_test RO \ingroup tests \{ */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>
#include <math.h>

#include <drivers/i2c/i2c.h>
#include <drivers/gpio/gpio.h>
#include <drivers/ro/ro.h>



void ro_delay_ms(uint32_t ms) { (void)ms; }
static void test_checksum_and_invalid_command(void **state)
{
    uint8_t data[] = { 0x11U, 0x22U, 0x33U };
    ro_config_t config = {0};
    (void)state;
    assert_int_equal(ro_calc_checksum(data, sizeof(data)), 0U);
    assert_int_equal(ro_write_cmd(config, (ro_cmd_t){ 0x99U, 0U }), -1);
}
static void test_state_parser_rejects_null_output(void **state)
{
    ro_config_t config = {0};
    (void)state;
    assert_int_equal(ro_get_state(config, NULL), -1);
}
int main(void)
{
    const struct CMUnitTest tests[] = { cmocka_unit_test(test_checksum_and_invalid_command), cmocka_unit_test(test_state_parser_rejects_null_output) };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

/** \} End of ro_unit_test group */
