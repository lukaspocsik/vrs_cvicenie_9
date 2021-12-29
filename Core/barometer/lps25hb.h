#include "main.h"
#include "i2c.h"


#define 	LPS25HB_DEVICE_ADDRESS 					0xBB
#define 	LPS25HB_DEVICE_ADDRESS_0 				0xB8
#define 	LPS25HB_DEVICE_ADDRESS_1 				0xBA

#define 	LPS25HB_WHO_AM_I_ADDRESS				0x0F
#define 	LPS25HB_WHO_AM_I_VALUE					0xBD

#define 	LPS25HB_ADDRESS_CTRL1					0x20
#define 	LPS25HB_ADDRESS_CTRL2					0x21

#define 	LPS25HB_PRESSURE_ADDRESS				0x28

uint8_t lps25hb_init(void);

uint8_t lps25hb_read_byte(uint8_t reg_addr);
void 	lps25hb_readArray(uint8_t * data, uint8_t reg, uint8_t length);
void 	lps25hb_write_byte(uint8_t reg_addr, uint8_t value);

void 	lps25hb_get_pressure(float *p);
void    lps25hb_get_altitude(float *a);

