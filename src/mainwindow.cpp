/*
    MIT License

    Copyright (c) 2021-2023 Andrea Zanellato <redtid3@gmail.com>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/
#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "application.hpp"
#include "dialogabout.hpp"
#include "settings.hpp"

Qtilities::MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Qtilities::Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(ui->layout);
    loadSettings();

    Application *theApp = static_cast<Application *>(qApp);
    connect(ui->actPrefs, &QAction::triggered, theApp, &Application::preferences);
    connect(ui->actAbout, &QAction::triggered, theApp, &Application::about);
    connect(ui->actExit, &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);
}

Qtilities::MainWindow::~MainWindow() { delete ui; }

void Qtilities::MainWindow::loadSettings()
{
    Settings &settings = static_cast<Application *>(qApp)->settings();
    ui->centralwidget->setStyleSheet(
        QString("background-color:rgb(%1,%2,%3);"
                "color:rgb(%4,%5,%6)}")
            .arg(settings.backgroundColor().red())
            .arg(settings.backgroundColor().green())
            .arg(settings.backgroundColor().blue())
            .arg(settings.foregroundColor().red())
            .arg(settings.foregroundColor().green())
            .arg(settings.foregroundColor().blue()));
}

void Qtilities::MainWindow::saveSettings()
{
    Settings &settings = static_cast<Application *>(qApp)->settings();
    settings.setPosition(pos());
    settings.setSize(size());
}
