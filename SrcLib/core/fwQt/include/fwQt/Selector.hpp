#ifndef FWQT_SELECTOR_HPP_
#define FWQT_SELECTOR_HPP_


#include <QListWidget>
#include <QPushButton>
 #include <QDialog>

namespace fwQt
{

class Selector
{
public:
  Selector(QWidget *parent, QStringList selection);

  QListWidget *m_list;
  QPushButton *m_okButton;
  QPushButton *m_cancelButton;


};
}


#endif 
