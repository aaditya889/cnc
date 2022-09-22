//Libraries
#include <EEPROM.h>//https://github.com/esp8266/Arduino/blob/master/libraries/EEPROM/EEPROM.h
#include <BasicLinearAlgebra.h>

using namespace BLA;
//Constants
#define EEPROM_SIZE 12

void eeprom_init()
{
  Serial.println("Initialising EEPROM...");
  //Init EEPROM
  EEPROM.begin(EEPROM_SIZE);

}


// void write_to_eeprom() {
//   //Write data into eeprom
//   int address = 0;
//   int boardId = 18;
//   EEPROM.put(address, boardId);
//   address += sizeof(boardId); //update address value
//   float param = 26.5;
//   EEPROM.put(address, param);
//   EEPROM.commit();
//   //Read data from eeprom
//   address = 0;
//   int readId;
//   EEPROM.get(address, readId);
//   Serial.print("Read Id = ");
//   Serial.println(readId);
//   address += sizeof(readId); //update address value
//   float readParam;
//   EEPROM.get(address, readParam); //readParam=EEPROM.readFloat(address);
//   Serial.print("Read param = ");
//   Serial.println(readParam);
//   EEPROM.end();
// }
