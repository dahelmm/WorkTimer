#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum typeWork {
        Sitting,
        Stending
    };

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        int hoursToMsec(float countHours);
    void showMessage(typeWork typeWorkForMessage);

    private slots:
        void updateInformation();
        void on_bttnApplySettings_clicked();

    private:
        Ui::MainWindow *ui;
        QSystemTrayIcon *trayIcon;

        int m_timeSitting;
        int m_timeStending;

        QTimer *m_mainTimer;

        int m_secCounter;

        typeWork m_typeWork;

    // QWidget interface
    protected:
        void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
