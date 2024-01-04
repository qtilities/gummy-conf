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
#include "settings.hpp"
#include "qtilities.hpp"

#include <QApplication>
#include <QSettings>

Qtilities::Settings::Settings()
    : bgColor_(Default::backgroundColor)
    , fgColor_(Default::foregroundColor)
    , position_(Default::position)
    , size_(Default::size)
{
}

void Qtilities::Settings::load()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QApplication::organizationName(),
                       QApplication::applicationDisplayName());

    settings.beginGroup("General");
    bgColor_ = settings.value(QSL("BackgroundColor"), Default::backgroundColor).value<QColor>();
    fgColor_ = settings.value(QSL("ForegroundColor"), Default::foregroundColor).value<QColor>();
    position_ = settings.value(QSL("Position"), Default::position).toPoint();
    size_ = settings.value(QSL("Size"), Default::size).toSize();
    settings.endGroup();
}

void Qtilities::Settings::save()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QApplication::organizationName(),
                       QApplication::applicationDisplayName());

    settings.beginGroup("General");
    settings.setValue(QSL("BackgroundColor"), bgColor_);
    settings.setValue(QSL("ForegroundColor"), fgColor_);
    settings.setValue(QSL("Position"), position_);
    settings.setValue(QSL("Size"), size_);
    settings.endGroup();
}
