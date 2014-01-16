#include "imu_gui_proto_main_window.h"

ImuGuiProtoMainWindow::ImuGuiProtoMainWindow(QWidget *parent)
    : QWidget (parent),
      m_hBox (new QHBoxLayout),
      m_compass (new Compass),
      m_attInd (new AttitudeIndicator)
{
    m_hBox->addWidget (m_compass);
    m_hBox->addWidget (m_attInd);

    setLayout (m_hBox);
    setWindowTitle ("IMU GUI Prototype");
    resize (800,400);
}

ImuGuiProtoMainWindow::~ImuGuiProtoMainWindow()
{  
}
