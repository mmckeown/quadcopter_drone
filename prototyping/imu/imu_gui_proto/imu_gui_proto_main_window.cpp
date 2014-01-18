#include "imu_gui_proto_main_window.h"

ImuGuiProtoMainWindow::ImuGuiProtoMainWindow(QWidget *parent)
    : QWidget (parent),
      m_hBox (new QHBoxLayout),
      m_compassButton (new QPushButton ("Needle")),
      m_compass (new Compass),
      m_attInd (new AttitudeIndicator)
{
    // Connect compass button to compass
    m_compassButton->setCheckable (true);
    connect (m_compassButton, SIGNAL (clicked (bool)), m_compass, SLOT(setRotateNeedle (bool)));

    // Add widgets to layout
    m_hBox->addWidget (m_compassButton, 0, Qt::AlignTop);
    m_hBox->addWidget (m_compass);
    m_hBox->addWidget (m_attInd);

    setLayout (m_hBox);
    setWindowTitle ("IMU GUI Prototype");
    resize (900,400);
}

ImuGuiProtoMainWindow::~ImuGuiProtoMainWindow()
{  
}
