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

#include <QColor>
#include <QPoint>
#include <QSize>
#include <QTime>

namespace Qtilities {

namespace Default {
static constexpr bool   autostart      {false};
static constexpr int    backlightMin   {35};
static constexpr int    backlightMax   {100};
static constexpr int    brightnessMin  {backlightMin};
static constexpr int    brightnessMax  {backlightMax};
static constexpr int    temperatureMin {3200};
static constexpr int    temperatureMax {6500};
static constexpr QPoint position       {200, 200};
static constexpr QSize  size           {480, 320};
static const     QTime  timeStart      {6, 0};
static const     QTime  timeEnd        {16, 0};
} // namespace Default

class Settings
{
public:
    Settings();

    void load();
    void save();

    bool autostart() const { return autostart_; }
    void setAutostart(bool autostart) { autostart_ = autostart; }

    QPoint position() const { return position_; }
    void setPosition(const QPoint& position) { position_ = position; }

    QSize size() const { return size_; }
    void setSize(const QSize& size) { size_ = size; }

    int backlight() const { return backlight_; }
    void setBacklight(int backlight);

    int brightness() const { return brightness_; }
    void setBrightness(int brightness);

    int temperature() const { return temperature_; }
    void setTemperature(int temperature);

    QTime timeStart() const { return timeStart_; }
    void setTimeStart(QTime time);

    QTime timeEnd() const { return timeEnd_; }
    void setTimeEnd(QTime time);

private:
    bool   autostart_;
    int    backlight_;
    int    brightness_;
    int    temperature_;
    QPoint position_;
    QSize  size_;
    QTime  timeStart_;
    QTime  timeEnd_;
};
} // namespace Qtilities
