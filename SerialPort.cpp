#include "SerialPort.h"

SerialPort::SerialPort(
    const char* portName)
{
    connected = false;

    hSerial =
        CreateFileA(
            portName,
            GENERIC_READ,
            0,
            0,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            0);

    if (hSerial == INVALID_HANDLE_VALUE)
        return;

    DCB dcbSerialParams = { 0 };

    dcbSerialParams.DCBlength =
        sizeof(dcbSerialParams);

    GetCommState(
        hSerial,
        &dcbSerialParams);

    dcbSerialParams.BaudRate =
        CBR_115200;

    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    SetCommState(
        hSerial,
        &dcbSerialParams);

    connected = true;
}

SerialPort::~SerialPort()
{
    if (connected)
        CloseHandle(hSerial);
}

bool SerialPort::IsConnected()
{
    return connected;
}

int SerialPort::ReadData(
    char* buffer,
    unsigned int size)
{
    DWORD bytesRead;

    if (ReadFile(
        hSerial,
        buffer,
        size,
        &bytesRead,
        NULL))
    {
        return bytesRead;
    }

    return 0;
}