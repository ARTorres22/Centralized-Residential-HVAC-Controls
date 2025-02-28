/* generated HAL source file - do not edit */
#include "hal_data.h"
dtc_instance_ctrl_t g_transfer_adc0_group_ctrl;

#if (1 == 1)
transfer_info_t g_transfer_adc0_group_info DTC_TRANSFER_INFO_ALIGNMENT =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_2_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_REPEAT,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 64, };

#elif (1 > 1)
/* User is responsible to initialize the array. */
transfer_info_t g_transfer_adc0_group_info[1] DTC_TRANSFER_INFO_ALIGNMENT;
#else
/* User must call api::reconfigure before enable DTC transfer. */
#endif

const dtc_extended_cfg_t g_transfer_adc0_group_cfg_extend =
{ .activation_source = VECTOR_NUMBER_ADC0_SCAN_END, };

const transfer_cfg_t g_transfer_adc0_group_cfg =
{
#if (1 == 1)
  .p_info = &g_transfer_adc0_group_info,
#elif (1 > 1)
    .p_info              = g_transfer_adc0_group_info,
#else
    .p_info = NULL,
#endif
  .p_extend = &g_transfer_adc0_group_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer_adc0_group =
{ .p_ctrl = &g_transfer_adc0_group_ctrl, .p_cfg = &g_transfer_adc0_group_cfg, .p_api = &g_transfer_on_dtc };
tau_instance_ctrl_t g_timer0_ctrl;
const tau_extended_cfg_t g_timer0_extend =
{ .opirq = TAU_INTERRUPT_OPIRQ_BIT_RESET,
  .tau_func = TAU_FUNCTION_INTERVAL,
  .bit_mode = TAU_BIT_MODE_16BIT,
  .initial_output = TAU_PIN_OUTPUT_CFG_DISABLED,
  .input_source = TAU_INPUT_SOURCE_NONE,
  .tau_filter = TAU_INPUT_NOISE_FILTER_DISABLE,
  .trigger_edge = TAU_TRIGGER_EDGE_RISING,
  .operation_clock = TAU_OPERATION_CK00,
  /* Not used for 16-bit or lower 8-bit mode */
  .period_higher_8bit_counts = (uint16_t) 0x100,
  .higher_8bit_cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_TAU0_TMI00H)
    .higher_8bit_cycle_end_irq       = VECTOR_NUMBER_TAU0_TMI00H,
#else
  .higher_8bit_cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
const timer_cfg_t g_timer0_cfg =
{ .mode = (timer_mode_t) 0,
/* The current period count(0xa4100) is exceeding the maximum limit; the actual value is assigned to the maximum value.*/
/* Actual Period: 0.0020480000 seconds. */
/* Minimum Period ~ Maximum Period: 0.0000000625 ~ 0.00204800 seconds. */.period_counts = (uint32_t) 0x10000,
  .duty_cycle_counts = 0, .source_div = (timer_source_div_t) BSP_CFG_TAU_CK00, .channel = 0, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_timer0_extend,
  .cycle_end_ipl = (2),
#if defined(VECTOR_NUMBER_TAU0_TMI00)
    .cycle_end_irq       = VECTOR_NUMBER_TAU0_TMI00,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer0 =
{ .p_ctrl = &g_timer0_ctrl, .p_cfg = &g_timer0_cfg, .p_api = &g_timer_on_tau };
adc_d_instance_ctrl_t g_adc0_ctrl;

/* ADC conversion time 9.1875 us, conversion start time: 1.0625us */
const adc_d_extended_cfg_t g_adc0_cfg_extend =
{ .channel_mode = ADC_D_CHANNEL_MODE_SCAN, .operation_voltage = ADC_D_VOLTAGE_MODE_NORMAL_1, .conversion_clockdiv =
          ADC_D_CLOCK_DIV_1,
  .trigger_source = ADC_D_TRIGGER_SOURCE_ELC, .operation_trigger = ADC_D_TRIGGER_MODE_WAIT, .conversion_operation =
          ADC_D_CONVERSION_MODE_ONESHOT,
  .upper_lower_bound = ADC_D_BOUNDARY_IN_RANGE, .negative_vref = ADC_D_NEGATIVE_VREF_VSS, .positive_vref =
          ADC_D_POSITIVE_VREF_VCC,
  .upper_bound_limit = 255, .lower_bound_limit = 0, };
const adc_cfg_t g_adc0_cfg =
{ .unit = 0, .mode = (adc_mode_t) 0, // Unused
  .resolution = ADC_RESOLUTION_12_BIT,
  .alignment = (adc_alignment_t) 0, // Unused
  .trigger = (adc_trigger_t) 0, // Unused
  .p_callback = g_adc0_callback,
  .p_context = NULL,
  .p_extend = &g_adc0_cfg_extend,
#if defined(VECTOR_NUMBER_ADC0_SCAN_END)
    .scan_end_irq        = VECTOR_NUMBER_ADC0_SCAN_END,
#else
  .scan_end_irq = FSP_INVALID_VECTOR,
#endif
  .scan_end_ipl = (2),
  .scan_end_b_irq = FSP_INVALID_VECTOR,
  .scan_end_b_ipl = BSP_IRQ_DISABLED, };

const adc_d_channel_cfg_t g_adc0_channel_cfg =
{ .channel_input = ADC_CHANNEL_0 };
/* Instance structure to use this module. */
const adc_instance_t g_adc0 =
{ .p_ctrl = &g_adc0_ctrl, .p_cfg = &g_adc0_cfg, .p_channel_cfg = &g_adc0_channel_cfg, .p_api = &g_adc_on_adc_d };
iica_master_instance_ctrl_t g_iica_master0_ctrl;
const iica_master_extended_cfg_t g_iica_master0_extend =
{ .clock_settings.comm_rez = IICA_MASTER_COMM_REZ_DISABLED,
  .clock_settings.digital_filter = 0,
  /* Actual calculated bitrate: 99379. IICWH0 duty cycle: 53. IICWL0 duty cycle: 47. */.clock_settings.iicwl_value = 76,
  .clock_settings.iicwh_value = 85,
  .clock_settings.cks_value = 1,
  .sda_pin_settings.pin = BSP_IO_PORT_09_PIN_13,
  .sda_pin_settings.cfg = (uint32_t) (IOPORT_PERIPHERAL_IICA1),
  .scl_pin_settings.pin = BSP_IO_PORT_09_PIN_14,
  .scl_pin_settings.cfg = (uint32_t) (IOPORT_PERIPHERAL_IICA1), };
const i2c_master_cfg_t g_iica_master0_cfg =
{ .channel = 0, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT, .p_callback =
          i2c_master_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_IICA0_TXRXI)
    .tei_irq             = VECTOR_NUMBER_IICA0_TXRXI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (2),
  .p_extend = &g_iica_master0_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_iica_master0 =
{ .p_ctrl = &g_iica_master0_ctrl, .p_cfg = &g_iica_master0_cfg, .p_api = &g_iica_master_on_iica };
uarta_instance_ctrl_t g_uart0_ctrl;

uarta_baud_setting_t g_uart0_baud_setting =
{
#if (BSP_CFG_UARTA0_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC)

  /* Baud rate calculated with Acutal_Error 1.36%. */
  /* The permissible baud rate error range during reception: -4.35% ~ 4.76% */
  .utanck_clock_b.utasel = UARTA_CLOCK_SOURCE_MOSC,
  .utanck_clock_b.utanck = UARTA_CLOCK_DIV_8, .brgca = 11, .delay_time = 1
#elif (BSP_CFG_UARTA0_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_HOCO)

/* Baud rate calculated with Acutal_Error 2.12%. */
/* The permissible baud rate error range during reception: -4.49% ~ 4.94% */
  .utanck_clock_b.utasel = UARTA_CLOCK_SOURCE_HOCO
, .utanck_clock_b.utanck = UARTA_CLOCK_DIV_8
, .brgca = 17
, .delay_time = 1
 #elif (BSP_CFG_UARTA0_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MOCO)

/* Baud rate calculated with Acutal_Error 2.12%. */ 
/* The permissible baud rate error range during reception: -4.49% ~ 4.94% */
  .utanck_clock_b.utasel = UARTA_CLOCK_SOURCE_MOCO
, .utanck_clock_b.utanck = UARTA_CLOCK_DIV_1
, .brgca = 17
, .delay_time = 1
 #elif ((BSP_CFG_UARTA0_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_FSXP) || (BSP_CFG_UARTA0_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_LOCO))

/* Baud rate calculated with Acutal_Error 100%. */
/* The permissible baud rate error range during reception: Invalid Range Error */
  .utanck_clock_b.utasel = 0
, .utanck_clock_b.utanck = 0
, .brgca = 0
, .delay_time = 31
 #elif (BSP_CFG_UARTA0_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_SUBCLOCK)

/* Baud rate calculated with Acutal_Error 100%. */
/* The permissible baud rate error range during reception: Invalid Range Error */
  .utanck_clock_b.utasel = 0
, .utanck_clock_b.utanck = 0
, .brgca = 0
, .delay_time = 31
 #endif
        };

/** UART extended configuration for UART on UARTA HAL driver */
const uarta_extended_cfg_t g_uart0_cfg_extend =
{ .transfer_dir = UARTA_DIR_BIT_LSB_FIRST, .transfer_level = UARTA_ALV_BIT_POSITIVE_LOGIC, .clock_output =
          UARTA_CLOCK_OUTPUT_DISABLED,
  .p_baud_setting = &g_uart0_baud_setting, };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg =
{ .channel = 0, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          UartA_CB,
  .p_context = NULL, .p_extend = &g_uart0_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (2),
  .txi_ipl = (2), .eri_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_UARTA0_RXI)
                .rxi_irq             = VECTOR_NUMBER_UARTA0_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_UARTA0_TXI)
                .txi_irq             = VECTOR_NUMBER_UARTA0_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_UARTA0_ERRI)
                .eri_irq             = VECTOR_NUMBER_UARTA0_ERRI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart0 =
{ .p_ctrl = &g_uart0_ctrl, .p_cfg = &g_uart0_cfg, .p_api = &g_uart_on_uarta };
void g_hal_init(void)
{
    g_common_init ();
}
