#include "fwQt/Selector.hpp"



#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <iostream>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>





namespace fwQt
{

Selector::Selector(QWidget *parent, QStringList selection)
{  
  QDialog *box = new QDialog(parent);
  box->resize(250, 150);
  
  m_okButton = new QPushButton("Valider");
  m_cancelButton = new QPushButton("Cancel");
  
  m_list = new QListWidget();
  m_list->addItems(selection);
  
  QHBoxLayout *hLayout = new QHBoxLayout();
  hLayout->addWidget(m_okButton);
  hLayout->addWidget(m_cancelButton);
  
  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->addWidget(m_list);
  vLayout->addLayout(hLayout);

  box->setLayout(vLayout);
 
  m_okButton->show();
  m_cancelButton->show();
  
  box->setModal(true);
  box->show();
}

}