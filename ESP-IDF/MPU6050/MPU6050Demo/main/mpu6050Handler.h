/*
 * mpu6050.h
 *
 *  Created on: 4 de jul de 2018
 *      Author: Vinicius
 */

#ifndef MAIN_MPU6050HANDLER_H_
#define MAIN_MPU6050HANDLER_H_

#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_err.h"
#include "messageLibrary.h"

#define ORIGINAL_OUTPUT	(0)
#define ACC_FULLSCALE	(2)
#define GYRO_FULLSCALE	(250)

#if ORIGINAL_OUTPUT == 0
	#if ACC_FULLSCALE == 2
		#define AccAxis_Sensitive (float)(16384)
	#elif ACC_FULLSCALE == 4
		#define AccAxis_Sensitive (float)(8192)
	#elif ACC_FULLSCALE == 8
		#define AccAxis_Sensitive (float)(4096)
	#elif ACC_FULLSCALE == 16
		#define AccAxis_Sensitive (float)(2048)
	#endif

	#if GYRO_FULLSCALE == 250
		#define GyroAxis_Sensitive (float)(131.0)
	#elif GYRO_FULLSCALE == 500
		#define GyroAxis_Sensitive (float)(65.5)
	#elif GYRO_FULLSCALE == 1000
		#define GyroAxis_Sensitive (float)(32.8)
	#elif GYRO_FULLSCALE == 2000
		#define GyroAxis_Sensitive (float)(16.4)
	#endif

#else
	#define AccAxis_Sensitive 	(1)
	#define GyroAxis_Sensitive 	(1)
#endif


//#define MPU6050_PIN_SDA				26
#define MPU6050_PIN_SDA				33
//#define MPU6050_PIN_SCL				25
#define MPU6050_PIN_SCL				32

#define I2C_MASTER_NUM				I2C_NUM_0
#define I2C_MASTER_FREQ_HZ    		400000     	/*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0   		/*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0   		/*!< I2C master do not need buffer */

#define MPU6050_ADDR         		0x68 // MPU-6050 device address
#define MPU6050_SMPLRT_DIV   		0x19 // MPU-6050 register address
#define MPU6050_CONFIG       		0x1a
#define MPU6050_GYRO_CONFIG  		0x1b
#define MPU6050_ACCEL_CONFIG 		0x1c
#define MPU6050_WHO_AM_I     		0x75
#define MPU6050_PWR_MGMT_1   		0x6b

#define MPU6050_ACCEL_XOUT_H 		0x3B
#define MPU6050_ACCEL_XOUT_L 		0x3C
#define MPU6050_ACCEL_YOUT_H 		0x3D
#define MPU6050_ACCEL_YOUT_L 		0x3E
#define MPU6050_ACCEL_ZOUT_H 		0x3F
#define MPU6050_ACCEL_ZOUT_L 		0x40
#define MPU6050_TEMP_OUT_H			0x41
#define MPU6050_TEMP_OUT_L			0x42
#define MPU6050_GYRO_XOUT_H			0x43
#define MPU6050_GYRO_XOUT_L			0x44
#define MPU6050_GYRO_YOUT_H			0x45
#define MPU6050_GYRO_YOUT_L			0x46
#define MPU6050_GYRO_ZOUT_H			0x47
#define MPU6050_GYRO_ZOUT_L			0x48

typedef struct MPU6050Context_s
{
    int16_t Raw_Acc_X;
    int16_t Raw_Acc_Y;
    int16_t Raw_Acc_Z;
    int16_t Raw_T;
    int16_t Raw_Gyro_X;
    int16_t Raw_Gyro_Y;
    int16_t Raw_Gyro_Z;
    float Gyro_X;
    float Gyro_Y;
    float Gyro_Z;
    float Acc_X;
    float Acc_Y;
    float Acc_Z;
    float Acc_Angle_X;
    float Acc_Angle_Y;
    short Raw_Temperature;
}MPU6050Context_t;


esp_err_t MPU6050Handler_Calibrate();
MPU6050Context_t MPU6050Handler_GetData();
void MPU6050Handler_Write( uint8_t reg, uint8_t data);

esp_err_t MPU6050Handler_Initialize();
esp_err_t MPU6050Handler_I2C_Initialize(gpio_num_t sda, gpio_num_t scl, i2c_port_t port );
esp_err_t MPU6050Handler_I2C_SlaveWrite( uint8_t slaveAddress, uint8_t regAddress, uint8_t data );
esp_err_t MPU6050Handler_I2C_SlaveRead( uint8_t slaveAddress, uint8_t data , uint8_t *buf, uint32_t len );
esp_err_t MPU6050Handler_I2C_SlaveReadByte( uint8_t slaveAddress, uint8_t reg );

#endif /* MAIN_MPU6050HANDLER_H_ */
