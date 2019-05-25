#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addPoint(double x, double y);
    void clearData();
    void plot();
    void update_linear_t();

private slots:
    void on_btn_add_clicked();

    void on_btn_clear_clicked();

    void clickedGraph(QMouseEvent *event);

    void clickedGraphRelease(QMouseEvent *event);

    void onGraph(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    QVector<double> qv_x, qv_y;
    QVector<double> linear_t, linear_y;
    QPoint c_point;
    int drawing = 0;
};

#endif // MAINWINDOW_H
