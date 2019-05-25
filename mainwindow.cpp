#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    qDebug()<<"set lineat data";
    qDebug()<<qv_x;
    qDebug()<<qv_y;
    qDebug()<<linear_t;
    qDebug()<<linear_y;

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
