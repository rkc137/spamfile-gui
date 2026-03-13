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

    for(int i = 0; i < metaMeasurement.keyCount(); i++)
        ui->target_size_measurement->addItem(metaMeasurement.key(i), metaMeasurement.value(i));

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

    connect(ui->start_button, &QPushButton::clicked, this, [&]{
        if(!check_input_path() || !check_output_path())
        {
            QMessageBox::warning(
                this, tr("path error"),
                tr("input file or output folder is unavailable"),
                QMessageBox::Ok
            );
            return;
        }

        fs::path source_file_path{ui->input_file->text().toStdString()};
        SourceFile source_file;
        if(std::ifstream stream{source_file_path, std::ios::binary}; stream.is_open())
        {
            auto s = fs::file_size(source_file_path);
            source_file.size = s == 0 ? 1 : s;
            source_file.data.assign(std::istreambuf_iterator<char>{stream}, {});
            source_file.name = source_file_path.stem().string();
            source_file.ext = source_file_path.extension().string();
        }
        else
        {
            std::error_code ec;
            auto status = fs::status(source_file_path, ec);
            QMessageBox::critical(this, "Error", QString::fromStdString(ec.message()), QMessageBox::Ok);
            return;
        }

        size_t available_size = fs::space(fs::path{ui->output_folder->text().toStdString()}).available;
        size_t target_size = 0;
        uint64_t file_count = 0;
        if(ui->maximum_button->isChecked())
        {
            file_count = available_size / source_file.size;
            target_size = file_count * source_file.size;
        }
        else if(ui->target_count_button->isChecked())
        {
            // TODO: i lost a plot for a moment get back later
        }
        else if(ui->target_size_button->isChecked())
        {
            // TODO: i lost a plot for a moment get back later
            // file_count = ui->target_size_number->value() * std::pow(1024, ui->target_size_measurement->currentIndex());
            // target_size = file_count * source_file.size;
        }
        else std::unreachable();

        QString text =
            tr("file count is ") + QString::number(file_count) +
            "(" + QString::number(target_size * 1024*1024) + "mb)";

        QMessageBox::StandardButton reply = QMessageBox::question(
            this, tr("continue?"), text, QMessageBox::Yes|QMessageBox::No
        );
        if(reply == QMessageBox::Yes)
            spaming(file_count, source_file, ui->output_folder->text().toStdString());
    });
}

void MainWindow::spaming(uint64_t file_count, SourceFile &source_file, fs::path target_folder_path)
{
    ui->progressBar->setMaximum(file_count);
    ui->progressBar->setMinimum(0);
    auto length_fmt = std::to_string(file_count).size();

    ui->config_frame->setDisabled(true);
    for(size_t i = 0; i < file_count; i++)
    {
        if(i % 10 == 0)
            ui->progressBar->setValue(i);
        std::ostringstream oss;
        oss << std::setw(length_fmt) << std::setfill('0') << i;
        auto name = source_file.name + oss.str() + source_file.ext;
        if(std::ofstream os{target_folder_path / name, std::ios::binary}; os.is_open())
            os.write(source_file.data.data(), source_file.data.size());
        else
            QMessageBox::warning(this, tr("Error"), tr("failed to produce"), QMessageBox::Ok);
    }
    ui->config_frame->setDisabled(false);
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
    auto reply = QMessageBox::question(
        this, APP_NAME, tr("Quit?"), QMessageBox::Yes|QMessageBox::No
    );
    if(reply == QMessageBox::Yes)
        QApplication::quit();
}

MainWindow::~MainWindow()
{
    delete ui;
}
