#ifndef SDES_H___
#define SDES_H___

#include "bgc_aes.h"

class SDES
{

private:
    SDES(char *key);
    static SDES* instance;
public:
    friend class free_instance_for_SDES;
    static SDES* getInstance();

	void Encryption(char*& data,unsigned long size);
	void Decryption(char*& data,unsigned long size);
    ~SDES();
private:
	TAesClass *aes;
	char safe_key[32];
};

#endif // SDES

