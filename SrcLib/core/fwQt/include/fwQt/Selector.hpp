#ifndef FWQT_SELECTOR_HPP_
#define FWQT_SELECTOR_HPP_


#include <QListWidget>
#include <QPushButton>
#include <QDialog>

#include "fwQt/config.hpp"

namespace fwQt
{

class FWQT_CLASS_API Selector
{
public:
 FWQT_API Selector(QWidget *parent, QStringList selection);

  QDialog *box;
  QListWidget *m_list;
  QPushButton *m_okButton;
  QPushButton *m_cancelButton;


};
}


#endif
