#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QMenu>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_timeSitting(0),
    m_timeStending(0),
    m_mainTimer(nullptr),
    m_secCounter(0)
{
    ui->setupUi(this);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/Icons/clock.png"));
    connect(trayIcon, &QSystemTrayIcon::activated, this, [=] (QSystemTrayIcon::ActivationReason reason) {
        if(reason == QSystemTrayIcon::DoubleClick){
            if(this->isHidden())
                this->show();
        }
    });

    QMenu *trayMenu = new QMenu(this);
    QAction *exitTrayAction = new QAction(trayMenu);
    exitTrayAction->setText("Остановить таймер и закрыть приложение");
    connect(exitTrayAction, &QAction::triggered, qApp, &QApplication::quit);
    trayMenu->addAction(exitTrayAction);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    m_mainTimer = new QTimer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::hoursToMsec(float countHours)
{
    return static_cast<int>(countHours * 3600 * 1000);
}

void MainWindow::showMessage(typeWork typeWorkForMessage)
{
    QString message;
    switch(typeWorkForMessage){
        case Sitting:{
            message = tr("Пора садиться!");
        }
        break;
        case Stending:{
            message = tr("Пора поработать стоя!");
        }
        break;
    }
    trayIcon->showMessage(tr("Внимание!"), message, QSystemTrayIcon::Information, -1);
}

void MainWindow::updateInformation()
{
    ++m_secCounter;

    switch(m_typeWork){
        case Sitting:{
            if(m_secCounter == m_timeSitting){
                m_typeWork = Stending;
                m_secCounter = 0;
                showMessage(m_typeWork);
            }
        }
        break;
        case Stending:{
            if(m_secCounter == m_timeStending){
                m_typeWork = Sitting;
                m_secCounter = 0;
                showMessage(m_typeWork);
            }
        }
        break;
    }
}

void MainWindow::on_bttnApplySettings_clicked()
{
    float timeSitting = ui->lineEditTimeSitting->text().toFloat();
    float timeStending = ui->lineEditTimeStending->text().toFloat();
    //Если пользователь ввел неправильное значение времени
    if((timeSitting <= 0 || timeSitting > 6) || (timeStending <= 0 || timeStending > 6)) {
        //Возвращаем всё в стандартное положение
        ui->lineEditTimeSitting->setText(QString::number(1));
        ui->lineEditTimeStending->setText(QString::number(1));
        //И сигнализируем о проблеме
        QMessageBox::critical(this,
                              tr("Ошибка"),
                              tr("Вы ввели неправильные значения, проверьте всё и попробуйте снова\n"
                                 "Значение не должно быть меньше 0 и больше 6"));
        return;
    }
    m_timeSitting = hoursToMsec(timeSitting);
    m_timeStending = hoursToMsec(timeStending);


    m_typeWork = static_cast<MainWindow::typeWork>(ui->comboBoxTypeWork->currentIndex());
    switch(m_typeWork){
        case Sitting:{
            m_mainTimer->setInterval(m_timeSitting);
        }
        break;
        case Stending:{
            m_mainTimer->setInterval(m_timeStending);
        }
        break;
    }
    connect(m_mainTimer, &QTimer::timeout, this, &MainWindow::updateInformation);
    m_mainTimer->start(1000);
    ui->bttnApplySettings->setEnabled(false);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_mainTimer->stop();
    this->hide();
    event->ignore();
}
