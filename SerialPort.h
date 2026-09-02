#pragma once

#include <windows.h>
#include <string>

class SerialPort
{
public:

    SerialPort(
        const char* portName);

    ~SerialPort();

    bool IsConnected();

    int ReadData(
        char* buffer,
        unsigned int size);

private:

    HANDLE hSerial;
    bool connected;
};
