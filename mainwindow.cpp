#include "ioc.h"
#include "mainwindow.h"
#include <QSplitter>
#include <QListView>
#include <QTreeView>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>
#include <QDebug>
#include <QtWidgets/QWidget>
// библиотеки для построения графиков
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Laba3"); // заголовк окна
    resize(1500, 500); // стандартный размер

    QString str = PRJ_PATH;
    homePath = str + "/Input"; // путь к папке с базами данных

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files); // при распознавании файлов учитывается, что могут быть точки в названии файла
    fileModel->setRootPath(homePath); // сохраняем путь к папке в переменную


    label_path = new QLabel("Choosen Path: "); // надпись с выбранным путём к файлу
    label_path->setText(homePath); // ставим в неё текст из переменной homePath

    label = new QLabel("Выберите тип диаграммы"); // надпись для выбора диаграммы

    combobox = new QComboBox();//выпадающий список с выбором графика
    //combobox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    btnPrint = new QPushButton("Печать графика"); //кнопка печати графика
    //btnPrint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    checkbox = new QCheckBox("Черно-белый график"); //выпадающий список с выбором цвета графика
    //checkbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    selectDir = new QPushButton("Выбор папки"); // кнопка выбора папки
    //selectDir->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);


    countChart = 12; // т.к. данных слишком много, и диаграмма получается чрезмерно большой, сокращаем количество секций в графике (12)

    for (int i = 0; i < countChart; i++){
        int R = rand()%250;
        int G = rand()%250;
        int B = rand()%250; // задаём цвета ячейки в RGB-формате
        int BW = (R+G+B)/3;
        colorColored.push_back(QColor(R,G,B));
        colorBlack_White.push_back(QColor(BW, BW, BW)); // случай для чёрно-белого графика
    }



    QHBoxLayout *layout = new QHBoxLayout(); // располагаем виджеты в горизонтальный компоновщик
    layout->addWidget(label);
    layout->addWidget(combobox);
    layout->addWidget(checkbox);
    layout->addWidget(btnPrint);
    layout->addWidget(selectDir);
    //layout->addStretch();
    layout->setAlignment(Qt::AlignCenter); // и располагаем компоновщик по центру
    //setLayout(layout);

    splitter = new QSplitter(parent); // разделитель

    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // заполнять всё доступное пространство по вертикали


    tableView = new QTableView(splitter); // табличное представление для файловой модели
    tableView->setModel(fileModel); // и располагаем туда файлы из указанной папки с БД


    chartView = new QChartView(splitter);

    flag_color = false;// начальный цвет графика
    splitter->setStretchFactor(0, 1); // начальное положение разделителя
    splitter->setStretchFactor(1, 150);

    QVBoxLayout *layout_vertical = new QVBoxLayout(this); // вертикальный компоновщик
    layout_vertical->addLayout(layout);
    layout_vertical->addWidget(splitter); // он состоит из 3 элементов
    layout_vertical->addWidget(label_path);

    QItemSelectionModel *selectionModel = tableView->selectionModel();

    QModelIndex indexHomePath = fileModel->index(homePath);
    QFileInfo fileInfo = fileModel->fileInfo(indexHomePath);

    combobox->addItem("Bar Chart"); // добавляем в выпадающий список виды диаграмм
    combobox->addItem("Pie Chart");

    /* Рассмотрим способы обхода содержимого папок на диске.
     * Предлагается вариант решения, которы может быть применен для более сложных задач.
     * Итак, если требуется выполнить анализ содержимого папки, то необходимо организовать обход содержимого. Обход выполняем относительно модельного индекса.
     * Например:*/
    if (fileInfo.isDir()) {
        /*
         * Если fileInfo папка то заходим в нее, чтобы просмотреть находящиеся в ней файлы.
         * Если нужно просмотреть все файлы, включая все вложенные папки, то нужно организовать рекурсивный обход.
        */
        QDir dir  = fileInfo.dir();

        if (dir.cd(fileInfo.fileName())) {
            /**
             * Если зашли в папку, то пройдемся по контейнеру QFileInfoList ,полученного методом entryInfoList,
             * */

            foreach (QFileInfo inf, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Type)) {
                qDebug() << inf.fileName() << "---" << inf.size(); // в консоль выводим размер каждого файла
            }

            dir.cdUp();// выходим из папки
        }
    }


    //Выполнение соединения слота и сигнала при выборе файла с данными
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(on_selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
    connect(combobox, &QComboBox::currentIndexChanged, this, &MainWindow::on_select_comboboxOnChangedSlot);// выбор графика
    connect (btnPrint, SIGNAL(clicked()), this, SLOT(on_print_chart_slot())); //печать графика
    connect (selectDir, SIGNAL(clicked()), this, SLOT(on_select_dir_slot())); //выбор папки
    connect (checkbox, SIGNAL(toggled(bool)), this, SLOT(on_color_chart_slot())); // смена цвета

    //Пример организации установки курсора в TableView относительно модельного индекса

    tableView->setRootIndex(fileModel->setRootPath(homePath));
}

// метод смены цвета диаграммы
void MainWindow::on_color_chart_slot(){
    if (checkbox->checkState())
    {
        chart->reColor(colorBlack_White);
        flag_color = true;
    }
    else
    {
        chart->reColor(colorColored);
        flag_color = false;
    }
    on_reDraw(); // перерисовывание диаграммы при смене цвета
}

// выбор папки и смена пути
void MainWindow::on_select_dir_slot()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    if (dialog.exec())
    {
        homePath = dialog.selectedFiles().first(); // назначаем каталог, выбранный пользователем через проводник
    }
    tableView->setRootIndex(fileModel->setRootPath(homePath)); // также нужно изменить корень таблицы файлов
}

//Печать графика через QPdfWriter
void MainWindow::on_print_chart_slot()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    if (dialog.exec())
        homePathSavePdf = dialog.selectedFiles().first(); // назначаем каталог, выбранный пользователем через проводник

    QPdfWriter writer(homePathSavePdf + "/" + "file.pdf");

    writer.setCreator("Author");// Создатель документа
    writer.setPageSize(QPageSize::A4);// Устанавливаем размер страницы А4

    QPainter painter(&writer);

    chartView->render(&painter);
    painter.end();

    QMessageBox mes;
    mes.setText("PDF сохранён в " + homePathSavePdf);//сообщение о том, что график сохранён в pdf
    mes.exec();
}

//Слот для обработки выбора элемента в TableView, выбор осуществляется с помощью курсора

void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    //Q_UNUSED(deselected);
    QModelIndex index = tableView->selectionModel()->currentIndex();
    QModelIndexList indexs =  selected.indexes();
    filePath = "";

    // Размещаем информацию в label_path относительно выделенного модельного индекса

    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        filePath = fileModel->filePath(ix);
        label_path->setText(filePath);
        //this->statusBar()->showMessage("Выбранный путь : " + dirModel->filePath(indexs.constFirst()));
    }


    int length = 200;
    int dx = 30;

    if (fileModel->fileName(index).length() * dx > length) {
        length = length + fileModel->fileName(index).length() * dx; // ширина столбца устанавливается относительно длины названия папки
        qDebug() << "r = " << index.row() << "c = " << index.column() << fileModel->fileName(index) << fileModel->fileInfo(
                     index).size();

    }
    QString type_chart (combobox->currentText()); // определяется тип графика
     if (type_chart == "Pie Chart"){
         gContainer.RegisterInstance<IChart, PieChart>(); // регистрируем экземпляр, подставляя аргумент типа графика
     }
     else
     if (type_chart == "Bar Chart")
     {
         gContainer.RegisterInstance<IChart, BarChart>();
     }
     else { // исключение
         QMessageBox mes;
         mes.setText("Тип графика не верный: " + type_chart);
         mes.exec();
      }



    if(filePath.endsWith(".sqlite")) // проверка формата файла
        gContainer.RegisterInstance<IChartFileData, ChartFileDataSqlite>();
    else if(filePath.endsWith(".json"))
            gContainer.RegisterInstance<IChartFileData, ChartFileDataJson>();
        else
        {
            QMessageBox mes;
            mes.setText("Формат файла выбран не верно"); // исключение
            mes.exec();
        }
    on_reDraw();
}

void MainWindow::on_reDraw(){ // функция для перерисовки цвета графика при смене цвета
    chart = gContainer.GetObject<IChart>().get(); //получаем график
    if(flag_color)
    {
        chart->createChart(gContainer.GetObject<IChartFileData>()->getData(filePath), countChart, colorBlack_White);//рисуем черно-белую диаграмму
    }
    else
        chart->createChart(gContainer.GetObject<IChartFileData>()->getData(filePath), countChart, colorColored); // рисуем цветную диаграмму

    chartView->setChart(chart->getChart());
}

// функция, устанавливающая тип графика
void MainWindow::on_select_comboboxOnChangedSlot(const int index)
{
    //splitter->deleteLater();
    try
    {
        switch (index)
        {
        case 0:
            // устанавливаем вертикальную диаграмму
            gContainer.RegisterInstance<IChart, BarChart>();
            break;
        case 1:
            gContainer.RegisterInstance<IChart, PieChart>();
            // устанавливаем круговую диаграмму
            break;
        default:
            throw std::runtime_error("Unknown display type selected."); // исключение, если не выбран ни один из 2-х типов
            break;
        }
    on_reDraw();
    }
    catch (const std::runtime_error &e) // обработка исключения
    {
        QMessageBox::about(this, "Error", e.what()); // сообщаем, что в процессе выбора вида отображения что-то пошло не так
    }
}

MainWindow::~MainWindow()
{

}
