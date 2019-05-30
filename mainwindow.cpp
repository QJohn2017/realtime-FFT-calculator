#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<gsl/gsl_interp.h>
#include<iostream>
#include<algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plot->addGraph();
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->plot->addGraph();
    ui->plot->graph(1)->setScatterStyle(QCPScatterStyle::ssPeace);
    ui->plot->graph(1)->setLineStyle(QCPGraph::lsNone);
    connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)),SLOT(clickedGraph(QMouseEvent*)));
    connect(ui->plot, SIGNAL(mouseMove(QMouseEvent*)),SLOT(onGraph(QMouseEvent*)));
    connect(ui->plot, SIGNAL(mouseRelease(QMouseEvent*)),SLOT(clickedGraphRelease(QMouseEvent*)));
    // set linear x data
    double tmax = 30; // seconds
    double tmin = -30; // seconds
    double t_span = tmax - tmin;
    int N = 100;
    double dt = t_span / N;
    double t;
    for(t=tmin;t<tmax;t+=dt){
        linear_t.append(t);
        linear_y.append(0);
    }
    ui->plot->xAxis->setRange(tmin, tmax);
    ui->plot->yAxis->setRange(-5, 5);
    // qDebug() << dt;
    // qDebug()<<linear_t;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addPoint(double x, double y)
{
    qv_x.append(x);
    qv_y.append(y);
}

void MainWindow::clearData()
{
    qv_x.clear();
    qv_y.clear();
}

void MainWindow::plot()
{
    ui->plot->graph(0)->setData(qv_x, qv_y);
    ui->plot->replot();
    ui->plot->update();
}

void MainWindow::update_linear_t(){

    qDebug()<<"set linear data";
    // unsigned n = qv_x.length();

    check_vector_x.clear();
    check_vector_y.clear();
    check_vector_x = qv_x.toStdVector();
    check_vector_y = qv_y.toStdVector();
    // make sure the x axis is only increasing
    qDebug() << "before";
    qDebug() << check_vector_x;
    std::vector<double>::size_type i;
    i = 0;
    while(i < check_vector_x.size()-1){
        if (check_vector_x[i] >= check_vector_x[i+1]){
            check_vector_x.erase(check_vector_x.begin()+i+1);
            check_vector_y.erase(check_vector_y.begin()+i+1);
        }
        else{
            i++;
        }
    }
    qDebug() << "after";
    qDebug()<<check_vector_x;

    // append the vector contents to array
    n = check_vector_x.size();
    for (i=0;i<n;i++){
        nonlin_x[i] = check_vector_x[i];
        nonlin_y[i] = check_vector_y[i];
        if (n>=100) break;
    }
    // qDebug() << nonlin_x;
    qDebug() << "n: ";
    qDebug() << n;
    // the number of points cant exceed the interp array size
    if (n > 100){
        n = 100;
    }
    qDebug() << "n: ";
    qDebug() << n;

    gsl_interp *interpolation = gsl_interp_alloc(gsl_interp_linear, n);
    gsl_interp_init(interpolation, nonlin_x, nonlin_y, n);

    gsl_interp_accel *accelerator = gsl_interp_accel_alloc();
    double value = gsl_interp_eval(interpolation, nonlin_x, nonlin_y, 2.0, accelerator);

    // double value = gsl_interp_eval(interpolation, &qv_x[0], &qv_y[0], 2.0, accelerator);
    // qDebug()<<value;

    // qDebug()<<linear_t;
    // qDebug()<<linear_y;

}

void MainWindow::on_btn_add_clicked()
{
    addPoint(ui->bx_x->value(), ui->bx_y->value());
    plot();
}

void MainWindow::on_btn_clear_clicked()
{
    clearData();
    plot();

}

void MainWindow::clickedGraph(QMouseEvent *event)
{
    // add first point
    addPoint(ui->plot->xAxis->pixelToCoord(c_point.x()), ui->plot->yAxis->pixelToCoord(c_point.y()));
    plot();

    drawing = 1;
}

void MainWindow::clickedGraphRelease(QMouseEvent *event)
{
    drawing = 0;
    // take only the top points to make it a function
    // interpolate the drawn line onto x axis
    update_linear_t();
    clearData();
    ui->plot->graph(1)->setData(linear_t, linear_y);
    plot();
}


void MainWindow::onGraph(QMouseEvent *event)
{
    c_point = event->pos();
    if (drawing == 1){
        // qDebug() << c_point.x();
        // qDebug() << c_point.y();
        addPoint(ui->plot->xAxis->pixelToCoord(c_point.x()), ui->plot->yAxis->pixelToCoord(c_point.y()));
        plot();
    }
}
