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
#pragma once

#include "settings.hpp"
#include "gummyd.hpp"

#include <QApplication>
#include <QIcon>
#include <QTranslator>
#include <QTime>

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
QT_END_NAMESPACE

namespace Qtilities {

class MainDialog;
class SystemTrayIcon;
class Application : public QApplication
{
    Q_OBJECT
    friend class MainDialog;

public:
    Application(int argc, char *argv[]);

    void about();

    QIcon     icon() const { return appIcon_; }
    Settings& settings()   { return settings_; }
    QMenu*    menu() const;

Q_SIGNALS:
    void backlightChanged(int);
    void brightnessChanged(int);
    void temperatureChanged(int);
    void timeStartChanged(QTime);
    void timeEndChanged(QTime);

private:
    void initLocale();
    void initUi();

    void onAboutToQuit();
    void onBacklightChanged(int);
    void onBrightnessChanged(int);
    void onStartStop();
    void onTemperatureChanged(int);
    void onTimeStartChanged(QTime);
    void onTimeEndChanged(QTime);

    MainDialog*             mainDialog_;
    QAction*                actAutoStart_;
    QIcon                   appIcon_;
    QTranslator             qtTranslator_, translator_;
    Settings                settings_;
    SystemTrayIcon*         trayIcon_;
    std::unique_ptr<GummyD> gummyd_;
};
} // namespace Qtilities
