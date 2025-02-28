/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_tau.h"
#include "r_timer_api.h"
#include "r_adc_d.h"
#include "r_adc_api.h"
#include "r_iica_master.h"
#include "r_i2c_master_api.h"
#include "r_uarta.h"
#include "r_uart_api.h"
FSP_HEADER
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer_adc0_group;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer_adc0_group_ctrl;
extern const transfer_cfg_t g_transfer_adc0_group_cfg;
/** TAU Timer Instance */
extern const timer_instance_t g_timer0;

/** Access the TAU instance using these structures when calling API functions directly (::p_api is not used). */
extern tau_instance_ctrl_t g_timer0_ctrl;
extern const timer_cfg_t g_timer0_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc0;

/** Access the ADC instance using these structures when calling API functions directly (::p_api is not used). */
extern adc_d_instance_ctrl_t g_adc0_ctrl;
extern const adc_cfg_t g_adc0_cfg;
extern const adc_d_channel_cfg_t g_adc0_channel_cfg;

#ifndef g_adc0_callback
void g_adc0_callback(adc_callback_args_t *p_args);
#endif
/* IICA Master on IICA Instance. */
extern const i2c_master_instance_t g_iica_master0;

/** Access the IICA Master instance using these structures when calling API functions directly (::p_api is not used). */
extern iica_master_instance_ctrl_t g_iica_master0_ctrl;
extern const i2c_master_cfg_t g_iica_master0_cfg;

#ifndef i2c_master_callback
void i2c_master_callback(i2c_master_callback_args_t *p_args);
#endif
/** UART on UARTA Instance. */
extern const uart_instance_t g_uart0;

/** Access the UARTA instance using these structures when calling API functions directly (::p_api is not used). */
extern uarta_instance_ctrl_t g_uart0_ctrl;
extern const uart_cfg_t g_uart0_cfg;
extern const uarta_extended_cfg_t g_uart0_cfg_extend;

#ifndef UartA_CB
void UartA_CB(uart_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
