//BB Notifier Server - server for receiving call notifications from phones
//Copyright (C) 2014  Green Snake Software

//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "AESWrapper.h"
#include "Base64.h"
#include "osrng.h"
#include "cryptlib.h"
#include "aes.h"
#include "modes.h"
#include "time.h"

//unsigned char CAESWrapper::iv[CryptoPP::AES::BLOCKSIZE ] = {0x12,0x23,0xFA,0xAB,0x33,0x28,0x78,0x11,0x1a,0x39,0xEC,0x67,0x2D,0x29,0x92,0x74};

// Decrypting incoming data
QString CAESWrapper::Decrypt(char *buf, int length,AESKey key, unsigned char* iv){

    // Decryption
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption
        Decryptor( key.Data, sizeof(key), iv );

    std::string decryptedText;

    try
    {
            CryptoPP::StringSource(std::string (buf, length), true,
                new CryptoPP::StreamTransformationFilter( Decryptor,
                    new CryptoPP::StringSink( decryptedText )
                ) // StreamTransformationFilter
            ); // StringSink

    }
    catch(...)
    {
        return "";
    }

    return QString::fromUtf8(decryptedText.c_str());
}

// Generating new Key
AESKey CAESWrapper::GenKey()
{
    AESKey aesKey;
    CryptoPP::AutoSeededRandomPool rnp(false,16);

    time_t t;
    time(&t);
    for(int i=0;i<(t%5);i++)
    {
        rnp.Reseed(false, 16);
    }

    rnp.GenerateBlock(aesKey.Data,16);
    return aesKey;
}

QString AESKey::ToString()
{

    char tmp[16];
    for (int i = 0; i<16; i++){
        tmp[i]=Data[i];
    }

    std::string text(tmp,sizeof(tmp));

    std::string result = "";
    CryptoPP::StringSource(text,true,new CryptoPP::Base64Encoder(new CryptoPP::StringSink(result)));
    return QString::fromStdString(result.substr(0,4)+"-"+result.substr(4,4)+"-"+result.substr(8,4)+"-"+result.substr(12,4)+"-"+result.substr(16,4)+"-"+result.substr(20,2));
}
