#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <limits>
#include <QFileDialog>

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

public slots:
    bool check_input_path();
    bool check_output_path();

private:
    QPalette default_palette;
    QPalette bad_palette;

    Ui::MainWindow *ui = nullptr;
};
#endif // MAINWINDOW_H
