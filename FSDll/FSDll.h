#pragma once

//Making DLL exportable and importable
#ifdef FSDLL_EXPORTS
#define FSDLL_API __declspec(dllexport)
#else
#define FSDLL_API __declspec(dllimport)
#endif

#include <iostream>
#include <windows.h>
#include <stdio.h>
using namespace std;

#pragma pack(1)

typedef struct
{
	BYTE    jump[3];                        //переход на программу начальной загрузки
	BYTE    systemID[8];                    //операционная система и версия
	UINT16  bytesPerSector;                 //размер сектора в байтах
	UINT16  sectorsPerCluster;              //количество секторов в одном кластере
	BYTE    reserv1[6];                     //зарезервировано
	UINT16  mediaDescriptor;                //тип носителя данных
	BYTE    reserv2;                        //зарезервировано
	UINT16  sectorsPerTrack;                //количество секторов на дорожке
	UINT16  numberOfHeads;                  //количество магнитных головок
	BYTE    reserv3[8];                     //зарезервировано
	UINT32  defaultValue;                   //содержит число 0х80
	UINT64  numOfSectorsInTheVol;           //количество секторов
	UINT64  LCNofVCNofTheMFT;               //lcn - логический номер кластера
	UINT64  LCNofVCNofTheMFTMirr;           //vcn - виртуальный номер кластера
	DWORD   clustersPerMFTrecord;           //размер записи mft в кластерах
	UINT32  clustersPerIndexRecord;         //размер буфера индексов в кластерах
	UINT64  volumeSerialNumber;             //серийный номер тома
} NTFS;

#pragma pack()

//External linkage of DLL functions
extern "C++" FSDLL_API bool GetInfo(string rootPathName, NTFS * pBoot);
extern "C++" FSDLL_API void PrintInfo(NTFS pBoot);
extern "C++" FSDLL_API string getNameFS(string fileName);


//test