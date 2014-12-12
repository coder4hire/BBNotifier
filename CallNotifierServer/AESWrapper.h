#ifndef AESWRAPPER_H
#define AESWRAPPER_H

#include "qstring.h"
#include "cryptlib.h"
#include "aes.h"
#include "qvariant.h"

struct AESKey
{
    unsigned char Data[16];
    QString ToString();
    QVariantList ToQVList()
    {
        QVariantList retVal;
        for(int i=0;i<sizeof(Data);i++)
        {
            retVal.push_back(Data[i]);
        }
        return retVal;
    }

    AESKey()
    {
        memset(Data,0,sizeof(Data));
    }
};

class CAESWrapper
{
public:
    static AESKey GenKey();
    static QString Decrypt(char *buf, int length,AESKey key,unsigned char *iv);

protected:
    static unsigned char iv[ CryptoPP::AES::BLOCKSIZE ];
};

#endif // AESWRAPPER_H
