/*
 * mpu6050Handler.c
 *
 *  Created on: 4 de jul de 2018
 *      Author: Vinicius
 */

#include "mpu6050Handler.h"

i2c_port_t _mpu6050I2CPort = 0;

esp_err_t MPU6050Handler_Initialize()
{
	esp_err_t result = ESP_OK;
	esp_err_t functionResult = false;

	// Inicializando a MPU.
	ESP_LOGI( TAG_MPU, LOGI_0045 );

	result = MPU6050Handler_I2C_Initialize( MPU6050_PIN_SDA, MPU6050_PIN_SCL, I2C_NUM_0 );

	// Ligando o modulo.
	ESP_LOGI( TAG_MPU, LOGI_0050 );
	functionResult = MPU6050Handler_I2C_SlaveWrite( MPU6050_ADDR, MPU6050_PWR_MGMT_1 , 0x00 );
	if (functionResult != ESP_OK)
		return functionResult;

	// Configurando a amostragem.
	ESP_LOGI( TAG_MPU, LOGI_0051 );
	functionResult = MPU6050Handler_I2C_SlaveWrite( MPU6050_ADDR, MPU6050_SMPLRT_DIV , 0x07 );
	if (functionResult != ESP_OK)
		return functionResult;

	// Configurando parametros gerais.
	ESP_LOGI( TAG_MPU, LOGI_0052 );
	functionResult = MPU6050Handler_I2C_SlaveWrite( MPU6050_ADDR, MPU6050_CONFIG , 0x07 );
	if (functionResult != ESP_OK)
		return functionResult;

	// Configurando o giroscopio.
	ESP_LOGI( TAG_MPU, LOGI_0053 );
	functionResult = MPU6050Handler_I2C_SlaveWrite( MPU6050_ADDR, MPU6050_GYRO_CONFIG , 0x18 );
	if (functionResult != ESP_OK)
		return functionResult;

	// Configurando o acelerometro.
	ESP_LOGI( TAG_MPU, LOGI_0054 );
	functionResult = MPU6050Handler_I2C_SlaveWrite( MPU6050_ADDR, MPU6050_ACCEL_CONFIG , 0x01 );
	if (functionResult != ESP_OK)
		return functionResult;

	// "MPU inicializada."
	ESP_LOGI( TAG_MPU, LOGI_0046 );

	return result;
}

esp_err_t MPU6050Handler_I2C_Initialize(gpio_num_t sda, gpio_num_t scl, i2c_port_t port )
{
	// Inicializando a comunicacao I2C.
	ESP_LOGI( TAG_MPU, LOGI_0049 );

	_mpu6050I2CPort = port;

	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = sda;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_io_num = scl;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
	i2c_param_config(_mpu6050I2CPort, &conf);
	i2c_driver_install(_mpu6050I2CPort, conf.mode, I2C_MASTER_TX_BUF_DISABLE, I2C_MASTER_RX_BUF_DISABLE, 0);

	return ESP_OK;
}

esp_err_t MPU6050Handler_Calibrate()
{
	// "Calibrando o dispositivo."
	ESP_LOGI( TAG_MPU, LOGI_0055 );

	MPU6050Context_t context;

	for (int i = 0 ; i < 50 ; i++)
	{
		context = MPU6050Handler_GetData();
		ESP_LOGI("[MPU6050]", "ACC(%.2f, %.2f, %.2f) GYRO (%.2f, %.2f, %.2f)", context.Acc_X, context.Acc_Y, context.Acc_Z, context.Gyro_X, context.Gyro_Y, context.Gyro_Z );
	}
	return ESP_OK;
}

float getAccX()
{
	uint8_t r[0];
	if ( MPU6050Handler_I2C_SlaveRead(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, r, 2) != ESP_OK )
		return 0.0;

	short accX = r[0] << 8 | r[1];
	return (float)accX / AccAxis_Sensitive;
}

float getAccY()
{
	uint8_t r[0];
	if ( MPU6050Handler_I2C_SlaveRead(MPU6050_ADDR, MPU6050_ACCEL_YOUT_H, r, 2) != ESP_OK )
		return 0.0;

	short accY = r[0] << 8 | r[1];
	return (float)accY / AccAxis_Sensitive;
}

float getAccZ()
{
	uint8_t r[0];
	if ( MPU6050Handler_I2C_SlaveRead(MPU6050_ADDR, MPU6050_ACCEL_ZOUT_H, r, 2) != ESP_OK )
		return 0.0;

	short accZ = r[0] << 8 | r[1];
	return (float)accZ / AccAxis_Sensitive;
}

float getGyroX()
{
	uint8_t r[0];
	if ( MPU6050Handler_I2C_SlaveRead(MPU6050_ADDR, MPU6050_GYRO_XOUT_H, r, 2) != ESP_OK )
		return 0.0;

	short gyroX = r[0] << 8 | r[1];
	return (float)gyroX / GyroAxis_Sensitive;
}

float getGyroY()
{
	uint8_t r[0];
	if ( MPU6050Handler_I2C_SlaveRead(MPU6050_ADDR, MPU6050_GYRO_YOUT_H, r, 2) != ESP_OK )
		return 0.0;

	short gyroY = r[0] << 8 | r[1];
	return (float)gyroY / GyroAxis_Sensitive;
}

float getGyroZ()
{
	uint8_t r[0];
	if ( MPU6050Handler_I2C_SlaveRead(MPU6050_ADDR, MPU6050_GYRO_ZOUT_H, r, 2) != ESP_OK )
		return 0.0;

	short gyroZ = r[0] << 8 | r[1];
	return (float)gyroZ / GyroAxis_Sensitive;
}

short getTemp()
{
	uint8_t r[0];
	if ( MPU6050Handler_I2C_SlaveRead(MPU6050_ADDR, MPU6050_TEMP_OUT_H, r, 2) != ESP_OK )
		return 0.0;

	return r[0] << 8 | r[1];
}

MPU6050Context_t MPU6050Handler_GetData()
{
	MPU6050Context_t context;

	context.Acc_X = getAccX();
	context.Acc_Y = getAccY();
	context.Acc_Z = getAccZ();
	context.Gyro_X = getGyroX();
	context.Gyro_Y = getGyroY();
	context.Gyro_Z = getGyroZ();

	return context;
}

esp_err_t MPU6050Handler_I2C_SlaveWrite( uint8_t slaveAddress, uint8_t regAddress, uint8_t data )
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, slaveAddress << 1, 1);
	i2c_master_write_byte(cmd, regAddress, 1);
	i2c_master_write_byte(cmd, data, 1);
	i2c_master_stop(cmd);
	int ret = i2c_master_cmd_begin( _mpu6050I2CPort, cmd, 1000 / portTICK_RATE_MS );
	i2c_cmd_link_delete(cmd);

	return ret;
}

esp_err_t MPU6050Handler_I2C_SlaveRead( uint8_t slaveAddress, uint8_t data , uint8_t *buf, uint32_t len )
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start( cmd );
	i2c_master_write_byte( cmd, slaveAddress << 1, 1 );
	i2c_master_write_byte( cmd, data, 1 );
	i2c_master_stop( cmd );
	int ret = i2c_master_cmd_begin( _mpu6050I2CPort, cmd, 1000 / portTICK_RATE_MS );
	i2c_cmd_link_delete( cmd );

	if ( ret != ESP_OK )
		return ret;

	cmd = i2c_cmd_link_create();
	i2c_master_start( cmd );
	i2c_master_write_byte( cmd, slaveAddress << 1 | 1, 1 );

	while( len )
	{
		i2c_master_read_byte( cmd, buf, (len == 1) );
		buf++;
		len--;
	}

	i2c_master_stop( cmd );
	ret = i2c_master_cmd_begin( _mpu6050I2CPort, cmd, 1000 / portTICK_RATE_MS );
	i2c_cmd_link_delete( cmd );

	if ( ret != ESP_OK )
		return ret;

	return ESP_OK;
}

esp_err_t MPU6050Handler_I2C_SlaveReadByte( uint8_t slaveAddress, uint8_t reg )
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start( cmd );
	i2c_master_write_byte( cmd, slaveAddress << 1, 1 );
	i2c_master_write_byte( cmd, reg, 1 );
	i2c_master_stop( cmd );
	int ret = i2c_master_cmd_begin( _mpu6050I2CPort, cmd, 1000 / portTICK_RATE_MS );
	i2c_cmd_link_delete( cmd );

	uint8_t buf;
	cmd = i2c_cmd_link_create();
	i2c_master_start( cmd );
	i2c_master_write_byte( cmd, slaveAddress << 1 | 1, 1 );
	i2c_master_read_byte(cmd, &buf, 1);
	i2c_master_stop( cmd );
	ret = i2c_master_cmd_begin( _mpu6050I2CPort, cmd, 1000 / portTICK_RATE_MS );
	i2c_cmd_link_delete( cmd );

	return ret;
}
