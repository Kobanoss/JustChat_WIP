#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <cinttypes>
#include <qudpsocket.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

private slots:
    void on_SendButton_pressed();

    void on_SizeSlider_valueChanged(int value);
    void on_SizeSpinBox_editingFinished();
    void on_SizeSlider_sliderReleased();

    void on_DelaySlider_valueChanged(int value);
    void on_DelaySpinBox_editingFinished();
    void on_DelaySlider_sliderReleased();

    void on_FileButton_pressed();

    void on_requestDevileryCheckBox_stateChanged(int arg1);
    void on_requestLostCheckBox_stateChanged(int arg1);

private:
    std::uint8_t files_count;
    std::uint64_t package_size;
    std::uint64_t package_delay;
    QMap<QByteArray, QByteArray> files_data;
    QByteArray filename;

    std::uint16_t port;

    bool request_status;
    bool request_lost_status;

    void Set_Package_Size(std::uint64_t package_size_local);
    void Set_Package_Delay(std::uint64_t package_delay_local);

    void Set_Global_Vars();
    void Set_File_status();

    void Start_Receiving_Thread();
    void Start_Sending_Thread(QString message);


    void Data_To_Chat(QByteArray data);
    void Data_To_File(QByteArray data, QString filename);

    void Call_Sending_Thread(QString message);

    Ui::MainWindow *ui;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

};
#endif // MAINWINDOW_H
