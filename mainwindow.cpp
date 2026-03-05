#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(APP_NAME);
    ui->target_size_number->setMaximum(std::numeric_limits<int>::max());
    ui->target_count->setMaximum(std::numeric_limits<int>::max());

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
        ui->target_count->setEnabled(ui->target_count_button->isChecked());
        bool is_target_size = ui->target_size_button->isChecked();
        ui->target_size_number->setEnabled(is_target_size);
        ui->target_size_measurement->setEnabled(is_target_size);
    };
    connect(ui->target_count_button, &QRadioButton::toggled, this, update_size_type);
    connect(ui->target_size_button, &QRadioButton::toggled, this, update_size_type);
    update_size_type();

//     connect(ui->start_button, &QPushButton::clicked, this, [&]{
//         auto is_count = ui->target_count_button->isChecked();
//         uint64_t file_count = 0;

//         auto file_count =
//             target_size.value_or(fs::space(target_folder_path).available) /
//             source_file.size;

//         auto total_size = file_count * source_file.size;

//         QString text = tr("file count is ") + QString::number(file_count) + "(" + QString::number();
//         QMessageBox::StandardButton reply;
//         reply = QMessageBox::question(this, APP_NAME, tr("Quit?"), QMessageBox::Yes|QMessageBox::No);
//         if(reply == QMessageBox::Yes)
//     });
}

void MainWindow::spaming()
{
    // uint64_t file_count = 0;
    // for(size_t i = 0; i < file_count; i++)
    // {
    //     if(i % 10 == 0)
    //     {
    //         auto text = std::to_string(i) + " of " + std::to_string(file_count);
    //         // ui->progressBar->set
    //     }
    //     std::ostringstream oss;
    //     static auto length_fmt = std::to_string(file_count).size();
    //     oss << std::setw(length_fmt) << std::setfill('0') << i;
    //     auto name = source_file.name + oss.str() + source_file.ext;
    //     if(std::ofstream os{target_folder_path / name, std::ios::binary}; os.is_open())
    //         os.write(source_file.data.data(), source_file.data.size());
    //     else
    //         exit_with_failure("unable to open file " + std::to_string(i));
    // }
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, APP_NAME, tr("Quit?"), QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QApplication::quit();
    }
    else
    {
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
