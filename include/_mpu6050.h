#ifndef __constants_h__
#define __constants_h__
#endif
#include <BasicLinearAlgebra.h>


using namespace BLA;


// MPU6050 Slave Device Address
const uint8_t MPU6050_SLAVE_ADDRESS = 0x68;
// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV = 0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL = 0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1 = 0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2 = 0x6C;
const uint8_t MPU6050_REGISTER_CONFIG = 0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG = 0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG = 0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN = 0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE = 0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H = 0x3B;
const uint8_t MPU6050_REGISTER_ACCEL_YOUT_H = 0x3D;
const uint8_t MPU6050_REGISTER_ACCEL_ZOUT_H = 0x3F;
const uint8_t MPU6050_REGISTER_GYRO_XOUT_H = 0x43;
const uint8_t MPU6050_REGISTER_GYRO_YOUT_H = 0x45;
const uint8_t MPU6050_REGISTER_GYRO_ZOUT_H = 0x47;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET = 0x68;
const uint8_t MPU6050_REGISTER_WHOAMI = 0x75;

const uint16_t ACC_SCALE_FACTOR = 16384;
const uint16_t GYRO_SCALE_FACTOR = 131;

#define AX 0
#define AY 1
#define AZ 2
#define GX 0
#define GY 1
#define GZ 2
#define ROLL 0
#define PITCH 1
#define YAW 2

float gyro_avg_x = 0.0, gyro_avg_y = 0.0, gyro_avg_z = 0.0;


void i2c_write(uint8_t device_address, uint8_t reg_address, uint8_t data)
{
  Wire.beginTransmission(device_address);
  Wire.write(reg_address);
  Wire.write(data);
  Wire.endTransmission();
}


void fetch_imu_data()
{ 
  uint8_t device_address = MPU6050_SLAVE_ADDRESS, reg_address = MPU6050_REGISTER_ACCEL_XOUT_H; 
  static unsigned long imu_start_time = 0;
  // static uint16_t acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z;
  double delta_time;

  Wire.beginTransmission(device_address);
  Wire.write(reg_address);
  Wire.endTransmission();
  Wire.requestFrom(device_address, (uint8_t)14);
  while (!Wire.available()){};
  
  ACC_X = (float)((int16_t)(((int16_t)Wire.read() << 8) | Wire.read()))/(float)ACC_SCALE_FACTOR; // Acc X
  ACC_Y = (float)((int16_t)(((int16_t)Wire.read() << 8) | Wire.read()))/(float)ACC_SCALE_FACTOR;// Acc Y
  ACC_Z = (float)((int16_t)(((int16_t)Wire.read() << 8) | Wire.read()))/(float)ACC_SCALE_FACTOR; //Acc Z
  
  (float)((int16_t)(((int16_t)Wire.read() << 8) | Wire.read())); // Temp
  
  GYRO_X= ((float)((int16_t)(((int16_t)Wire.read() << 8) | Wire.read()))/(float)GYRO_SCALE_FACTOR) - gyro_avg_x;//Gyro X
  GYRO_Y = ((float)((int16_t)(((int16_t)Wire.read() << 8) | Wire.read()))/(float)GYRO_SCALE_FACTOR) - gyro_avg_y;//Gyro Y
  GYRO_Z = ((float)((int16_t)(((int16_t)Wire.read() << 8) | Wire.read()))/(float)GYRO_SCALE_FACTOR) - gyro_avg_z;//Gyro Z

  delta_time = (micros() - imu_start_time)/(double)MEGA;
  // Serial << GYRO_Y << " ";
  // Serial << GYRO_Y << " ";
  // Serial.println(delta_time);

  GYRO_DEG_X += GYRO_X*(float)delta_time;
  GYRO_DEG_Y += GYRO_Y*(float)delta_time;
  GYRO_DEG_Z += GYRO_Z*(float)delta_time;
  
  imu_start_time = micros();
}


//configure MPU6050
void mpu_init()
{
  Serial.println("Initialising MPU6050...");
  i2c_write(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  i2c_write(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  i2c_write(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  // i2c_write(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_CONFIG, 0x05);
  i2c_write(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_CONFIG, 0x00);
  i2c_write(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_GYRO_CONFIG, 0x00);  //set +/-250 degree/second full scale
  i2c_write(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_CONFIG, 0x00); // set +/- 2g full scale
  i2c_write(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_FIFO_EN, 0x00);
  i2c_write(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_INT_ENABLE, 0x01);
  i2c_write(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  i2c_write(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_USER_CTRL, 0x00);

  uint16_t avg_count = 500;

  float temp_gyro_avg_x = 0.0, temp_gyro_avg_y = 0.0, temp_gyro_avg_z = 0.0;

  for (uint16_t i = 0; i < avg_count; i++)
  {
    fetch_imu_data();
    temp_gyro_avg_x += GYRO_X/(float)avg_count;
    temp_gyro_avg_y += GYRO_Y/(float)avg_count;
    temp_gyro_avg_z += GYRO_Z/(float)avg_count;
  }
  gyro_avg_x = temp_gyro_avg_x;
  gyro_avg_y = temp_gyro_avg_y;
  gyro_avg_z = temp_gyro_avg_z;
  Serial << "Got GYRO_AVG: " << gyro_avg_x << gyro_avg_y << gyro_avg_z << "\n";

  // if (MPU_GYRO_AVG(GZ) > 2.7) // TODO: Try to fix this without resetting!
  // {
  //   Serial << "Got incorrect average values, resetting the module...\n";
  //   ESP.restart();
  // }

  Serial.println("MPU6050 initialised!");
}
