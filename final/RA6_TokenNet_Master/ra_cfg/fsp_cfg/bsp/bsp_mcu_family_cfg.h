/* generated configuration header file - do not edit */
#ifndef BSP_MCU_FAMILY_CFG_H_
#define BSP_MCU_FAMILY_CFG_H_
#ifdef __cplusplus
            extern "C" {
            #endif

#include "bsp_mcu_device_pn_cfg.h"
#include "bsp_mcu_device_cfg.h"
#include "../../../ra/fsp/src/bsp/mcu/ra6m3/bsp_mcu_info.h"
#include "bsp_clock_cfg.h"
#define BSP_MCU_GROUP_RA6M3 (1)
#define BSP_LOCO_HZ                 (32768)
#define BSP_MOCO_HZ                 (8000000)
#define BSP_SUB_CLOCK_HZ            (32768)
#if   BSP_CFG_HOCO_FREQUENCY == 0
#define BSP_HOCO_HZ                 (16000000)
#elif BSP_CFG_HOCO_FREQUENCY == 1
                #define BSP_HOCO_HZ                 (18000000)
            #elif BSP_CFG_HOCO_FREQUENCY == 2
                #define BSP_HOCO_HZ                 (20000000)
            #else
                #error "Invalid HOCO frequency chosen (BSP_CFG_HOCO_FREQUENCY) in bsp_clock_cfg.h"
            #endif

#define BSP_CFG_FLL_ENABLE                 (0)
#ifndef BSP_CLOCK_CFG_AUTODRVEN
#define BSP_CLOCK_CFG_AUTODRVEN            (0)
#endif
#ifndef BSP_CLOCK_CFG_MAIN_OSC_WAIT
#define BSP_CLOCK_CFG_MAIN_OSC_WAIT        (9)
#endif

#define BSP_CORTEX_VECTOR_TABLE_ENTRIES    (16U)
#define BSP_VECTOR_TABLE_MAX_ENTRIES       (112U)
#define BSP_CFG_INLINE_IRQ_FUNCTIONS       (1)

#define OFS_SEQ1 0xA001A001 | (1 << 1) | (3 << 2)
#define OFS_SEQ2 (15 << 4) | (3 << 8) | (3 << 10)
#define OFS_SEQ3 (1 << 12) | (1 << 14) | (1 << 17)
#define OFS_SEQ4 (3 << 18) |(15 << 20) | (3 << 24) | (3 << 26)
#define OFS_SEQ5 (1 << 28) | (1 << 30)
#define BSP_CFG_ROM_REG_OFS0 (OFS_SEQ1 | OFS_SEQ2 | OFS_SEQ3 | OFS_SEQ4 | OFS_SEQ5)
#define BSP_CFG_ROM_REG_OFS1 (0xFFFFFEF8 | (1 << 2) | (3) |  (1 << 8))
#define BSP_CFG_ROM_REG_MPU_PC0_ENABLE (1)
#define BSP_CFG_ROM_REG_MPU_PC0_START (0xFFFFFFFC)
#define BSP_CFG_ROM_REG_MPU_PC0_END (0xFFFFFFFF)
#define BSP_CFG_ROM_REG_MPU_PC1_ENABLE (1)
#define BSP_CFG_ROM_REG_MPU_PC1_START (0xFFFFFFFC)
#define BSP_CFG_ROM_REG_MPU_PC1_END (0xFFFFFFFF)
#define BSP_CFG_ROM_REG_MPU_REGION0_ENABLE (1)
#define BSP_CFG_ROM_REG_MPU_REGION0_START (0x00FFFFFC)
#define BSP_CFG_ROM_REG_MPU_REGION0_END (0x00FFFFFF)
#define BSP_CFG_ROM_REG_MPU_REGION1_ENABLE (1)
#define BSP_CFG_ROM_REG_MPU_REGION1_START (0x200FFFFC)
#define BSP_CFG_ROM_REG_MPU_REGION1_END (0x200FFFFF)
#define BSP_CFG_ROM_REG_MPU_REGION2_ENABLE (1)
#define BSP_CFG_ROM_REG_MPU_REGION2_START (0x407FFFFC)
#define BSP_CFG_ROM_REG_MPU_REGION2_END (0x407FFFFF)
#define BSP_CFG_ROM_REG_MPU_REGION3_ENABLE (1)
#define BSP_CFG_ROM_REG_MPU_REGION3_START (0x400DFFFC)
#define BSP_CFG_ROM_REG_MPU_REGION3_END (0x400DFFFF)

/* Used to create IELS values for the interrupt initialization table g_interrupt_event_link_select. */
#define BSP_PRV_IELS_ENUM(vector)    (ELC_ ## vector)
/*
 ID Code
 Note: To permanently lock and disable the debug interface define the BSP_ID_CODE_PERMANENTLY_LOCKED in the compiler settings.
 WARNING: This will disable debug access to the part and cannot be reversed by a debug probe.
 */
#if defined(BSP_ID_CODE_PERMANENTLY_LOCKED)
            #define BSP_CFG_ID_CODE_LONG_1 (0x00000000)
            #define BSP_CFG_ID_CODE_LONG_2 (0x00000000)
            #define BSP_CFG_ID_CODE_LONG_3 (0x00000000)
            #define BSP_CFG_ID_CODE_LONG_4 (0x00000000)
            #else
/* ID CODE: FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF */
#define BSP_CFG_ID_CODE_LONG_1 (0xFFFFFFFF)
#define BSP_CFG_ID_CODE_LONG_2 (0xFFFFFFFF)
#define BSP_CFG_ID_CODE_LONG_3 (0xFFFFFFFF)
#define BSP_CFG_ID_CODE_LONG_4 (0xffFFFFFF)
#endif

#ifndef BSP_CFG_SDRAM_ENABLED
#define BSP_CFG_SDRAM_ENABLED  (0)
#endif

#ifndef BSP_CFG_SDRAM_TRAS
#define BSP_CFG_SDRAM_TRAS  (6)
#endif

#ifndef BSP_CFG_SDRAM_TRCD
#define BSP_CFG_SDRAM_TRCD  (3)
#endif

#ifndef BSP_CFG_SDRAM_TRP
#define BSP_CFG_SDRAM_TRP  (3)
#endif

#ifndef BSP_CFG_SDRAM_TWR
#define BSP_CFG_SDRAM_TWR  (2)
#endif

#ifndef BSP_CFG_SDRAM_TCL
#define BSP_CFG_SDRAM_TCL  (3)
#endif

#ifndef BSP_CFG_SDRAM_TRFC
#define BSP_CFG_SDRAM_TRFC  (937)
#endif

#ifndef BSP_CFG_SDRAM_TREFW
#define BSP_CFG_SDRAM_TREFW  (8)
#endif

#ifndef BSP_CFG_SDRAM_INIT_ARFI
#define BSP_CFG_SDRAM_INIT_ARFI  (10)
#endif

#ifndef BSP_CFG_SDRAM_INIT_ARFC
#define BSP_CFG_SDRAM_INIT_ARFC  (8)
#endif

#ifndef BSP_CFG_SDRAM_INIT_PRC
#define BSP_CFG_SDRAM_INIT_PRC  (3)
#endif

#ifndef BSP_CFG_SDRAM_MULTIPLEX_ADDR_SHIFT
#define BSP_CFG_SDRAM_MULTIPLEX_ADDR_SHIFT  (1)
#endif

#ifndef BSP_CFG_SDRAM_ENDIAN_MODE
#define BSP_CFG_SDRAM_ENDIAN_MODE  (0)
#endif

#ifndef BSP_CFG_SDRAM_ACCESS_MODE
#define BSP_CFG_SDRAM_ACCESS_MODE  (1)
#endif

#ifndef BSP_CFG_SDRAM_BUS_WIDTH
#define BSP_CFG_SDRAM_BUS_WIDTH  (0)
#endif

#ifdef __cplusplus
            }
            #endif
#endif /* BSP_MCU_FAMILY_CFG_H_ */
