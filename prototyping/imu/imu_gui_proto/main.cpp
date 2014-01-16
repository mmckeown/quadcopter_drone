#include "imu_gui_proto_main_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImuGuiProtoMainWindow w;
    w.show();
    
    return a.exec();
}
