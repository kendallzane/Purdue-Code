/* pinmux.h - pin multiplexing and control for the BBB ZCZ package */

/* Base Register Address for pinmux control */

#define PINMUXBASE			0x44E10800

/* MUX Control Registers */
#define PIN_CONF_GPMC_AD0		0x00
#define PIN_CONF_GPMC_AD1		0x01
#define PIN_CONF_GPMC_AD2		0x02
#define PIN_CONF_GPMC_AD3		0x03
#define PIN_CONF_GPMC_AD4		0x04
#define PIN_CONF_GPMC_AD5		0x05
#define PIN_CONF_GPMC_AD6		0x06
#define PIN_CONF_GPMC_AD7		0x07
#define PIN_CONF_GPMC_AD8		0x08
#define PIN_CONF_GPMC_AD9		0x09
#define PIN_CONF_GPMC_AD10		0x0A
#define PIN_CONF_GPMC_AD11		0x0B
#define PIN_CONF_GPMC_AD12		0x0C
#define PIN_CONF_GPMC_AD13		0x0D
#define PIN_CONF_GPMC_AD14		0x0E
#define PIN_CONF_GPMC_AD15		0x0F
#define PIN_CONF_GPMC_A0		0x10
#define PIN_CONF_GPMC_A1		0x11
#define PIN_CONF_GPMC_A2		0x12
#define PIN_CONF_GPMC_A3		0x13
#define PIN_CONF_GPMC_A4		0x14
#define PIN_CONF_GPMC_A5		0x15
#define PIN_CONF_GPMC_A6		0x16
#define PIN_CONF_GPMC_A7		0x17
#define PIN_CONF_GPMC_A8		0x18
#define PIN_CONF_GPMC_A9		0x19
#define PIN_CONF_GPMC_A10		0x1A
#define PIN_CONF_GPMC_A11		0x1B
#define PIN_CONF_GPMC_WAIT0		0x1C
#define PIN_CONF_GPMC_WPN		0x1D
#define PIN_CONF_GPMC_BEN1		0x1E
#define PIN_CONF_GPMC_CSN0		0x1F
#define PIN_CONF_GPMC_CSN1		0x20
#define PIN_CONF_GPMC_CSN2		0x21
#define PIN_CONF_GPMC_CSN3		0x22
#define PIN_CONF_GPMC_CLK		0x23
#define PIN_CONF_GPMC_ADVN_ALE		0x24
#define PIN_CONF_GPMC_OEN_REN		0x25
#define PIN_CONF_GPMC_WEN		0x26
#define PIN_CONF_GPMC_BEN0_CLE		0x27
#define PIN_CONF_LCD_DATA0		0x28
#define PIN_CONF_LCD_DATA1		0x29
#define PIN_CONF_LCD_DATA2		0x2A
#define PIN_CONF_LCD_DATA3		0x2B
#define PIN_CONF_LCD_DATA4		0x2C
#define PIN_CONF_LCD_DATA5		0x2D
#define PIN_CONF_LCD_DATA6		0x2E
#define PIN_CONF_LCD_DATA7		0x2F
#define PIN_CONF_LCD_DATA8		0x30
#define PIN_CONF_LCD_DATA9		0x31
#define PIN_CONF_LCD_DATA10		0x32
#define PIN_CONF_LCD_DATA11		0x33
#define PIN_CONF_LCD_DATA12		0x34
#define PIN_CONF_LCD_DATA13		0x35
#define PIN_CONF_LCD_DATA14		0x36
#define PIN_CONF_LCD_DATA15		0x37
#define PIN_CONF_LCD_VSYNC		0x38
#define PIN_CONF_LCD_HSYNC		0x39
#define PIN_CONF_LCD_PCLK		0x3A
#define PIN_CONF_LCD_AC_BIAS_EN		0x3B
#define PIN_CONF_MMC0_DAT3		0x3C
#define PIN_CONF_MMC0_DAT2		0x3D
#define PIN_CONF_MMC0_DAT1		0x3E
#define PIN_CONF_MMC0_DAT0		0x3F
#define PIN_CONF_MMC0_CLK		0x40
#define PIN_CONF_MMC0_CMD		0x41
#define PIN_CONF_MII1_COL		0x42
#define PIN_CONF_MII1_CRS		0x43
#define PIN_CONF_MII1_RX_ER		0x44
#define PIN_CONF_MII1_TX_EN		0x45
#define PIN_CONF_MII1_RX_DV		0x46
#define PIN_CONF_MII1_TXD3		0x47
#define PIN_CONF_MII1_TXD2		0x48
#define PIN_CONF_MII1_TXD1		0x49
#define PIN_CONF_MII1_TXD0		0x4A
#define PIN_CONF_MII1_TX_CLK		0x4B
#define PIN_CONF_MII1_RX_CLK		0x4C
#define PIN_CONF_MII1_RXD3		0x4D
#define PIN_CONF_MII1_RXD2		0x4E
#define PIN_CONF_MII1_RXD1		0x4F
#define PIN_CONF_MII1_RXD0		0x50
#define PIN_CONF_RMII1_REF_CLK		0x51
#define PIN_CONF_MDIO			0x52
#define PIN_CONF_MDC			0x53
#define PIN_CONF_SPI0_SCLK		0x54
#define PIN_CONF_SPI0_D0		0x55
#define PIN_CONF_SPI0_D1		0x56
#define PIN_CONF_SPI0_CS0		0x57
#define PIN_CONF_SPI0_CS1		0x58
#define PIN_CONF_ECAP0_IN_PWM0_OUT	0x59
#define PIN_CONF_UART0_CTSN		0x5A
#define PIN_CONF_UART0_RTSN		0x5B
#define PIN_CONF_UART0_RXD		0x5C
#define PIN_CONF_UART0_TXD		0x5D
#define PIN_CONF_UART1_CTSN		0x5E
#define PIN_CONF_UART1_RTSN		0x5F
#define PIN_CONF_UART1_RXD		0x60
#define PIN_CONF_UART1_TXD		0x61
#define PIN_CONF_I2C0_SDA		0x62
#define PIN_CONF_I2C0_SCL		0x63
#define PIN_CONF_MCASP0_ACLKX		0x64
#define PIN_CONF_MCASP0_FSX		0x65
#define PIN_CONF_MCASP0_AXR0		0x66
#define PIN_CONF_MCASP0_AHCLKR		0x67
#define PIN_CONF_MCASP0_ACLKR		0x68
#define PIN_CONF_MCASP0_FSR		0x69
#define PIN_CONF_MCASP0_AXR1		0x6A
#define PIN_CONF_MCASP0_AHCLKX		0x6B
#define PIN_CONF_XDMA_EVENT_INTR0	0x6C
#define PIN_CONF_XDMA_EVENT_INTR1	0x6D
#define PIN_CONF_WARMRSTN		0x6E
#define PIN_CONF_NNMI			0x70
#define PIN_CONF_TMS			0x74
#define PIN_CONF_TDI			0x75
#define PIN_CONF_TDO			0x76
#define PIN_CONF_TCK			0x77
#define PIN_CONF_TRSTN			0x78
#define PIN_CONF_EMU0			0x79
#define PIN_CONF_EMU1			0x7A
#define PIN_CONF_RTC_PWRONRSTN		0x7E
#define PIN_CONF_PMIC_POWER_EN		0x7F
#define PIN_CONF_EXT_WAKEUP		0x80
#define PIN_CONF_USB0_DRVVBUS		0x87
#define PIN_CONF_USB1_DRVVBUS		0x8D

/* Pin Modes */

#define PIN_MODE_0			0x00
#define PIN_MODE_1			0x01
#define PIN_MODE_2			0x02
#define PIN_MODE_3			0x03
#define PIN_MODE_4			0x04
#define PIN_MODE_5			0x05
#define PIN_MODE_6			0x06
#define PIN_MODE_7			0x07


/* Input Enable Settings */

#define PIN_INPUT_ENABLE		0x20
#define PIN_INPUT_DISABLE		0x00

/* Slew Rate */

#define PIN_SLEW_FAST			0x00
#define PIN_SLEW_SLOW			0x40

/* Pull Settings */

#define PIN_PULL_ENABLE			0x00	
#define PIN_PULL_DISABLE		0x08
#define PIN_PULL_DOWN			0x00
#define PIN_PULL_UP			0x10
