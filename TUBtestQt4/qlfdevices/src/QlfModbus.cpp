#include "QlfModbus.h"
#include <QtDebug>

QlfModbus::QlfModbus (QIODevice* ioDevice)
  : device (ioDevice)
{
}

bool QlfModbus::writeWord (unsigned char devAddress, unsigned short address, unsigned short value)
{
  unsigned char data[] = { devAddress, 0x06, address / 256, address % 256, value / 256, value % 256, 0x0, 0x0 };
  unsigned short crc = calculate_crc16 ( data, 6 );
  data[6] = crc % 256;
  data[7] = crc / 256;
  device -> write ((char*)data, 8);
  char reply[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
  int bytesRead = device -> read (reply, 8);
  if ( bytesRead < 8)
    {
      qDebug ("device not response");
      return false;
    }
  return true;
} 

unsigned short QlfModbus::readWord (unsigned char devAddress, unsigned short address)
{
  unsigned short result = 0;
  readWords ( devAddress, address, 1, &result);
  return result;
}

int QlfModbus::readWords (unsigned char devAddress, unsigned short address, 
			  unsigned short wordsToRead, unsigned short* words)
{
  unsigned char data[] = { devAddress, 0x4, address / 256, address % 256, wordsToRead / 256, wordsToRead % 256, 0x0, 0x0 };
  unsigned short crc = calculate_crc16 (data, 6 );
  data[6] = crc % 256;
  data[7] = crc / 256;
  device -> write ((char*)data, 8);
  unsigned char reply[] = {0x0, 0x0, 0x0};
  int bytesRead = device -> read ((char*)reply, 3);
  if ( bytesRead < 3)
    {
      qDebug ("device not response");
      return 0;
    }    
  
  unsigned char bytesTransfered = reply[2];
  unsigned char *buffer = new unsigned char [bytesTransfered + 1];
  memset (buffer, bytesTransfered + 1, 0);
   
  device -> read ((char*)buffer, bytesTransfered);
  
  for (int i = 0; i < bytesTransfered; i += 2)
    {
      words[i/2] = buffer[i] * 256 + buffer[i+1];
    }
  
  delete[] buffer;
  
  device -> read ((char*)&crc, sizeof (unsigned short));
      
  return bytesRead / sizeof (unsigned short);   
}

unsigned short QlfModbus::calculate_crc16(const unsigned char *data, int datalen) const
{
  unsigned short CRC = 0xFFFF;

  while ( datalen-- )
    {
      CRC ^= (unsigned int)(*data);
      for (int i = 0; i < 8; ++i)
	{
	  unsigned short carry = CRC & 1;
	  CRC >>= 1;
	  if ( carry )
	    CRC ^= 0xA001;
	}
      data++;
    }
  return CRC;
} 
