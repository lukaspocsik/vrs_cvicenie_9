#include "hts221.h"

uint8_t data1 = 0;
uint8_t address1 = HTS221_DEVICE_ADDRESS_0;

uint8_t hts221_init(void){

	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = hts221_read_byte(HTS221_WHO_AM_I_ADDRESS);

	if(val == HTS221_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		address1 = HTS221_DEVICE_ADDRESS_1;
		val = hts221_read_byte(HTS221_WHO_AM_I_ADDRESS);
		if(val == HTS221_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else
		{
			status = 0;
			//return status;
		}
	}

	uint8_t ctrl1 = 0b10000111;
	hts221_write_byte(HTS221_ADDRESS_CTRL1, ctrl1); //active mode

	return status;
}

void hts221_get_temperature(float *t)
{
	uint8_t data1[2];
	uint8_t ctrl = hts221_read_byte(HTS221_ADDRESS_CTRL2);

	ctrl |= 0x1;
	hts221_write_byte(HTS221_ADDRESS_CTRL2, ctrl);

	int16_t T_OUT, T0_OUT, T1_OUT, T0_degC, T1_degC;

	uint8_t T1_T0_MSB = hts221_read_byte(HTS221_ADDRESS_T1_T0_MSB);

	T0_degC = hts221_read_byte(HTS221_ADDRESS_T0_degC_x8);
	T1_degC = hts221_read_byte(HTS221_ADDRESS_T1_degC_x8);

	T0_degC += ((T1_T0_MSB & 0x3)<<8);
	T0_degC /= 8;

	T1_degC += ((T1_T0_MSB & ((0x3)<<2))<<6);
	T1_degC /= 8;

	hts221_readArray(data1, HTS221_ADDRESS_T0_OUT_1, 2);
	T0_OUT = (((uint16_t)data1[1]) << 8) | data1[0];

	hts221_readArray(data1, HTS221_ADDRESS_T1_OUT_1, 2);
	T1_OUT = (((uint16_t)data1[1]) << 8) | data1[0];

	hts221_readArray(data1, HTS221_ADDRESS_T_OUT_1, 2);

	T_OUT = ((uint16_t)data1[1]) << 8 | data1[0];
	*t = (float)((T1_degC - T0_degC)*(T_OUT-T0_OUT)/(float)(T1_OUT-T0_OUT)+T0_degC);
}

void hts221_get_humidity(float *h)
{
	uint8_t data[2], H0_rH, H1_rH, H0_rH_x2, H1_rH_x2;
	int16_t H_OUT, H0_T0_OUT, H1_T0_OUT;

	H0_rH_x2 = hts221_read_byte(HTS221_ADDRESS_H0_rH_x2);
	H1_rH_x2 = hts221_read_byte(HTS221_ADDRESS_H1_rH_x2);

	H0_rH = H0_rH_x2/2;
	H1_rH = H1_rH_x2/2;

	hts221_readArray(data, HTS221_ADDRESS_H0_T0_OUT_1, 2);
	H0_T0_OUT = ((uint16_t)data[1]) << 8 | data[0];

	hts221_readArray(data, HTS221_ADDRESS_H1_T0_OUT_1, 2);
	H1_T0_OUT = ((uint16_t)data[1]) << 8 | data[0];

	hts221_readArray(data, HTS221_ADDRESS_H_OUT_1, 2);

	H_OUT = ((uint16_t)data[1]) << 8 | data[0];
	*h = (float)((H1_rH-H0_rH)*(H_OUT-H0_T0_OUT)/(float)(H1_T0_OUT-H0_T0_OUT)+H0_rH);
}

uint8_t hts221_read_byte(uint8_t reg_addr)
{
	data1 = 0;
	return *(i2c_master_read(&data1, 1, reg_addr, HTS221_DEVICE_ADDRESS, 0));
}

void hts221_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, HTS221_DEVICE_ADDRESS, 1);
}


void hts221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, HTS221_DEVICE_ADDRESS, 0);
}
