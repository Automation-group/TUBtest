#ifndef SEARCHDEVICES_H_
#define SEARCHDEVICES_H_

#include <QString>
#include <QlfTUB.h>

class SearchDevices
{
  public:
    void SearchDev(QString *portTUB);
  
  private:
    QlfTUB* tub;
  
  protected:
    bool SearchTUB(QString port);
    QString osName();
};

#endif /*SEARCHDEVICES_H_*/
