/*
    MIT License

    Copyright (c) 2024 Andrea Zanellato <redtid3@gmail.com>

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
#include "gummyd.hpp"

#include <QProcess>

#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <filesystem>

GummyD::GummyD()
{
}

GummyD::~GummyD()
{
}

bool GummyD::is_running() const
{
    std::filesystem::path xdgRuntimePath(std::getenv("XDG_RUNTIME_DIR"));
    if (xdgRuntimePath.empty()) {
        std::cerr << "XDG_RUNTIME_DIR not set.\n";
        return false;
    }
    std::filesystem::path gummydLockPath = xdgRuntimePath / u8"gummyd-lock";
    int fd = open(gummydLockPath.c_str(), O_RDWR | O_CREAT, 0640);
    if (fd < 0) {
        std::cerr << "Failed to open the lock file.\n";
        return false;
    }
    flock fl;
    fl.l_type   = F_WRLCK;  // read/write lock
    fl.l_whence = SEEK_SET; // beginning of file
    fl.l_start  = 0;        // offset from l_whence
    fl.l_len    = 0;        // length, 0 = to EOF
    fcntl(fd, F_GETLK, &fl);

    return fl.l_type != F_UNLCK;
}

void GummyD::start()
{
    QStringList args;
    args << QStringLiteral("start");
    send_command(args);
}

void GummyD::stop()
{
    QStringList args;
    args << QStringLiteral("stop");
    send_command(args);
}

void GummyD::send_command(const QStringList& args)
{
    QProcess::startDetached(QStringLiteral("gummy"), args);
}
