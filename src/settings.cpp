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
#include "settings.hpp"
#include "qtilities.hpp"

#include "application.hpp"
#include <QSettings>

Qtilities::Settings::Settings()
    : autostart_(Default::autostart)
    , backlight_(Default::backlightMax)
    , brightness_(Default::brightnessMax)
    , position_(Default::position)
    , size_(Default::size)
    , temperature_(Default::temperatureMax)
    , timeStart_(Default::timeStart)
    , timeEnd_(Default::timeEnd)
{
}

void Qtilities::Settings::load()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QApplication::organizationName(),
                       QApplication::applicationDisplayName());

    settings.beginGroup("General");

    backlight_ = std::clamp(
        settings.value(QSL("Backlight"), Default::backlightMax).toInt(),
        Default::backlightMin, Default::backlightMax
    );
    brightness_ = std::clamp(
        settings.value(QSL("Brightness"),  Default::brightnessMax).toInt(),
        Default::brightnessMin, Default::brightnessMax
    );
    temperature_ = std::clamp(
        settings.value(QSL("Temperature"), Default::temperatureMax).toInt(),
        Default::temperatureMin, Default::temperatureMax
    );
    autostart_ = settings.value(QSL("Autostart"), Default::autostart).toBool();
    position_  = settings.value(QSL("Position"),  Default::position).toPoint();
    size_      = settings.value(QSL("Size"),      Default::size).toSize();
    timeStart_ = settings.value(QSL("TimeStart"), Default::timeStart).toTime();
    timeEnd_   = settings.value(QSL("TimeStop"),  Default::timeEnd).toTime();
}

void Qtilities::Settings::save()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QApplication::organizationName(),
                       QApplication::applicationDisplayName());

    settings.beginGroup("General");
    settings.setValue(QSL("Autostart"),   autostart_);
    settings.setValue(QSL("Backlight"),   backlight_);
    settings.setValue(QSL("Brightness"),  brightness_);
    settings.setValue(QSL("Position"),    position_);
    settings.setValue(QSL("Size"),        size_);
    settings.setValue(QSL("Temperature"), temperature_);
    settings.setValue(QSL("TimeStart"),   timeStart_);
    settings.setValue(QSL("TimeStop"),    timeEnd_);
    settings.endGroup();
}

void Qtilities::Settings::setBacklight(int backlight)
{
    if (backlight_ == backlight)
        return;

    Application* theApp = static_cast<Application*>(qApp);
    backlight_ = backlight;
    Q_EMIT theApp->backlightChanged(backlight);
}

void Qtilities::Settings::setBrightness(int brightness)
{
    if (brightness_ == brightness)
        return;

    Application* theApp = static_cast<Application*>(qApp);
    brightness_ = brightness;
    Q_EMIT theApp->brightnessChanged(brightness);
}

void Qtilities::Settings::setTemperature(int temperature)
{
    if (temperature_ == temperature)
        return;

    Application* theApp = static_cast<Application*>(qApp);
    temperature_ = temperature;
    Q_EMIT theApp->temperatureChanged(temperature);
}

void Qtilities::Settings::setTimeStart(QTime time)
{
    if (timeStart_ == time)
        return;

    Application* theApp = static_cast<Application*>(qApp);
    timeStart_ = time;
    Q_EMIT theApp->timeStartChanged(time);
}

void Qtilities::Settings::setTimeEnd(QTime time)
{
    if (timeEnd_ == time)
        return;

    Application* theApp = static_cast<Application*>(qApp);
    timeEnd_ = time;
    Q_EMIT theApp->timeEndChanged(time);
}
