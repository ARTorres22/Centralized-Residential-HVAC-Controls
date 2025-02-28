/* generated ELC source file - do not edit */
#include "r_elc_api.h"
const elc_cfg_t g_elc_cfg =
{ .link[ELC_PERIPHERAL_AD] = ELC_EVENT_TAU0_TMI00, /* TAU0 TMI00 (End of timer channel 00 count or capture) */
  .link[ELC_PERIPHERAL_TAU00] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_TAU01] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_IOPORT1] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_IOPORT2] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_ITLC] = ELC_EVENT_NONE, /* No allocation */
};

#if BSP_TZ_SECURE_BUILD

        void R_BSP_ElcCfgSecurityInit(void);

        /* Configure ELC Security Attribution. */
        void R_BSP_ElcCfgSecurityInit(void)
        {
 #if (2U == BSP_FEATURE_ELC_VERSION)
            uint32_t elcsarbc = UINT32_MAX;

            elcsarbc &=  ~(1U << ELC_PERIPHERAL_AD);

            /* Write the settings to ELCSARn Registers. */
            R_ELC->ELCSARA = 0xFFFFFFFEU;
            R_ELC->ELCSARB = elcsarbc;
 #else
            uint16_t elcsarbc[2] = {0xFFFFU, 0xFFFFU};
            elcsarbc[ELC_PERIPHERAL_AD / 16U] &= (uint16_t) ~(1U << (ELC_PERIPHERAL_AD % 16U));

            /* Write the settins to ELCSARn Registers. */
            R_ELC->ELCSARA = 0xFFFEU;
            R_ELC->ELCSARB = elcsarbc[0];
            R_ELC->ELCSARC = elcsarbc[1];
 #endif
        }
#endif
