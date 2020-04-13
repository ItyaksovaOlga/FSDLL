#include <Windows.h>
#include <stdio.h>
#include "pch.h"
#include <iostream>
#include "FSDLL.h" 

using namespace std;

void PrintInfo(NTFS pBoot)
{
    cout << "Sector size: " << pBoot.bytesPerSector << endl;
    cout << "Sectors per cluster: " << pBoot.sectorsPerCluster << endl;
    cout << "Volume descriptor: " << pBoot.mediaDescriptor << endl;
    cout << "Sectors per track: " << pBoot.sectorsPerTrack << endl;
    cout << "Number of heads: " << pBoot.numberOfHeads << endl;
    cout << "Number of sectors: " << pBoot.numOfSectorsInTheVol << endl;
    cout << "First cluster of MFT: " << pBoot.LCNofVCNofTheMFT << endl;
    cout << "First cluster of MFT mirror: " << pBoot.LCNofVCNofTheMFTMirr << endl;
    cout << "Clusters per MFT Record: " << pBoot.clustersPerMFTrecord << endl;
    cout << "Clusters per Index Record: " << pBoot.clustersPerIndexRecord << endl;
    cout << "Volume serial number: " << pBoot.volumeSerialNumber << endl;
}


string getNameFS(string fsName)
{
    char volumeNameBuffer[MAX_PATH];
    char fileSystemNameBuffer[MAX_PATH];
    DWORD volumeSerialNumber;
    DWORD maximumComponentLength;
    DWORD fileSystemFlags;

    string rootPathName = fsName + ":\\";

    //GetVolumeInformation bзвлекает информацию о файловой системе
    if (GetVolumeInformationA(
        rootPathName.c_str(),			//Указатель на строку, которая содержит корневой каталог тома, который будет описан
        volumeNameBuffer,				//Указатель на буфер, который получает имя указанного тома
        sizeof(volumeNameBuffer),		//Длина буфера имени тома
        &volumeSerialNumber,			//Указатель на переменную, которая получает серийный номер тома
        &maximumComponentLength,		//Компонент имени файла - это часть имени файла между обратными косыми чертами
        &fileSystemFlags,				//Указатель на переменную, которая получает флаги, связанные с указанной файловой системой
        fileSystemNameBuffer,			//Указатель на буфер, который получает имя файловой системы
        sizeof(fileSystemNameBuffer)	//Длина буфера имени файловой системы
    ))
    {
        cout << "File system is: " << fileSystemNameBuffer << endl;
        string fileSystemName = fileSystemNameBuffer;
        return fileSystemName;
    }

}

//HANDLE - дескриптор, т.е. число, с помощью которого можно идентифицировать ресурс
//В операционной системе Windows для получения дескриптора файла его необходимо открыть функцией CreateFile 
//В Windows все объекты идентифицируются 32-разрядным целым числом, которое называется дескриптором

bool GetInfo(string rootPathName, NTFS* pBoot)
{
    HANDLE fileHandle = CreateFileA(
        rootPathName.c_str(),           //конвертируем имя файла из string для функции
        GENERIC_READ,                       //Запрашиваемый доступ для чтения к файлу или устройству
        //Позволяет последующим операциям открытия файла или устройства запрашивать доступ для чтения/записи/удаления
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,                      //Открывает файл или устройство, только если оно существует
        0,
        NULL
    );

    //параметр для недопустимого значения дескриптора
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        perror("Invalid handle value!!!");
    }

    //Позиционирование на нулевом смещении
    LARGE_INTEGER sectorOffset;
    sectorOffset.QuadPart = 0;

    //Перемещает указатель файла
    unsigned long currentPosition = SetFilePointer(
        fileHandle,                                     // дескриптор файла
        sectorOffset.LowPart,                           //байты перемещения указателя
        &sectorOffset.HighPart,                         //байты перемещения указателя
        FILE_BEGIN                                      //Начальная точка - ноль или начало файла.
    );

    if (currentPosition != sectorOffset.LowPart)
    {
        perror("Error!!!");
    }

    //Выделение памяти в стеке
    BYTE dataBuffer[1024];

    //Результат считывания байтов
    DWORD bytesRead;

    //чтение
    bool readResult = ReadFile(
        fileHandle,
        dataBuffer,
        1024,
        &bytesRead,
        NULL
    );

    if (!readResult || bytesRead != 1024)
    {
        perror("Error!!!! ");
    };

    //приведение типов
    pBoot = reinterpret_cast<NTFS*>(dataBuffer);

    CloseHandle(fileHandle);

    return true;
};