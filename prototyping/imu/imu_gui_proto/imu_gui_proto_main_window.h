#ifndef IMU_GUI_PROTO_MAIN_WINDOW_H
#define IMU_GUI_PROTO_MAIN_WINDOW_H

#include <QtWidgets>

#include "compass.h"
#include "attitude_indicator.h"

class ImuGuiProtoMainWindow : public QWidget
{
    Q_OBJECT
    
public:
    ImuGuiProtoMainWindow (QWidget* parent = 0);
    ~ImuGuiProtoMainWindow ();

private:
    QHBoxLayout*        m_hBox;
    Compass*            m_compass;
    AttitudeIndicator*  m_attInd;
};

#endif // IMU_GUI_PROTO_MAIN_WINDOW_H
