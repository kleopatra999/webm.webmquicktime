// Copyright (c) 2010 The WebM project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.


#include "EbmlWriter.h"
#include <stdlib.h>
#include <wchar.h>
#include <string.h>


void Ebml_WriteLen(EbmlGlobal *glob, long long val)
{
    //TODO check and make sure we are not > than 0x0100000000000000LLU
    unsigned char size = 8; //size in bytes to output
    unsigned long long minVal = 0x00000000000000ffLLU; //mask to compare for byte size

    for (size = 1; size < 8; size ++)
    {
        if (val < minVal)
            break;

        minVal = (minVal << 7);
    }

    val |= (0x000000000000080LLU << ((size - 1) * 7));

    Ebml_Serialize(glob, (void *) &val, size);
}

void Ebml_WriteString(EbmlGlobal *glob, const char *str)
{
    const size_t size_ = strlen(str);
    const unsigned long long  size = size_;
    Ebml_WriteLen(glob, size);
    //TODO: it's not clear from the spec whether the nul terminator
    //should be serialized too.  For now we omit the null terminator.
    Ebml_Write(glob, str, size);
}

void Ebml_WriteUTF8(EbmlGlobal *glob, const wchar_t *wstr)
{
    const size_t strlen = wcslen(wstr);

    //TODO: it's not clear from the spec whether the nul terminator
    //should be serialized too.  For now we include it.
    const unsigned long long  size = strlen;

    Ebml_WriteLen(glob, size);
    Ebml_Write(glob, wstr, size);
}

void Ebml_WriteID(EbmlGlobal *glob, unsigned long class_id)
{
    if (class_id >= 0x01000000)
        Ebml_Serialize(glob, (void *)&class_id, 4);
    else if (class_id >= 0x00010000)
        Ebml_Serialize(glob, (void *)&class_id, 3);
    else if (class_id >= 0x00000100)
        Ebml_Serialize(glob, (void *)&class_id, 2);
    else
        Ebml_Serialize(glob, (void *)&class_id, 1);
}
void Ebml_SerializeUnsigned64(EbmlGlobal *glob, unsigned long class_id, unsigned long long ui)
{
    unsigned char sizeSerialized = 8 | 0x80;
    Ebml_WriteID(glob, class_id);
    Ebml_Serialize(glob, &sizeSerialized, 1);
    Ebml_Serialize(glob, &ui, 8);
}

void Ebml_SerializeUnsigned(EbmlGlobal *glob, unsigned long class_id, unsigned long ui)
{
    unsigned char size = 8; //size in bytes to output
    unsigned char sizeSerialized = 0;
    Ebml_WriteID(glob, class_id);
    unsigned long minVal;
    minVal = 0x7fLU; //mask to compare for byte size

    for (size = 1; size < 4; size ++)
    {
        if (ui < minVal)
        {
            break;
        }

        minVal <<= 7;
    }

    sizeSerialized = 0x80 | size;
    Ebml_Serialize(glob, &sizeSerialized, 1);
    unsigned int _s = size;
    Ebml_Serialize(glob, &ui, size);
}

void Ebml_SerializeFloat(EbmlGlobal *glob, unsigned long class_id, double d)
{
    Ebml_WriteID(glob, class_id);
    unsigned char len = 0x88;
    Ebml_Serialize(glob, &len, 1);
    Ebml_Serialize(glob,  &d, 8);
}

//TODO make this more generic to signed
void Ebml_WriteSigned16(EbmlGlobal *glob, short val)
{
    signed long out = ((val & 0x003FFFFF) | 0x00200000) << 8;
    Ebml_Serialize(glob, &out, 3);
}

void Ebml_SerializeString(EbmlGlobal *glob, unsigned long class_id, const char *s)
{
    Ebml_WriteID(glob, class_id);
    Ebml_WriteString(glob, s);
}

void Ebml_SerializeUTF8(EbmlGlobal *glob, unsigned long class_id, wchar_t *s)
{
    Ebml_WriteID(glob,  class_id);
    Ebml_WriteUTF8(glob,  s);
}

void Ebml_SerializeData(EbmlGlobal *glob, unsigned long class_id, unsigned char *data, unsigned long data_length)
{
    unsigned char size = 4;
    Ebml_WriteID(glob, class_id);
    Ebml_WriteLen(glob, data_length);
    Ebml_Write(glob,  data, data_length);
}

void Ebml_WriteVoid(EbmlGlobal *glob, unsigned long vSize)
{
    Ebml_WriteID(glob, 0xEC);
    unsigned char tmp = 0;
    unsigned long i = 0;
    Ebml_WriteLen(glob, vSize);

    for (i = 0; i < vSize; i++)
    {
        Ebml_Write(glob, &tmp, 1);
    }
}

//TODO Serialize Date
