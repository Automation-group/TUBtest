#include "QlfRNet.h"

QlfRNet::RegValue::RegValue (QlfRNet::RegValue::RegType type)
  : readOnly (false), regType (type)
{
}

QlfRNet::RegValue::RegValue (bool value)
  : readOnly (false) 
{
  regType = QlfRNet::RegValue::Bool;
  data.BoolVal =  value ? 0xFF : 0x00;
}
  
QlfRNet::RegValue::RegValue (char value)
  : readOnly (false)
{
  regType = QlfRNet::RegValue::Byte;
  data.ByteVal = value;
}

QlfRNet::RegValue::RegValue (short value)
  : readOnly (false)
{
  regType = QlfRNet::RegValue::Int;
  data.IntVal = value;
}

QlfRNet::RegValue::RegValue (int value)
  : readOnly (false)
{
  regType = QlfRNet::RegValue::Long;
  data.LongVal = value;
}

QlfRNet::RegValue::RegValue (unsigned char value)
  : readOnly (false)
{
  regType = QlfRNet::RegValue::Ubyte;
  data.UbyteVal = value;
}

QlfRNet::RegValue::RegValue (unsigned short value)
  : readOnly (false)
{
  regType = QlfRNet::RegValue::Uint;
  data.UintVal = value;
}

QlfRNet::RegValue::RegValue (unsigned int value)
  : readOnly (false)
{
  regType = QlfRNet::RegValue::Ulong;
  data.UlongVal = value;
}

QlfRNet::RegValue::RegValue (float value)
  : readOnly (false)
{
  regType = QlfRNet::RegValue::Float;
  data.FloatVal = value;
}

QlfRNet::RegValue::RegValue (double value)
  : readOnly (false)
{
  regType = QlfRNet::RegValue::Double;
  data.DoubleVal = value;
} 

int QlfRNet::RegValue::getDataSize() const
{
  switch (regType)
  {
    case Bool : return 1;
    case Ubyte : return 1;
    case Byte : return 1;
    case Uint : return 2;
    case Int : return 2;
    case Ulong : return 4;
    case Long : return 4;
    case Float : return 4;
    case Double : return 8;
  }
  return 1;
} 

QlfRNet::RegValue::operator bool ()
{
  switch (regType)
  {
    case Bool : return data.BoolVal == 0xFF;
    default :
      qDebug ("conversion failed"); return bool();
  }     
}
        
QlfRNet::RegValue::operator char ()
{
  switch (regType)
  {
    case Bool : return data.BoolVal == 0xFF;
    case Byte : return data.ByteVal;
    default :
      qDebug ("conversion failed"); return char ();
  }     
}

QlfRNet::RegValue::operator short ()
{
  switch (regType)
  {
    case Bool : return data.BoolVal == 0xFF;
    case Ubyte : return data.UbyteVal;
    case Byte : return data.ByteVal;
    case Int : return data.IntVal;
    default :
      qDebug ("conversion failed"); return short ();
  }       
}
  
QlfRNet::RegValue::operator int ()
{
  switch (regType)
  {
    case Bool : return data.BoolVal == 0xFF;
    case Ubyte : return data.UbyteVal;
    case Byte : return data.ByteVal;
    case Uint : return data.UintVal;
    case Int : return data.IntVal;
    case Long : return data.LongVal;
    default :
      qDebug ("conversion failed"); return int ();
  }     
}

QlfRNet::RegValue::operator unsigned char ()
{
  typedef unsigned char returnType;
  switch (regType)
  {
    case Bool : return data.BoolVal == 0xFF;
    case Ubyte : return data.UbyteVal;
    default :
      qDebug ("conversion failed"); return returnType();
  } 
}

QlfRNet::RegValue::operator unsigned short ()
{
  typedef unsigned short returnType;
  switch (regType)
  {
    case Bool : return data.BoolVal == 0xFF;
    case Ubyte : return data.UbyteVal;
    case Uint : return data.UintVal;
    default :
      qDebug ("conversion failed"); return returnType();
  } 
}

QlfRNet::RegValue::operator unsigned int ()
{
  typedef unsigned int returnType;
  switch (regType)
  {
    case Bool : return data.BoolVal == 0xFF;
    case Ubyte : return data.UbyteVal;
    case Uint : return data.UintVal;
    case Ulong : return data.UlongVal;
    default :
      qDebug ("conversion failed"); return returnType();
  } 
}

QlfRNet::RegValue::operator float ()
{
  switch (regType)
  {
    case Bool : return data.BoolVal == 0xFF;
    case Ubyte : return data.UbyteVal;
    case Byte : return data.ByteVal;
    case Uint : return data.UintVal;
    case Int : return data.IntVal;
    case Ulong : return data.UlongVal;
    case Long : return data.LongVal;
    case Float : return data.FloatVal;
    default :
      qDebug ("conversion failed"); return float ();
  } 
} 

QlfRNet::RegValue::operator double ()
{
  switch (regType)
  {
    case Bool : return data.BoolVal == 0xFF;
    case Ubyte : return data.UbyteVal;
    case Byte : return data.ByteVal;
    case Uint : return data.UintVal;
    case Int : return data.IntVal;
    case Ulong : return data.UlongVal;
    case Long : return data.LongVal;
    case Float : return data.FloatVal;
    case Double : return data.DoubleVal;
    default :
      qDebug ("conversion failed"); return double ();
  }   
} 

QlfRNet::QlfRNet (QIODevice* ioDevice)
  : ioDevice (ioDevice)
{
} 

QlfRNet::RegValue QlfRNet::readReg (unsigned char dev, unsigned char cha, unsigned char reg) const
{
  QByteArray message;
  message.push_back (dev);
  message.push_back (cha);
  message.push_back (reg);
  message.push_back ('\0');
  message.push_back ( calculate_crc (message) );
  ioDevice -> write (message);

  char header[] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
  const int headerLen = 5;
  int bytesRead = ioDevice -> read (header, headerLen);
  if ( headerLen != bytesRead )
  {
    qDebug ( "QlfRNet readReg - device not responsed" );
    return QlfRNet::RegValue (QlfRNet::RegValue::Nothing);
  }
    
  unsigned char type = header[4] & 0x3F; 
  QlfRNet::RegValue result ((QlfRNet::RegValue::RegType)type);
  ioDevice -> read ((char*)&result.data, result.getDataSize());
  char crc = 0;
  ioDevice -> read (&crc, 1);
  result.readOnly = header[4] & 0x80;
      
  return result;
}

bool QlfRNet::writeReg (unsigned char dev, unsigned char cha, unsigned char reg, const QlfRNet::RegValue &value) const
{
  QByteArray message;
  message.reserve(10);
  message.push_back ( dev );
  message.push_back ( cha );
  message.push_back ( reg );
  message.push_back ( 0x01 );
  message.push_back ( value.getType() );
  
  const char* data = static_cast<const char*> ( static_cast<const void*> (&value.data) );
  for (int i = 0; i < value.getDataSize(); ++i)
    message.push_back ( data[i] );
  
  int messageLen = message.size ();
  message.push_back ( calculate_crc (message) );

  ioDevice -> write (message);
  
  char reply[] = { 0x0, 0x0, 0x0, 0x0, 0x0 };
  int bytesRead = ioDevice -> read (reply, 5);
  if ( bytesRead < 5)
  {
    qDebug ("QlfRNet writeReg - device not responsed");
    return false;
  }
  return true;
} 

char QlfRNet::calculate_crc(const QByteArray& data) const
{
  unsigned char CRC = 0xFF;
  
  for (QByteArray::const_iterator i = data.constBegin (); i != data.constEnd (); ++i)
  {
    CRC ^= static_cast<unsigned char>(*i);
    for (int i = 0; i < 8; ++i)
    {
      unsigned char aux = CRC & 1;
      CRC >>= 1;
      if ( aux )
        CRC ^= 0x8C;
    }
  }
  return CRC;
}
