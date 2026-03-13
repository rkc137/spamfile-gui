#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaEnum>

#include <fstream>
#include <limits>
#include <string_view>
#include <utility>
#include <filesystem>
namespace fs = std::filesystem;

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
    bool check_input_path();
    bool check_output_path();
    void closeEvent(QCloseEvent *event) override;

private:
    struct SourceFile
    {
        std::string name, ext, data;
        uint64_t size;
    };
    enum class Measurement
    {
        b, kb, mb, gb
    };
    Q_ENUM(Measurement)
    QMetaEnum metaMeasurement = QMetaEnum::fromType<Measurement>();


    QPalette default_palette;
    QPalette bad_palette;

    Ui::MainWindow *ui = nullptr;

    void spaming(uint64_t file_count, SourceFile &source_file, fs::path target_folder_path);
};
#endif // MAINWINDOW_H
