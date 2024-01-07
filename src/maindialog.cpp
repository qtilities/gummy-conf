/*
    MIT License

    Copyright (c) 2021-2024 Andrea Zanellato <redtid3@gmail.com>

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
#include "maindialog.hpp"
#include "ui_maindialog.h"
#include "application.hpp"
#include "settings.hpp"

#include <QColorDialog>
#include <QPushButton>

Qtilities::MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
    , ui_(new Qtilities::Ui::MainDialog)
{
    ui_->setupUi(this);

    // TODO
    ui_->timeStart->setVisible(false);
    ui_->timeEnd->setVisible(false);
    ui_->lblTime->setVisible(false);

    loadSettings();

    Application* theApp = static_cast<Application*>(qApp);
    connect(ui_->buttonBox,    &QDialogButtonBox::accepted, this,   &MainDialog::accept);
    connect(ui_->buttonBox,    &QDialogButtonBox::rejected, this,   &MainDialog::reject);
    connect(ui_->pbnAbout,     &QPushButton::clicked,       theApp, &Application::about);
    connect(ui_->pbnStartStop, &QPushButton::clicked,       theApp, &Application::onStartStop);
}

Qtilities::MainDialog::~MainDialog() { delete ui_; }

void Qtilities::MainDialog::setIsRunning(bool isRunning)
{
    if (isRunning) {
        ui_->pbnStartStop->setIcon(QIcon::fromTheme(QString::fromUtf8("media-playback-stop")));
        ui_->pbnStartStop->setText(tr("Stop"));
        return;
    }
    ui_->pbnStartStop->setIcon(QIcon::fromTheme(QString::fromUtf8("media-playback-start")));
    ui_->pbnStartStop->setText(tr("Start"));
}

void Qtilities::MainDialog::loadSettings()
{
    Settings &settings = static_cast<Application*>(qApp)->settings();
    // move and resize are set in Application
    ui_->sbxBacklight->setValue(settings.backlight());
    ui_->sbxBright->setValue(settings.brightness());
    ui_->sbxTemp->setValue(settings.temperature());
    ui_->timeStart->setTime(settings.timeStart());
    ui_->timeEnd->setTime(settings.timeEnd());
}

void Qtilities::MainDialog::saveSettings()
{
    Settings& settings = static_cast<Application*>(qApp)->settings();
    settings.setPosition(pos());
    settings.setSize(size());
    settings.setBacklight(ui_->sbxBacklight->value());
    settings.setBrightness(ui_->sbxBright->value());
    settings.setTemperature(ui_->sbxTemp->value());
    settings.setTimeStart(ui_->timeStart->time());
    settings.setTimeEnd(ui_->timeEnd->time());
}

void Qtilities::MainDialog::accept()
{
    saveSettings();
    QDialog::accept();
    hide();
}
