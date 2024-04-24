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
#include "application.hpp"
#include "maindialog.hpp"
#include "dialogabout.hpp"
#include "qtilities.hpp"
#include "systemtrayicon.hpp"

#include <QLibraryInfo>
#include <QLocale>
#include <QMenu>
#include <QProcess>

Qtilities::Application::Application(int argc, char *argv[])
    : QApplication(argc, argv)
    , trayIcon_(new SystemTrayIcon(this))
    , gummyd_(new GummyD)
{
    // UseHighDpiPixmaps is default from Qt6
#if QT_VERSION < 0x060000
    setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
    setApplicationName(PROJECT_ID);
    setApplicationDisplayName(APPLICATION_NAME);
    setOrganizationName(ORGANIZATION_NAME);
    setOrganizationDomain(ORGANIZATION_DOMAIN);
    setDesktopFileName(PROJECT_APPSTREAM_ID);

    setQuitOnLastWindowClosed(false);

    initLocale();
    initUi();
}

void Qtilities::Application::initLocale()
{
#if PROJECT_TRANSLATION_TEST_ENABLED
    QLocale locale(QLocale(PROJECT_TRANSLATION_TEST));
    QLocale::setDefault(locale);
#else
    QLocale locale = QLocale::system();
#endif
    // install the translations built-into Qt itself
    if (qtTranslator_.load(QStringLiteral("qt_") + locale.name(),
#if QT_VERSION < 0x060000
                           QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
#else
                           QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
#endif
        installTranslator(&qtTranslator_);

    // E.g. "<appname>_en"
    QString translationsFileName = QCoreApplication::applicationName().toLower() + '_' + locale.name();
    // Try first in the same binary directory, in case we are building,
    // otherwise read from system data
    QString translationsPath = QCoreApplication::applicationDirPath();

    bool isLoaded = translator_.load(translationsFileName, translationsPath);
    if (!isLoaded) {
        // "/usr/share/<appname>/translations
        isLoaded = translator_.load(translationsFileName,
                                    QStringLiteral(PROJECT_DATA_DIR) + QStringLiteral("/translations"));
    }
    if (isLoaded)
        installTranslator(&translator_);
}

void Qtilities::Application::initUi()
{
    settings_.load();

    QString icoLocalPath
        = QCoreApplication::applicationDirPath() + '/'
        + QStringLiteral(PROJECT_APPSTREAM_ID) + '.' + QStringLiteral(PROJECT_ICON_FORMAT);
    QString icoSysPath = QStringLiteral(PROJECT_ICON_SYSTEM_PATH);

    // Try first to find the app icon in the current/build directory
    appIcon_ = QIcon(icoLocalPath);
    if (appIcon_.isNull())
        appIcon_ = QIcon(icoSysPath);

    mainDialog_ = new Qtilities::MainDialog;
    mainDialog_->move(settings_.position());
    mainDialog_->resize(settings_.size());
    mainDialog_->setWindowTitle(applicationDisplayName());
    mainDialog_->show();
    mainDialog_->hide();

    if(settings_.autostart() && !gummyd_->is_running())
        gummyd_->start();

    if(gummyd_->is_running()) {
        QIcon icon = QIcon(QSL(":/sun"));
        trayIcon_->setIcon(icon);
        mainDialog_->setWindowIcon(icon);
        mainDialog_->setIsRunning(true);
    } else {
        trayIcon_->setIcon(appIcon_);
        mainDialog_->setWindowIcon(appIcon_);
        mainDialog_->setIsRunning(false);
    }
    trayIcon_->setWidget(mainDialog_);

    QMenu*   trayMenu = trayIcon_->menu();
    QAction* before   = trayMenu->actions().at(0);
    actAutoStart_ = new QAction(tr("Auto&start"), trayMenu);
    actAutoStart_->setCheckable(true);
    actAutoStart_->setChecked(settings_.autostart());
    trayMenu->insertAction(before, actAutoStart_);

    connect(this, &QApplication::aboutToQuit, mainDialog_, &QObject::deleteLater);
    connect(this, &QApplication::aboutToQuit, this, [this]() {
        mainDialog_->saveSettings();
        settings_.setAutostart(actAutoStart_->isChecked());
        settings_.autostart() ? createAutostartFile() : deleteAutostartFile();
        settings_.save();
    });
    connect(this, &Application::backlightChanged,   this, &Application::onBacklightChanged);
    connect(this, &Application::brightnessChanged,  this, &Application::onBrightnessChanged);
    connect(this, &Application::temperatureChanged, this, &Application::onTemperatureChanged);
    connect(this, &Application::timeStartChanged,   this, &Application::onTimeStartChanged);
    connect(this, &Application::timeEndChanged,     this, &Application::onTimeEndChanged);
}

void Qtilities::Application::onBacklightChanged(int backlight)
{
    QStringList args;
    args << "-b" << QString::number(backlight);
    gummyd_->send_command(args);
}

void Qtilities::Application::onBrightnessChanged(int brightness)
{
    QStringList args;
    args << "-p" << QString::number(brightness);
    gummyd_->send_command(args);
}

void Qtilities::Application::onTemperatureChanged(int temperature)
{
    QStringList args;
    args << "-t" << QString::number(temperature);
    gummyd_->send_command(args);
}

void Qtilities::Application::onTimeStartChanged(QTime)
{
    // TODO: onTimeStartChanged
}

void Qtilities::Application::onTimeEndChanged(QTime)
{
    // TODO: onTimeEndChanged
}

void Qtilities::Application::onStartStop()
{
    if(gummyd_->is_running()) {
        gummyd_->stop();
        trayIcon_->setIcon(appIcon_);
        mainDialog_->setWindowIcon(appIcon_);
        mainDialog_->setIsRunning(false);
    } else {
        gummyd_->start();
        trayIcon_->setIcon(QIcon(QSL(":/sun")));
        mainDialog_->setWindowIcon(QIcon(QSL(":/sun")));
        mainDialog_->setIsRunning(true);
    }
}

void Qtilities::Application::about()
{
    DialogAbout about(mainDialog_);
    about.exec();
}

QMenu* Qtilities::Application::menu() const
{
    return trayIcon_->menu();
}

int main(int argc, char *argv[])
{
    Qtilities::Application app(argc, argv);
    return app.exec();
}
