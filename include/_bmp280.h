using namespace BLA;

Adafruit_BMP280  bmp280;
float altitude_offset = 0.0;


void fetch_altitude()
{
  float new_altimeter_weight = 0.2;
  ALTITUDE = (ALTITUDE*(1 - new_altimeter_weight)) + ((bmp280.readAltitude() - altitude_offset)*new_altimeter_weight);      // get altitude
}


void fetch_raw_altitude()
{
  ALTITUDE = bmp280.readAltitude();      // get altitude
}


void set_bmp_offset()
{
  uint16_t avg_count = 500;
  // float altitude = 0.0;

  for (uint16_t i = 0; i < avg_count; i++)
  {
    fetch_raw_altitude();
    altitude_offset += (ALTITUDE/500);
  }
  // altitude_offset /= (double)avg_count;

  Serial << "Got altitude offset: " << altitude_offset << "\n";
}


void bmp_init()
{
  // define device I2C address: 0x76 or 0x77 (0x77 is library default address)
  #define BMP280_I2C_ADDRESS  0x76
  Serial << "Initialising BMP280...\n";
  if ( bmp280.begin(BMP280_I2C_ADDRESS) == 0 ) {
    // connection error or device address wrong!
    Serial << "BMP280 connection error!\n";
    while(1);  // stay here
  }
  set_bmp_offset();
  Serial << "BMP280 Initialised!\n";
}
