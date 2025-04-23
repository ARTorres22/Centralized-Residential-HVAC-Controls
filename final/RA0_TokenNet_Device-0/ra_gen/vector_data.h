/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (3)
#endif
/* ISR prototypes */
void iica_master_txrxi_isr(void);
void uarta_txi_isr(void);
void uarta_rxi_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_IICA0_TXRXI ((IRQn_Type) 26) /* IICA0 TXRXI (End of IICA0 communication) */
#define IICA0_TXRXI_IRQn          ((IRQn_Type) 26) /* IICA0 TXRXI (End of IICA0 communication) */
#define VECTOR_NUMBER_UARTA0_TXI ((IRQn_Type) 40) /* UARTA0 TXI (UARTA0 transmission transfer end or buffer empty interrupt) */
#define UARTA0_TXI_IRQn          ((IRQn_Type) 40) /* UARTA0 TXI (UARTA0 transmission transfer end or buffer empty interrupt) */
#define VECTOR_NUMBER_UARTA0_RXI ((IRQn_Type) 41) /* UARTA0 RXI (UARTA0 reception transfer end) */
#define UARTA0_RXI_IRQn          ((IRQn_Type) 41) /* UARTA0 RXI (UARTA0 reception transfer end) */
#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
