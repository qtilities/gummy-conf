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
#include "application.hpp"
#include "mainwindow.hpp"
#include "dialogabout.hpp"
#include "dialogprefs.hpp"

#include <QKeySequence>
#include <QLibraryInfo>
#include <QLocale>
#include <QShortcut>

Qtilities::Application::Application(int argc, char *argv[])
    : QApplication(argc, argv)
{
    setApplicationName(PROJECT_ID);
    setApplicationDisplayName(APPLICATION_NAME);
    setOrganizationName(ORGANIZATION_NAME);
    setOrganizationDomain(ORGANIZATION_DOMAIN);

    initLocale();
    initUi();
}

void Qtilities::Application::initLocale()
{
#if 1
    QLocale locale = QLocale::system();
#else
    QLocale locale(QLocale("it"));
    QLocale::setDefault(locale);
#endif
    // Qt translations (buttons and the like)
    QString translationsPath
#if QT_VERSION < 0x060000
        = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#else
        = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
#endif
    QString translationsFileName = QStringLiteral("qt_") + locale.name();

    if (qtTranslator_.load(translationsFileName, translationsPath))
        installTranslator(&qtTranslator_);

    // E.g. "<appname>_en"
    translationsFileName = QCoreApplication::applicationName().toLower() + '_' + locale.name();

    // Try first in the same binary directory, in case we are building,
    // otherwise read from system data
    translationsPath = QCoreApplication::applicationDirPath();

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
    // UseHighDpiPixmaps is default from Qt6
#if QT_VERSION < 0x060000
    setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
    settings_.load();

    QString icoLocalPath
        = QCoreApplication::applicationDirPath() + '/' + QStringLiteral(PROJECT_ICON_NAME);
    QString icoSysPath = QStringLiteral(PROJECT_ICON_SYSTEM_PATH);

    // Try first to find the app icon in the current/build directory
    appIcon_ = QIcon(icoLocalPath);
    if (appIcon_.isNull())
        appIcon_ = QIcon(icoSysPath);

    mainWindow_ = new Qtilities::MainWindow;
    mainWindow_->move(settings_.position());
    mainWindow_->resize(settings_.size());
    mainWindow_->setWindowIcon(appIcon_);
    mainWindow_->setWindowTitle(applicationDisplayName());
    mainWindow_->show();

    connect(this, &QApplication::aboutToQuit, mainWindow_, &QObject::deleteLater);
    connect(this, &QApplication::aboutToQuit, this, [this]() {
        mainWindow_->saveSettings();
        settings_.save();
    });
}

void Qtilities::Application::about()
{
    DialogAbout about(mainWindow_);
    about.exec();
}

void Qtilities::Application::preferences()
{
    DialogPrefs prefs(mainWindow_);
    connect(&prefs, &DialogPrefs::accepted, mainWindow_, &MainWindow::loadSettings);
    prefs.loadSettings();
    prefs.exec();
}

int main(int argc, char *argv[])
{
    Qtilities::Application app(argc, argv);
    return app.exec();
}
