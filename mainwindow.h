#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <fstream>
#include <limits>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static constexpr auto APP_NAME = "Spamfile";

public slots:
    void spaming();
    bool check_input_path();
    bool check_output_path();
    void closeEvent(QCloseEvent *event) override;

private:
    QPalette default_palette;
    QPalette bad_palette;

    Ui::MainWindow *ui = nullptr;
};
#endif // MAINWINDOW_H
