/*******************************************************************/
/*                                                                 */
/* This is the header file for the FlxCard object                  */
/*                                                                 */
/* Author: Markus Joos, CERN                                       */
/*                                                                 */
/**C 2015 Ecosoft - Made from at least 80% recycled source code*****/


#ifndef FLXCARD_H
#define FLXCARD_H

#include <sys/types.h>
//#include "cmem_rcc/cmem_rcc.h"
#include "/home/echurch/felix/software.r3222/drivers_rcc/cmem_rcc/cmem_rcc.h"
#include "flx_common.h"

//#include "regmap/regmap.h"
#include "/home/echurch/felix/software.r3222/regmap/regmap/regmap.h"

//Constants
#define BAR0MAPSIZE                       0x500
#define BAR1MAPSIZE                       0x200
#define BAR2MAPSIZE                       0x8000
#define WAIT_TIME		          600
#define NUM_INTERRUPTS                    8
#define MICRO_WAIT_TIME	                  200
#define FLX_GBT_FILE_NOT_FOUND            -1
#define FLX_GBT_VERSION_NOT_FOUND         -2
#define FLX_GBT_TMODE_FEC                 0
#define FLX_GBT_ALIGNMENT_CONTINUOUS      0
#define FLX_DMA_WRAPAROUND                1
#define FLX_CFG_ERROR_NOT_WRITABLE        2
#define FLX_CFG_ERROR_NOT_WRITABLE        2
#define FLX_CFG_ERROR_NOT_READABLE        1
#define FLX_GBT_ALIGNMENT_ONE             1
#define FLX_GBT_CHANNEL_AUTO              1
#define FLX_GBT_TMODE_WideBus             1
#define FLX_GBT_CHANNEL_MANUAL            0
#define FLX_CFG_ERROR_NO_EXIST            3

//Board models
#define FLX_711                           711
#define FLX_710                           710
#define FLX_709                           709

//I2C definitions
#define I2C_ADDR_SWITCH1_FLX_710          0x70
#define I2C_ADDR_SWITCH1_FLX_709          0x74
#define I2C_ADDR_SWITCH2_FLX_709          0x75
#define I2C_FULL_FLAG                     (1 << 25)
#define I2C_EMPTY_FLAG                    (1 << 8)
#define I2C_DELAY                         1000
#define I2C_SLEEP                         100
#define I2C_ADDR_CXP                      0x6e
#define I2C_SWITCH_CXP1                   (1 << 2)
#define I2C_SWITCH_CXP2                   (1 << 3)
#define I2C_SWITCH_ADN                    (1 << 4)
#define I2C_ADDR_ADN                      0x40

//I2C error codes
#define I2C_DEVICE_ERROR_NOT_EXIST        1
#define I2C_DEVICE_ERROR_INVALID_PORT     2
#define I2C_DEVICE_ERROR_INVALID_ADDRESS  3
#define I2C_DEVICE_ERROR_INVALID_REGISTER 4
#define I2C_DEVICE_ERROR_INVALID_DATA     5

//SPI
#define SPI_FULL_FLAG                     (1ul << 32)
#define SPI_EMPTY_FLAG                    (1ul << 32)
#define SPI_DELAY                         2000

#define ALL_BITS                          0xFFFFFFFFFFFFFFFF


//Register model
typedef struct
{
  volatile u_long start_address;        /*  low half, bits  63:00 */
  volatile u_long end_address;          /*  low half, bits 127:64 */
  volatile u_long tlp         :11;      /* high half, bits  10:00 */
  volatile u_long read        : 1;      /* high half, bit      11 */
  volatile u_long wrap_around : 1;      /* high half, bit      12 */
  volatile u_long reserved    :51;      /* high half, bits  63:13 */
  volatile u_long read_ptr;             /* high half, bits 127:64 */
} dma_descriptor_t;


typedef struct
{
  volatile u_long current_address;      /* bits  63:00 */
  volatile u_long descriptor_done : 1;  /* bit      64 */
  volatile u_long even_addr_dma   : 1;  /* bit      65 */
  volatile u_long even_addr_pc    : 1;  /* bit      66 */
} dma_status_t;


typedef struct
{
  volatile u_int  control;              /* bits  63:00 */
  volatile u_int  data;                 /* bits  95:64 */
  volatile u_long address;              /* bits 127:96 */
} int_vec_t;


typedef struct
{
  volatile u_long date_time : 40;       /* bits  39:00 */
  volatile u_long reserved  : 24;       /* bits  63:40 */
  volatile u_long revision  : 16;       /* bits  79:64 */
} board_id_t;


typedef struct
{
  dma_descriptor_t DMA_DESC[8];	        /* 0x000 - 0x0ff */
  u_char           unused1[256];        /* 0x100 - 0x1ff */
  dma_status_t     DMA_DESC_STATUS[8];  /* 0x200 - 0x27f */
  u_char           unused2[128];	/* 0x280 - 0x2ff */
  volatile u_int   BAR0_VALUE;		/* 0x300 - 0x303 */
  u_char           unused3[12];		/* 0x304 - 0x30f */
  volatile u_int   BAR1_VALUE;		/* 0x310 - 0x313 */
  u_char           unused4[12];		/* 0x314 - 0x31f */
  volatile u_int   BAR2_VALUE;		/* 0x320 - 0x323 */
  u_char           unused5[220];	/* 0x324 - 0x3ff */
  volatile u_int   DMA_DESC_ENABLE;	/* 0x400 - 0x403 */
  u_char           unused6[12];	        /* 0x404 - 0x40f */
  volatile u_int   DMA_FIFO_FLUSH;	/* 0x410 - 0x413 */
  u_char           unused7[12];	        /* 0x414 - 0x41f */
  volatile u_int   DMA_RESET;           /* 0x420 - 0x423 */
  u_char           unused8[12];	        /* 0x424 - 0x42f */
  volatile u_int   SOFT_RESET;          /* 0x430 - 0x433 */
  u_char           unused9[12];	        /* 0x434 - 0x43f */
  volatile u_int   REGISTERS_RESET;     /* 0x440 - 0x443 */
} flxcard_bar0_regs_t;


typedef struct
{
  int_vec_t        INT_VEC[8];		/* 0x000 - 0x07f */
  u_char           unused1[128];	/* 0x080 - 0x0ff */
  volatile u_int   INT_TAB_ENABLE;	/* 0x100 - 0x103 */
} flxcard_bar1_regs_t;


typedef struct spi_device
{
  const char *name;
  const char *description;
  u_char address;
} spi_device_t;


typedef struct
{
  const char *name;
  const char *description;
  u_char address;
  const char *port;
} i2c_device_t;


//Macros
#define HEX(n) std::hex << n << std::dec

//External declarations          //MJ: review
extern i2c_device_t     i2c_devices_FLX_709[];
extern i2c_device_t     i2c_devices_FLX_710[];
extern i2c_device_t     i2c_devices_FLX_711[];
extern spi_device_t     spi_devices[];


class FlxCard
{
public:
    FlxCard();

    //Interaction with the driver
    void card_open(int n);
    void card_close(void);

    //DMA Access
    int  dma_max_tlp_bytes(void);
    void dma_program_write(u_int dma_id, u_long dst, size_t size, u_int tlp, u_int flags);
    void dma_program_read(u_int dma_id, u_long dst, size_t size, u_int tlp, u_int flags);
    void dma_wait(u_int dma_id);
    void dma_stop(u_int dma_id);
    void dma_advance_read_ptr(u_int dma_id, u_long dst, size_t size, size_t bytes);
    void dma_fifo_flush(void);
    void dma_reset(void);
    void dma_soft_reset(void);
    void registers_reset(void);

    //I2C
    void i2c_write_byte(u_char slave_addr, u_char byte);
    void i2c_write_byte_to_addr(u_char slave_addr, u_char addr, u_char byte);
    u_char i2c_read_byte(u_char slave_addr, u_char addr);

    //EEPROM
    void eeprom_read(void);
    void eeprom_write(u_long reg, const char *data);

    //GBT Configuration
    u_int gbt_setup(int soft_alignment, int alignment,int channel_mode, int descrambler_enable, u_long svn_version, long delay);
    long int gbt_version_delay(u_long svn_version, char *filename);

    //IRQ
    void irq_init(void);
    void irq_enable(u_long interrupt);
    void irq_enable_all(void);
    void irq_disable(u_long interrupt);
    void irq_disable_all(void);
    void irq_wait(int n);
    void irq_cancel(void);

    //Register and bitfield access
    void cfg_get_option(const char *key, u_long *value);
    void cfg_set_option(const char *key, u_long value);
    void cfg_set_reg(const char *key, u_long value);
    void cfg_get_reg(const char *key, u_long *value);

    //I2C Tool
    int card_model(void);
    void i2c_devices_read(const char *device, u_char reg_addr, u_char *value);
    void i2c_devices_write(const char *device, u_char reg_addr, u_char data);

    //SPI
    void spi_write_to_addr(u_short addr, u_int byte);
    u_int spi_read_from_addr(u_short addr);
    u_int spi_read(u_short reg);
    void spi_write(u_short reg, u_int data);

    //Service functions (not part of the user API)
    u_long openBackDoor(int bar);

    // Object variables
    static int                      m_cards_open;
    int                             m_timeout;
    int                             m_fd;
    int                             m_slotNumber;
    int                             m_maxTLPBytes;
    int                             m_fd_cmem;
    int                             m_card_model;
    bool                            m_is_open;
    card_params_t                   m_cardData;
    u_long                          m_Bar_0_Base;
    u_long                          m_Bar_1_Base;
    u_long                          m_Bar_2_Base;
    volatile flxcard_bar0_regs_t    *m_bar0;
    volatile flxcard_bar1_regs_t    *m_bar1;
    u_long                          m_physStartAddressCmemBuf;
    u_long                          m_virtStartAddressCmemBuf;
    u_int                           m_lastHandle;
    bool                            verboseFlag;

private:
    u_long i2c_wr_get(void);
    u_long i2c_rd_get(void);
    void i2c_wr_set(u_long value);
    void i2c_rd_set(u_long value);
    void i2c_wait_not_full(void);
    void i2c_wait_not_empty(void);
    int check_digic_value(const char *str, u_char *port1, u_char *port2, u_char *address);
    void str_upper(char *str);
    void check_port_value(const char *str, u_char *port1, u_char *port2);
    void i2c_write(u_char port1, u_char port2, u_char device, u_char reg, u_char data);
    u_char i2c_read(u_char port1, u_char port2, u_char device, u_char reg);
    void gbt_initialization(u_long svn_version, long delay);
    void gbt_tx_configuration(int channel_tx_mode, int alignment);
    int gbt_rx_configuration(int channel_rx_mode, int soft_alignment, u_int demux_mode, int topbot);
    int gbt_software_alignment(int number_channels);
    int gbt_channel_alignment(u_int channel);
    void gbt_topbot_oddeven_set(u_int topbot, u_int oddeven, u_int ch);
    int gbt_topbot_1_alignment(u_int ch, u_long *phase_found, u_int *oddeven_flag);
    int gbt_topbot_0_alignment(u_int ch, u_long *phase_found, u_int *oddeven_flag);
    u_long gbt_shift_phase(u_int ch);
    int check_digic_value2(const char *str, u_long *version, u_long *delay);
    u_long spi_wr_get(void);
    u_long spi_rd_get(void);
    void spi_wr_set(u_long value);
    void spi_rd_set(u_long value);
    void watchdogtimer_handler(int /*signum*/);
    int i2c_full(void);
    int i2c_empty(void);
    void eeprom_selection(char *eeprom);
    int spi_full(void);
    int spi_empty(void);
    u_long map_memory_bar(u_long pci_addr, size_t size);
    void unmap_memory_bar(u_long vaddr, size_t size);
};


#endif // FLXCARD_H
