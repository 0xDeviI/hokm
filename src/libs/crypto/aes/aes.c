/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Tuesday, April 2nd 2024, 12:06:13 am
 * Author: Armin Asefi
 * 
 * This license agreement (the "License") is a legal agreement between 
 * you (an individual or entity) and Armin Asefi (the "Licensor") 
 * regarding the use of the software provided by the Licensor.
 * 
 * By accessing, viewing, or sharing the source code of the software
 * provided by the Licensor, you agree to be bound by the terms and 
 * conditions of this License.
 * 
 * 1. GRANT OF LICENSE
 * SUBJECT TO THE TERMS AND CONDITIONS OF THIS LICENSE, THE LICENSOR 
 * GRANTS YOU A NON-EXCLUSIVE, NON-TRANSFERABLE, ROYALTY-FREE LICENSE
 * TO ACCESS, VIEW, AND SHARE THE SOURCE CODE OF THE SOFTWARE PROVIDED
 * BY THE LICENSOR.
 * 
 * 2. RESTRICTIONS
 * YOU ARE NOT ALLOWED TO MODIFY, DISTRIBUTE, SUBLICENSE, OR USE THE
 * SOURCE CODE FOR ANY COMMERCIAL PURPOSES WITHOUT THE EXPLICIT WRITTEN
 * PERMISSION OF THE LICENSOR. YOU MAY NOT REVERSE ENGINEER, DECOMPILE,
 * OR DISASSEMBLE THE SOFTWARE.
 * 
 * 3. OWNERSHIP
 * THE LICENSOR RETAINS ALL INTELLECTUAL PROPERTY RIGHTS IN THE SOFTWARE
 * AND THE SOURCE CODE. THIS LICENSE DOES NOT GRANT YOU ANY RIGHTS TO 
 * PATENTS, COPYRIGHTS, TRADE SECRETS, OR ANY OTHER INTELLECTUAL PROPERTY
 * OWNED BY THE LICENSOR.
 * 
 * 4. DISCLAIMER OF WARRANTY
 * THE SOFTWARE IS PROVIDED "AS IS," WITHOUT ANY WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED. THE LICENSOR SHALL NOT BE LIABLE FOR ANY DAMAGES
 * ARISING OUT OF THE USE OR INABILITY TO USE THE SOFTWARE.
 * 
 * 5. TERMINATION
 * THIS LICENSE IS EFFECTIVE UNTIL TERMINATED. THE LICENSOR RESERVES THE
 * RIGHT TO TERMINATE THIS LICENSE AT ANY TIME IF YOU FAIL TO COMPLY WITH
 * ITS TERMS AND CONDITIONS. UPON TERMINATION, YOU MUST CEASE ALL USE OF
 * THE SOFTWARE AND DESTROY ALL COPIES OF THE SOURCE CODE.
 * 
 * 6. GOVERNING LAW
 * THIS LICENSE SHALL BE GOVERNED BY AND CONSTRUED IN ACCORDANCE WITH THE
 * LAWS OF --IRAN, ISLAMIC REPUBLIC OF--. ANY DISPUTES ARISING UNDER THIS LICENSE
 * SHALL BE SUBJECT TO THE EXCLUSIVE JURISDICTION OF THE COURTS IN --IRAN, ISLAMIC REPUBLIC OF--.
 * 
 */


#include "aes.h"

// Function to add PKCS7 padding
void add_pkcs7_padding(uchar *data, int dataLength, int blockSize) {
    int padding = blockSize - (dataLength % blockSize);
    for (int i = 0; i < padding; i++) {
        data[dataLength + i] = padding;
    }
}

// Function to remove PKCS7 padding
void remove_pkcs7_padding(uchar *data, int dataLength) {
    int padding = data[dataLength - 1];
    if (padding > dataLength || padding < 1) {
        // Padding is invalid
        return;
    }
    for (int i = dataLength - padding; i < dataLength; i++) {
        if (data[i] != padding) {
            // Padding is invalid
            return;
        }
    }
    data[dataLength - padding] = '\0';
}

void aes_init(uchar *password, int passwordLength, uchar *salt, uchar *key, uchar *iv) {
    PKCS5_PBKDF2_HMAC_SHA1(password, passwordLength, salt, SALT_LENGTH, ITERATIONS, KEY_LENGTH, key);
    PKCS5_PBKDF2_HMAC_SHA1(password, passwordLength, salt, SALT_LENGTH, ITERATIONS, IV_LENGTH, iv);
}

void encrypt_data(uchar *key, uchar *iv, uchar *data, int dataLength, uchar *encryptedData, int *encryptedDataLength) {
    add_pkcs7_padding(data, dataLength, AES_BLOCK_SIZE);
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, encryptedData, encryptedDataLength, data, dataLength + AES_BLOCK_SIZE);
    EVP_EncryptFinal_ex(ctx, encryptedData + *encryptedDataLength, encryptedDataLength);
    EVP_CIPHER_CTX_free(ctx);
}

void decrypt_data(uchar *key, uchar *iv, uchar *encryptedData, int encryptedDataLength, uchar *decryptedData, int *decryptedDataLength) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_DecryptUpdate(ctx, decryptedData, decryptedDataLength, encryptedData, encryptedDataLength);
    EVP_DecryptFinal_ex(ctx, decryptedData + *decryptedDataLength, decryptedDataLength);
    remove_pkcs7_padding(decryptedData, *decryptedDataLength);
    EVP_CIPHER_CTX_free(ctx);
}