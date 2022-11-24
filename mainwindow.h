#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ichart.h"
#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QSplitter>
#include <QPdfWriter>
#include <QFileDialog>

class MainWindow : public QWidget//
{
    Q_OBJECT
private slots:

    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void on_select_comboboxOnChangedSlot(const int index); // слот, отрабатываемый при изменении типа графика
    void on_print_chart_slot(); // печать графика
    void on_select_dir_slot(); // выбор папки для печати
    void on_color_chart_slot(); // выбор цветности
    void on_reDraw(); // перерисование графика при изменении цвета

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    IChart *chart;
    QString filePath;
    QString homePath;
    QString homePathSavePdf;
    QFileSystemModel *fileModel;
    QTableView *tableView;
    QPushButton *btnPrint;
    QPushButton *selectDir;
    QCheckBox *checkbox;
    QComboBox *combobox;
    QLabel *label, *label_path;
    QChartView *chartView;
    QSplitter *splitter;
    QList <QColor> colorColored;
    QList <QColor> colorBlack_White;
    bool flag_color; // для определения цвета графика
    int countChart;
};


#endif // MAINWINDOW_H
