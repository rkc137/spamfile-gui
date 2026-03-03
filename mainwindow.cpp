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

    static const auto default_palette = palette();
    static const auto bad_palette = std::invoke([&]{
        auto ret = default_palette;
        ret.setColor(QPalette::Text, Qt::red);
        return ret;
    });

    connect(ui->open_input_file, &QPushButton::clicked, this, [&]{
        auto path = QFileDialog::getOpenFileName(this, tr("open file"), tr("file name"));
        ui->input_file->setText(path);
    });
    connect(ui->open_output_folder, &QPushButton::clicked, this, [&]{
        auto path = QFileDialog::getExistingDirectory(this, tr("set folder"), tr("folder name"));
        ui->output_folder->setText(path);
    });
    connect(ui->input_file, &QLineEdit::editingFinished, this, [&]{
        QFileInfo path{ui->input_file->text()};
        ui->input_file->setPalette(
            path.exists() && path.isFile() ? default_palette : bad_palette
        );
    });
    connect(ui->output_folder, &QLineEdit::editingFinished, this, [&]{
        QDir path{ui->output_folder->text()};
        ui->output_folder->setPalette(
            path.exists() ? default_palette : bad_palette
        );
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
