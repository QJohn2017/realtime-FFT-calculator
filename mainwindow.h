#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

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

    void on_transform_clicked();

private:
    Ui::MainWindow *ui;
    QVector<double> qv_x, qv_y;
    QVector<double> linear_t, linear_y;
    QVector<double> linear_f, linear_fy;
    QPoint c_point;
    int n_draw = 500;
    double nonlin_x[500];
    double nonlin_y[500];
    int drawing = 0;
    unsigned n;
    std::vector<double> check_vector_x;
    std::vector<double> check_vector_y;
};

#endif // MAINWINDOW_H
