#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <cinttypes>
#include <QString>
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

    void on_FreqSlider_valueChanged(int value);
    void on_FreqSpinBox_editingFinished();
    void on_FreqSlider_sliderReleased();

    void on_FileButton_pressed();

private:
    uint8_t files_count;
    uint64_t package_size;
    uint64_t package_freq;

    uint16_t port;

    void Set_Package_Size(uint64_t package_size_local);
    void Set_Package_Freq(uint64_t package_freq_local);

    void Set_Global_Vars();
    void Set_File_status();

    void Start_Receiving_Thread();
    void Start_Sending_Thread(QString message);


    void Data_To_Chat(QByteArray data);

    void call_sending_thread(QString message, QByteArray file = nullptr);

    Ui::MainWindow *ui;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

};
#endif // MAINWINDOW_H
