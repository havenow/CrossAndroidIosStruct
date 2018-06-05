
#include "sdes.h"
#include <stdio.h>
#include <string.h>

SDES::SDES(char *key)  //Initializes the object with 10-bits key
{
	memset(safe_key,0,32);
	int len = strlen(key);
	memcpy(safe_key,key,len);
	aes = new TAesClass;
	aes->InitializePrivateKey(16, (unsigned char*)safe_key); //进行初始化
}

/*Destructor*/
SDES::~SDES()
{
	delete aes;
}

void SDES::Encryption(char*& data,unsigned long size)
{
	unsigned char* miwen = new unsigned char[size*2];
	memset(miwen,0,size*2);
	int out_size = aes->OnAesEncrypt((unsigned long*)data, size, (unsigned long*)miwen); //进行加密
	memcpy(data,miwen,size);
	delete [] miwen;
}

void SDES::Decryption(char*& data,unsigned long size)
{
	unsigned char* mingwen = new unsigned char[size];
	memset(mingwen,0,size);
	aes->OnAesUncrypt((unsigned long*)data, size, (unsigned long*)mingwen); //进行解密
	memcpy(data,mingwen,size);
	delete [] mingwen;
}

//在此处初始化
SDES* SDES::instance = 0;

SDES* SDES::getInstance()
{
    if(!instance)
    {
		FILE* fp = fopen("key.txt","rt");
		char data[1024];
		fgets(data,1024,fp);
fclose(fp);
        instance = new SDES(data);
    }
    return instance;
}

class free_instance_for_SDES
{
public:
    free_instance_for_SDES()
    {
    }
    ~free_instance_for_SDES()
    {
        if(SDES::instance) delete SDES::instance;
    }
};

static free_instance_for_SDES free_fingleton;
