#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->target_size_number->setMaximum(std::numeric_limits<int>::max());

    default_palette = palette();
    bad_palette = default_palette;
    bad_palette.setColor(QPalette::Text, Qt::red);

    connect(ui->open_input_file, &QPushButton::clicked, this, [&]{
        auto path = QFileDialog::getOpenFileName(this, tr("open file"), tr("file name"));
        ui->input_file->setText(path);
        check_input_path();
    });
    connect(ui->open_output_folder, &QPushButton::clicked, this, [&]{
        auto path = QFileDialog::getExistingDirectory(this, tr("set folder"), tr("folder name"));
        ui->output_folder->setText(path);
        check_output_path();
    });
    connect(ui->input_file, &QLineEdit::editingFinished, this, &MainWindow::check_input_path);
    connect(ui->output_folder, &QLineEdit::editingFinished, this, &MainWindow::check_output_path);

    auto update_size_type = [&]{
        bool is_count = ui->target_count_button->isChecked();
        ui->target_count->setEnabled(is_count);
        ui->target_size_number->setDisabled(is_count);
        ui->target_size_measurement->setDisabled(is_count);
    };
    connect(ui->target_count_button, &QRadioButton::toggled, this, update_size_type);
    connect(ui->target_size_button, &QRadioButton::toggled, this, update_size_type);
    update_size_type();
}

bool MainWindow::check_input_path()
{
    QFileInfo path{ui->input_file->text()};
    bool is_valid = path.exists() && path.isFile();
    ui->input_file->setPalette(is_valid ? default_palette : bad_palette);
    return is_valid;
}

bool MainWindow::check_output_path()
{
    QDir path{ui->output_folder->text()};
    bool is_valid = path.exists();
    ui->output_folder->setPalette(is_valid ? default_palette : bad_palette);
    return is_valid;
}

MainWindow::~MainWindow()
{
    delete ui;
}
