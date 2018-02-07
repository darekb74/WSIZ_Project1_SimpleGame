#ifndef MAIN_H
#define MAIN_H
#pragma once

using namespace System;

const Int16 consoleWidth = 122;
const Int16 consoleHeight = 50;

#define M_PI 3.14159
#define W_KEY "NHGBE1QNERX2OEMRX3-4QYN5JFVM6PBCREAVHF"
#define MAGIC_NUMBER 71830

inline void WriteAt(String^,  int,  int );
inline void setColor(Int16);

const Int16 mLINES=20;

//const UInt16 tbCol[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
const UInt16 tabCol[16] = {0,8,7,15,14,6,2,10,11,3,1,9,12,4,5,13};

#endif
