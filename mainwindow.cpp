#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <limits>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->target_size_number->setMaximum(std::numeric_limits<int>::max());
    connect(ui->open_input_file, &QPushButton::clicked, this, [&]{
        auto path = QFileDialog::getOpenFileName(this, "открыть файл", "имя файла");
        ui->input_file->setText(path);
    });
    static const auto default_palette = palette();
    static const auto bad_palette = std::invoke([&]{
        auto ret = default_palette;
        ret.setColor(QPalette::Text, Qt::red);
        return ret;
    });
    connect(ui->input_file, &QLineEdit::editingFinished, this, [&]{
        QFileInfo path{ui->input_file->text()};
        ui->input_file->setPalette(
            path.exists() && path.isFile() ? default_palette : bad_palette
        );
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
