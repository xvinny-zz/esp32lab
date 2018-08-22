#include "mpu6050Handler.h"

void app_main(void)
{
	MPU6050Context_t context;

	MPU6050Handler_Initialize();
	MPU6050Handler_Calibrate();

	while(1)
	{
		context = MPU6050Handler_GetData();

		ESP_LOGI(TAG_MPU, "ACC(%.2f,%.2f,%.2f) GYRO(%.2f,%.2f,%.2f)"
						, context.Acc_X, context.Acc_Y, context.Acc_Z
						, context.Gyro_X, context.Gyro_Y, context.Gyro_Z);

		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}
