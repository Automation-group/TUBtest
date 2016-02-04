#include "QlfOven.h"
#include <QtDebug>
#include <stdint.h>
#include <arpa/inet.h>

QlfOven::QlfOven (QIODevice* ioDevice)
  : device (ioDevice) {
}

QByteArray QlfOven::encodeMessage(const QByteArray& message) const {
  static const char encTable[16] = {'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
				    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'};
  QByteArray result;
  QByteArray::const_iterator end = message.end();
  for(QByteArray::const_iterator i = message.begin(); i != end; ++i) {
    unsigned char d = static_cast<unsigned char>(*i);
    result.push_back(encTable[(d >> 4) & 0xF]);
    result.push_back(encTable[d & 0xF]);
  }
  return result;
}

QByteArray QlfOven::decodeMessage(const QByteArray& message) const {
  QByteArray result;
  QByteArray::const_iterator end = message.end();
  bool fullFrame = false;
  char data = 0;
  for(QByteArray::const_iterator i = message.begin(); i != end; ++i) {
    if (fullFrame) {
      data |= (*i) - 'G';
      result.push_back(data);
    } else
      data = ((*i) - 'G') << 4;
    fullFrame = ! fullFrame;
  }
  return result;
}

QByteArray QlfOven::encodeName(const QByteArray& name) const {
  QByteArray uName = name.toUpper();

  QByteArray result;
  QByteArray::const_iterator end = uName.end();
  char prevSymbol = 0;

  for(QByteArray::const_iterator i = uName.begin(); i != end; ++i) {
    char c = *i;
    char symbol = 0;
    switch(c) {
    case '-': symbol = (10 + 26) * 2; break;
    case '_': symbol = (10 + 26 + 1) * 2; break;
    case '/': symbol = (10 + 26 + 2) * 2; break;
    case ' ': symbol = (10 + 26 + 3) * 2; break;
    case '.': prevSymbol++;
    }
    if ( qMax('0', c) == qMin('9', c)) symbol = (c - '0') * 2;
    if ( qMax('A', c) == qMin('Z', c)) symbol = (c - 'A' + 10) * 2;

    if (prevSymbol) result.push_back(prevSymbol);
    prevSymbol = symbol;
  }

  if (prevSymbol) result.push_back(prevSymbol);

  while(result.size()<4) result.push_back((10 + 26 + 3) * 2); // добавил пробелы
  return result;
}

bool QlfOven::writeInt(int addr, const char* name, int value) {
}

QByteArray QlfOven::readData(int addr, const char* name, bool* status) {
  if (status) *status = false;
  QByteArray result;
  
  if (!device) return result;

  QByteArray eName = encodeName(name);
  QByteArray mes;
  mes.push_back(static_cast<char>(addr >> 3)); // старшая часть адреса узла сети
  mes.push_back(
		static_cast<char>((addr & 0x7) << 5) | // младшая часть адреса узла сети
		0x10  // бит запроса - чтение
		);

  unsigned short crc = calculate_name_crc16(eName);
  mes.push_back(static_cast<char>(crc >> 8));
  mes.push_back(static_cast<char>(crc & 0xFF));
  
  crc = calculate_crc16(mes);
  mes.push_back(static_cast<char>(crc >> 8));
  mes.push_back(static_cast<char>(crc & 0xFF));

  QByteArray data;
  data.push_back('#');
  data.append(encodeMessage(mes));
  data.push_back(0x0d);
  device->write(data);

  QByteArray reply;
  char d = 0;
  while(device->getChar(&d)) {
    reply.push_back(d);
    if(d == 0x0d) break;
  }

  if (reply.size() < data.size()) return result;
  if (!(reply.startsWith('#') && reply.endsWith('\r'))) return result;

  reply.remove(0, 1);
  reply.remove(reply.size()-1, 1);
  reply = decodeMessage(reply);
  
  //remove address
  reply.remove(0, 2);
  // remove hash
  reply.remove(0, 2);
  // remove crc
  reply.remove(reply.size()-2, 2);

  result = reply;//.mid(0, 4);
  if (status) *status = true;
  return result;
}

bool QlfOven::writeData(int addr, const char* name, const QByteArray& data) {

  bool result = false;
  
  if (!device) return result;

  QByteArray eName = encodeName(name);
  QByteArray mes;
  mes.push_back(static_cast<char>(addr >> 3)); // старшая часть адреса узла сети
  mes.push_back(
		static_cast<char>((addr & 0x7) << 5) | // младшая часть адреса узла сети
		(data.size() & 0xF)  // кол-во байт данных
		);

  unsigned short crc = calculate_name_crc16(eName);
  mes.push_back(static_cast<char>(crc >> 8));
  mes.push_back(static_cast<char>(crc & 0xFF));
  mes.append(data);
  crc = calculate_crc16(mes);
  mes.push_back(static_cast<char>(crc >> 8));
  mes.push_back(static_cast<char>(crc & 0xFF));

  QByteArray pData;
  pData.push_back('#');
  pData.append(encodeMessage(mes));
  pData.push_back(0x0d);
  device->write(pData);

  QByteArray reply;
  char d = 0;
  while(device->getChar(&d)) {
    reply.push_back(d);
    if(d == 0x0d) break;
  }

  if (reply.size() < data.size()) return result;
  if (!(reply.startsWith('#') && reply.endsWith('\r'))) return result;

  reply.remove(0, 1);
  reply.remove(reply.size()-1, 1);
  reply = decodeMessage(reply);
  
  //remove address
  reply.remove(0, 2);
  // remove hash
  reply.remove(0, 2);
  // remove crc
  reply.remove(reply.size()-2, 2);

  result = true;

  return result;
}

bool QlfOven::write(int addr, const char* name, const QVariant& value, const QByteArray& dataType) {
  QByteArray data;
  union udata_t {
    float v;
    uint8_t c[4];
    uint32_t i;
  } udata;

  switch(value.type()) {
  case QVariant::Double: 
    udata.v = value.toFloat();
    udata.i = htonl(udata.i);
    data.push_back(udata.c[0]);
    data.push_back(udata.c[1]);
    data.push_back(udata.c[2]);
    //data.push_back(udata.c[0]);
    break;
  default: return false;
  }
  return writeData(addr, name, data);
}

QVariant QlfOven::read(int addr, const char* name, const QByteArray& dataType, bool* status) {
  QByteArray binData = readData(addr, name, status);
  QVariant result;
  if (status && (!*status)) return result;

  if (dataType == "F24") {
    union udata_t {
      float v;
      uint8_t c[4];
      uint32_t i;
    } udata;

    udata.c[0] = static_cast<uint8_t>(binData[0]);
    udata.c[1] = static_cast<uint8_t>(binData[1]);
    udata.c[2] = static_cast<uint8_t>(binData[2]);
    udata.c[3] = 0;
    udata.i = ntohl(udata.i);
    return udata.v;
  }

  if (dataType == "ASCII") {
    result = binData;
    return result;
  }

  return result;
}

int QlfOven::readInt(int addr, const char* name, bool* status) {
  QVariant res = read(addr, name, "I", status);
  return res.toInt();
}

unsigned short QlfOven::calculate_name_crc16(const QByteArray& data) const {

  unsigned short crc = 0;
  QByteArray::const_iterator end = data.end();
  for(QByteArray::const_iterator d = data.begin(); d != end; ++d) {
    unsigned char b = (*d) << 1;
    for (int j = 0; j < 7; ++j, b <<= 1) {
      if ((b ^ (crc >> 8)) & 0x80) {
	crc <<= 1;
	crc ^= 0x8F57;
      }
      else
	crc <<= 1;
    }
  }
  return crc;
} 

unsigned short QlfOven::calculate_crc16(const QByteArray& data) const {

  unsigned short crc = 0;
  QByteArray::const_iterator end = data.end();
  for(QByteArray::const_iterator d = data.begin(); d != end; ++d) {
    unsigned char b = *d;
    for (int j = 0; j < 8; ++j, b <<= 1) {
      if ((b ^ (crc >> 8)) & 0x80) {
	crc <<= 1;
	crc ^= 0x8F57;
      }
      else
	crc <<= 1;
    }
  }
  return crc;
} 
