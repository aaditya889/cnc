#include <I2Cdev.h>
#include <HMC5883L.h>

HMC5883L magnetometer;

// Since magnetometer's X and Y axis are not aligned with IMU's, we're interchanging them, real offsets = {492.50, -315.50, 127.00}
// BLA::Matrix<3> magnetometer_offset = {-315.50, -492.50, 127.00};
// float magnetometer_offset[3] = {492.50, -315.50, 127.00};


void hmc_init()
{
  // class default I2C address is 0x1E
  // specific I2C addresses may be passed as a parameter here
  // this device only supports one I2C address (0x1E)
  // initialize device
  Serial.println("Initializing I2C devices...\n");
  magnetometer.initialize();

  // verify connection
  Serial.println("Testing device connections...\n");
  Serial.println(magnetometer.testConnection() ? "HMC5883L connection successful\n" : "HMC5883L connection failed\n");
}


void fetch_magnetometer_readings()
{
  int16_t mx, my, mz;
  
  // read raw heading measurements from device
  magnetometer.getHeading(&mx, &my, &mz);
  // Since magnetometer's X and Y axis are not aligned with IMU's, we're interchanging them
  MAGNETOMETER_X = my - MAGNETOMETER_OFFSET[MY];
  MAGNETOMETER_Y = mx - MAGNETOMETER_OFFSET[MX];
  MAGNETOMETER_Z = mz - MAGNETOMETER_OFFSET[MZ];
  
  // Derive Yaw from magnetometer readings
  MAGNETOMETER_HEADING = atan2((float)(((float)MAGNETOMETER_Y)), (((float)MAGNETOMETER_X))) * (180 / PI);
}


// void calibrate_magnetometer(BLA::Matrix<3> *magnetometer_readings, int calibration_sequence_time_ms = 20000)
// {
//   BLA::Matrix<3> max_readings = {-INFINITY, -INFINITY, -INFINITY}, min_readings = {INFINITY, INFINITY, INFINITY};
//   BLA::Matrix<3> magnetometer_min_max_offsets[2];
//   // double new_readings;
//   is_magnetometer_calibration_running = true;
//   remote_and_serial_debug((char *)"Magnetometer Calibration:: Start twisting and turning the magnetometer chaotically like a moron!", magnetometer_min_max_offsets);
//   interrupt_magnetometer_calibration.attach_ms(calibration_sequence_time_ms, interrupt_magnetometer_calibration_sequence);
//   fetch_raw_magnetometer_readings(magnetometer_readings);
//   max_readings = min_readings = *magnetometer_readings;
//   while (is_magnetometer_calibration_running)
//   {
//     fetch_raw_magnetometer_readings(magnetometer_readings);
//     // magnetometer_min_max_offsets[0] = *magnetometer_readings;
//     // remote_and_serial_debug((char *)"Magnetometer Readings:: ", magnetometer_min_max_offsets);
//     // new_readings = max((*magnetometer_readings)(MX), max_readings(MX));
//     // max_readings(MX) = max_readings(MX)/2 + new_readings/2;
//     // new_readings = max((*magnetometer_readings)(MY), max_readings(MY));
//     // max_readings(MY) = max_readings(MY)/2 + new_readings/2;
//     // new_readings = max((*magnetometer_readings)(MZ), max_readings(MZ));
//     // max_readings(MZ) = max_readings(MZ)/2 + new_readings/2;

//     // new_readings = min((*magnetometer_readings)(MX), min_readings(MX));
//     // min_readings(MX) = min_readings(MX)/2 + new_readings/2;
//     // new_readings = min((*magnetometer_readings)(MY), min_readings(MY));
//     // min_readings(MY) = min_readings(MY)/2 + new_readings/2;
//     // new_readings = min((*magnetometer_readings)(MZ), min_readings(MZ));
//     // min_readings(MZ) = min_readings(MZ)/2 + new_readings/2;

//     max_readings(MX) = max((*magnetometer_readings)(MX), max_readings(MX));
//     // max_readings(MX) = max_readings(MX)/2 + new_readings/2;
//     max_readings(MY) = max((*magnetometer_readings)(MY), max_readings(MY));
//     // max_readings(MY) = max_readings(MY)/2 + new_readings/2;
//     max_readings(MZ) = max((*magnetometer_readings)(MZ), max_readings(MZ));
//     // max_readings(MZ) = max_readings(MZ)/2 + new_readings/2;

//     min_readings(MX) = min((*magnetometer_readings)(MX), min_readings(MX));
//     // min_readings(MX) = min_readings(MX)/2 + new_readings/2;
//     min_readings(MY) = min((*magnetometer_readings)(MY), min_readings(MY));
//     // min_readings(MY) = min_readings(MY)/2 + new_readings/2;
//     min_readings(MZ) = min((*magnetometer_readings)(MZ), min_readings(MZ));
//     // min_readings(MZ) = min_readings(MZ)/2 + new_readings/2;
//   }
//   magnetometer_offset = (max_readings + min_readings) / 2;
//   Serial << "Magnetometer calibration completed, you can stop being weird now! " << max_readings << "\t" << min_readings << "\n";
//   Serial << "Magnetometer Offset: " << magnetometer_offset << "\n";
//   magnetometer_min_max_offsets[0] = min_readings;
//   magnetometer_min_max_offsets[1] = max_readings;
//   remote_and_serial_debug((char *)"Magnetometer calibration completed, you can stop being weird now!", magnetometer_min_max_offsets);
// }
