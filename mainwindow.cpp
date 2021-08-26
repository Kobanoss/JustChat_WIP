#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "udp_sender.cpp"
#include "udp_receiver.cpp"
#include "udp_receiver.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QWidget::setEnabled(true);
    ui->SendButton->setEnabled(true);
    ui->InputText->setEnabled(true);
    ui->DelaySlider->setEnabled(true);
    ui->SizeSlider->setEnabled(true);

    Set_Global_Vars();

    QThread *receiving_thread = new QThread();
    UdpReceiver *receiver = new UdpReceiver(port);
    receiver->moveToThread(receiving_thread);
    connect(receiving_thread,SIGNAL(started()),receiver,SLOT(init_Thread()));
    connect(receiver, &UdpReceiver::to_Chat, this, &MainWindow::Data_To_Chat, Qt::BlockingQueuedConnection);
    connect(receiver, &UdpReceiver::to_File, this, &MainWindow::Data_To_File, Qt::BlockingQueuedConnection);
    receiving_thread->start();

}




MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::Set_Global_Vars() {
    package_size = 1;
    package_delay = 1;
    files_count = 0;
    files_data = {};
    port = 10000;
    request_status = false;
}



void MainWindow::on_SizeSlider_valueChanged(int package_size_local) {
    ui->SizeSpinBox->setValue((std::uint64_t)package_size_local);
}
void MainWindow::on_SizeSpinBox_editingFinished() {
    ui->SizeSlider->setValue((std::uint64_t)ui->SizeSpinBox->value());
    Set_Package_Size((std::uint64_t)ui->SizeSpinBox->value());
}
void MainWindow::on_SizeSlider_sliderReleased() {
    ui->SizeSpinBox->setValue((std::uint64_t)ui->SizeSlider->value());
    Set_Package_Size((std::uint64_t)ui->SizeSpinBox->value());
}
void MainWindow::Set_Package_Size(std::uint64_t package_size_local) {
    package_size = package_size_local;
     ui->LogBrowser->append(("Changed package size to "+QString::number(package_size_local)+'\n' ));
}


void MainWindow::on_DelaySlider_valueChanged(int package_delay_local){
    ui->DelaySpinBox->setValue((std::uint64_t)package_delay_local);
}
void MainWindow::on_DelaySpinBox_editingFinished() {
    ui->DelaySlider->setValue((std::uint64_t)ui->DelaySpinBox->value());
    Set_Package_Delay((std::uint64_t)ui->DelaySpinBox->value());
}
void MainWindow::on_DelaySlider_sliderReleased() {
    ui->DelaySpinBox->setValue((std::uint64_t)ui->DelaySlider->value());
    Set_Package_Delay((std::uint64_t)ui->DelaySpinBox->value());
}
void MainWindow::Set_Package_Delay(std::uint64_t package_delay_local) {
    package_delay = package_delay_local;
    ui->LogBrowser->append(("Changed package sending delay to "+QString::number(package_delay_local)+'\n' ));
}



void MainWindow::Set_File_status() {
    ui->FileStatus->setText(QString::number(files_count) + " files added");
}


void MainWindow::on_SendButton_pressed() {
    QString message = ui->InputText->toPlainText();
    if (message.size() || files_count) {
        ui->ChatBrowser->append(message+((files_count>0)?("\n### Connected "+QString::number(files_count)+" files"):"\n"));
        Call_Sending_Thread(message);
    }
    ui->InputText->clear();

    files_count = 0;
    Set_File_status();
}


void MainWindow::on_FileButton_pressed() {
    QString raw_filename = QFileDialog::getOpenFileName(this, "Open File", QString(), "*.*");
    QFileInfo file_info(raw_filename);
    filename = file_info.fileName().toUtf8();
    QFile opened_file(raw_filename);
    opened_file.open(QIODevice::ReadOnly);
    files_data[filename] = opened_file.readAll();
    qDebug() << files_data[filename];
    files_count++;
    ui->LogBrowser->append("File added to message (total = "+QString::number(files_count)+")\n");
    Set_File_status();

}

void MainWindow::Call_Sending_Thread(QString message) {
    QThread *sending_thread = new QThread();
    UdpSender * send = new UdpSender(package_size, package_delay, port, request_status, message, files_data);
    send->moveToThread(sending_thread);
    send->connect(sending_thread,SIGNAL(started()),send,SLOT(init_Thread()));
    sending_thread->start();
    files_data = {};
}

void MainWindow::Data_To_Chat(QByteArray data) {
    ui->ChatBrowser->append(QString(data) + "\n");
}

void MainWindow::Data_To_File(QByteArray data, QString filename) {
    QDir download_dir("./Downloads");
    if (!download_dir.exists()) {
        download_dir.mkpath("");
    }

    QFile file("./Downloads/" + filename);
    if (file.open(QIODevice::ReadWrite)) {
            file.write(data);
        }
    file.close();
}


void MainWindow::on_requestDevileryCheckBox_stateChanged(int state) {
    request_status = (state == 0)?false:true;
    if (state == 0) ui->requestLostCheckBox->setChecked(false);
}


void MainWindow::on_requestLostCheckBox_stateChanged(int state)
{
    request_lost_status = (state == 0)?false:true;
}

