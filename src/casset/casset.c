/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Saturday, March 30th 2024, 9:21:06 pm
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


#include "casset.h"


uchar CAS_V1_HEADER[SIZE_OF_CAS_V1_HEADER] = {
    0x0, 0x0, 0x0, 0x0, 
    0x0, 0x0, 0x0, 0x80, 
    0x86, 0x43, 0x41, 0x53, 
    0x56, 0x31, 0x80, 0x86
};


uchar get_file_from_cas(uchar *file_id, uchar size_of_file_id, uchar *buffer) {
    if (!is_cas_valid())
        return 0;

    // defining aes features
    uchar key[KEY_LENGTH];
    uchar iv[IV_LENGTH];
    uchar salt[SALT_LENGTH];
    uchar password[KEY_SIZE / 4];
    get_mspc_key(password);

    uchar cas_file_path[SYS_MAX_EXE_PATH_LENGTH];
    get_cas_file_path(cas_file_path, sizeof(cas_file_path));

    ushort index = CAS_V1_JUNK_SIZE + SIZE_OF_CAS_V1_HEADER + CAS_V1_AES_PAD_SIZE;

    // reading cas file
    // here
    int cas_file_size = get_file_size(cas_file_path);
    FILE *cas_fp = fopen(cas_file_path, "rb");
    int aes_file_size = cas_file_size - (index + CAS_V1_JUNK_SIZE + CAS_V1_AES_PAD_SIZE);
    int aes_payload_size = aes_file_size - (sizeof(salt) + sizeof(iv));

    // copying salt
    if (!copy_n_bytes(salt, index + aes_file_size - (sizeof(iv) + sizeof(salt)), sizeof(salt), cas_fp)) {
        fclose(cas_fp);
        return 4;
    }
    
    // copying IV
    if (!copy_n_bytes(iv, index + aes_file_size - sizeof(iv), sizeof(iv), cas_fp)) {
        fclose(cas_fp);
        return 4;
    }

    aes_init(password, strlen(password), salt, key, iv);
    uchar *cas_file_decrypted = (uchar *) malloc(aes_payload_size);
    uchar *cas_file_aes_portion = (uchar *) malloc(aes_payload_size);
    
    if (!copy_n_bytes(cas_file_aes_portion, index, aes_payload_size, cas_fp)) {
        fclose(cas_fp);
        free(cas_file_decrypted);
        free(cas_file_aes_portion);
        return 4;
    }
if (!copy_n_bytes(salt, index + aes_file_size - (sizeof(iv) + sizeof(salt)), sizeof(salt), cas_fp)) {
        fclose(cas_fp);
        return 4;
    }
    
    // copying IV
    if (!copy_n_bytes(iv, index + aes_file_size - sizeof(iv), sizeof(iv), cas_fp)) {
        fclose(cas_fp);
        return 4;
    }
    printf("d-key: %s\n", key);
    decrypt_data(key, iv, cas_file_aes_portion, aes_payload_size, cas_file_decrypted, &aes_payload_size);
    aes_payload_size = aes_file_size - (sizeof(salt) + sizeof(iv));

    // for (int i = 0; i < aes_payload_size; i++) {
    //     printf("char: %d\n", cas_file_decrypted[i]);
    // }

    FILE *test = fopen("test.out", "wb+");
    fwrite(cas_file_decrypted, aes_payload_size, 1, test);
    fclose(test);

}


void get_cas_file_path(uchar *cas_file_path, ushort size_of_cas_file_path) {
    get_cwd_path(cas_file_path, size_of_cas_file_path);
    strcat(cas_file_path, CAS_V1_FILE_NAME);
}


uchar is_cas_exists(void) {
    uchar cas_file_path[SYS_MAX_EXE_PATH_LENGTH];
    get_cas_file_path(cas_file_path, sizeof(cas_file_path));
    return access(cas_file_path, F_OK) == 0;
}


uchar is_cas_valid(void) {
    if (!is_cas_exists())
        return 0;

    uchar cas_file_path[SYS_MAX_EXE_PATH_LENGTH];
    get_cas_file_path(cas_file_path, sizeof(cas_file_path));
    ullong cas_file_size = get_file_size(cas_file_path);

    FILE *cas_fp = fopen(cas_file_path, "rb");
    if (cas_fp == NULL)
        return 0;

    if (cas_file_size < CAS_V1_DEFAULT_LENGTH) {
        fclose(cas_fp);
        return 0;
    }
    else {
        uchar cas_header[16];
        ullong index = CAS_V1_JUNK_SIZE;
        if (!copy_n_bytes(cas_header, index, SIZE_OF_CAS_V1_HEADER, cas_fp)) {
            fclose(cas_fp);
            return 0;
        }

        for (int i = 0; i < 16; i++) {
            printf("%d ", cas_header[i]);
        }
        printf("\n");
        for (int i = 0; i < 16; i++) {
            printf("%d ", CAS_V1_HEADER[i]);
        }
        printf("\n");

        if (memcmp(cas_header, CAS_V1_HEADER, SIZE_OF_CAS_V1_HEADER) != 0) {
            fclose(cas_fp);
            return 0;
        }
        else {
            // validating AES f-padding
            char aes_pad[CAS_V1_AES_PAD_SIZE];
            if (!copy_n_bytes(aes_pad, index + SIZE_OF_CAS_V1_HEADER, CAS_V1_AES_PAD_SIZE, cas_fp)) {
                fclose(cas_fp);
                return 0;
            }
            for (uchar i = 0; i < CAS_V1_AES_PAD_SIZE; i++)
                if (aes_pad[i] != '\x0') {
                    fclose(cas_fp);
                    return 0;
                }


            // validating AES l-padding
            if (!copy_n_bytes(aes_pad, cas_file_size - CAS_V1_JUNK_SIZE - CAS_V1_AES_PAD_SIZE, CAS_V1_AES_PAD_SIZE, cas_fp)) {
                fclose(cas_fp);
                return 0;
            }
            for (uchar i = 0; i < CAS_V1_AES_PAD_SIZE; i++)
                if (aes_pad[i] != '\x0') {
                    fclose(cas_fp);
                    return 0;
                }

            fclose(cas_fp);
            return 1;
        }
    }
}


void generate_cas(void) {
    // defining index as last index of first junk data.
    ullong index = CAS_V1_JUNK_SIZE;

    // initialize cas content and feeding it with first 1024 junk data
    char cas_file_content[CAS_V1_DEFAULT_LENGTH + 1];
    FILE *urandom_fp = fopen(CAS_V1_CSPRNG, "r");
    fread(&cas_file_content, 1, index, urandom_fp);

    // writing header
    for (uchar n = 0; n < SIZE_OF_CAS_V1_HEADER; n++) {
        cas_file_content[index++] = CAS_V1_HEADER[n];
    }

    // adding 128 zeros for AES f-l padding (first, last padding)
    for (uchar n = CAS_V1_AES_PAD_SIZE * 2; n > 0; n--) {
        cas_file_content[index++] = '\0';
    }

    // finally, feed last 1024 bytes
    fread(cas_file_content + index, 1, CAS_V1_JUNK_SIZE, urandom_fp);
    fclose(urandom_fp);


    uchar cas_file_path[SYS_MAX_EXE_PATH_LENGTH];
    get_cas_file_path(cas_file_path, sizeof(cas_file_path));

    FILE* cas_fp = fopen(cas_file_path, "wb+");
    
    if (cas_fp == NULL) {
        exit(0);
    }

    fwrite(cas_file_content, CAS_V1_DEFAULT_LENGTH, 1, cas_fp);
    fclose(cas_fp);
}


void init_cas_bundle(void) {
    if (!is_cas_valid())
        generate_cas();
}


void init_casset(void) {
    init_cas_bundle();
}


uchar copy_n_bytes(uchar *out, ullong offset, ullong size, FILE *fp) {
    if (fseek(fp, offset, SEEK_SET) < 0) {
        return 0;
    }
    fread(out, 1, size, fp);
    return 1;
}


void create_aes_payload_buffer(uchar *file_content, ullong size_of_file, uchar *file_id, ullong size_of_file_id, ullong offset, FILE *fp) {
    // feeding first 128 bytes (asset f-padding)
    for (ushort i = 0; i < CAS_V1_ASSET_PADDING; i++)
        file_content[offset + i] = '\x0c';

    printf("first loop passed!\n");

    fread(file_content + CAS_V1_ASSET_PADDING + offset, 1, size_of_file, fp);

    // feeding last 128 bytes (asset l-padding)
    for (ushort i = 0; i < CAS_V1_ASSET_PADDING; i++)
        file_content[offset + size_of_file + CAS_V1_ASSET_PADDING + i] = '\x0c';
        
    printf("second loop passed!\n");

    // adding file name
    for (uchar i = 0; i < size_of_file_id; i++)
        file_content[offset + size_of_file + (CAS_V1_ASSET_PADDING * 2) + i] = file_id[i];

    printf("third loop passed!\n");

    for (uchar i = 0; i < CAS_V1_ASSET_SEP_LENGTH; i++)
        file_content[offset + size_of_file + (CAS_V1_ASSET_PADDING * 2) + size_of_file_id + i] = '\x19';

    printf("fourth loop passed!\n");
}


/*
 * tries to insert a file into casset
 * when returns 0, file_path appended.
 * when returns 1, casset file doesn't exist.
 * when returns 2, file_path doesn't exist.
 * when returns 3, size_of_file_id exceeds.
 * when returns 4, failds while performing operations on files.
 */
uchar insert_file_to_casset(uchar *file_id, uchar size_of_file_id, uchar *file_path) {
    if (!is_cas_valid()) {
        return 1;
    }

    if (access(file_path, F_OK | R_OK) != 0)
        return 2;

    if (size_of_file_id > CAS_V1_MAX_FILE_ID_LENGTH)
        return 3;

    uchar cas_file_path[SYS_MAX_EXE_PATH_LENGTH];
    get_cas_file_path(cas_file_path, sizeof(cas_file_path));
    
    // reading cas file
    ullong cas_file_size = get_file_size(cas_file_path);
    FILE *cas_fp = fopen(cas_file_path, "rb");

    // reading file_path
    ullong size_of_file = get_file_size(file_path);
    FILE *fp = fopen(file_path, "rb");

    // cloning first 1104 bytes (1024 junks + 16 header + 64 AES f-padding)
    ushort index = CAS_V1_JUNK_SIZE + SIZE_OF_CAS_V1_HEADER + CAS_V1_AES_PAD_SIZE;
    uchar cas_file_first_portion[index];
    if (!copy_n_bytes(cas_file_first_portion, 0, index, cas_fp)) {
        fclose(cas_fp);
        fclose(fp);
        return 4;
    }
    
    // calculating size of AES payload bytes size
    int aes_file_size = cas_file_size - (index + CAS_V1_JUNK_SIZE + CAS_V1_AES_PAD_SIZE);
    int aes_payload_size;
    uchar *cas_file_aes_portion;

    // defining aes features
    uchar key[KEY_LENGTH];
    uchar iv[IV_LENGTH];
    uchar salt[SALT_LENGTH];
    uchar password[KEY_SIZE / 4];
    get_mspc_key(password);

    if (aes_file_size == 0) {
        // creating new file and encrypting it
        aes_payload_size = size_of_file + (CAS_V1_ASSET_PADDING * 2) + CAS_V1_ASSET_SEP_LENGTH + size_of_file_id;
        uchar *file_content = (uchar *) malloc(aes_payload_size);
        create_aes_payload_buffer(file_content, size_of_file, file_id, size_of_file_id, 0, fp);


        RAND_bytes(iv, IV_LENGTH);
        RAND_bytes(salt, SALT_LENGTH);
        aes_init(password, strlen(password), salt, key, iv);

        int file_encryption_size = aes_payload_size;
        // cas_file_aes_portion = (uchar *) malloc(aes_payload_size);
        // memcpy(cas_file_aes_portion, file_content, aes_payload_size);
        cas_file_aes_portion = (uchar *) malloc(aes_payload_size + sizeof(iv) + sizeof(salt));
        encrypt_data(key, iv, file_content, aes_payload_size, cas_file_aes_portion, &file_encryption_size);
        memcpy(cas_file_aes_portion + aes_payload_size, salt, sizeof(salt));
        memcpy(cas_file_aes_portion + aes_payload_size + sizeof(salt), iv, sizeof(iv));

        for (int i = 0; i < sizeof(iv); i++)
            printf("iv[%d]: %d\t\taes-iv[%d]: %d\n", i, iv[i], i, *(cas_file_aes_portion + aes_payload_size + sizeof(salt) + i));

        for (int i = 0; i < sizeof(salt); i++)
            printf("salt[%d]: %d\t\taes-salt[%d]: %d\n", i, salt[i], i, *(cas_file_aes_portion + aes_payload_size + i));

        free(file_content);
    }
    else {
        int decrypted_data_size = aes_file_size - (sizeof(iv) + sizeof(salt));
        uchar *cas_file_decrypted = (uchar *) malloc(decrypted_data_size);
        cas_file_aes_portion = (uchar *) malloc(decrypted_data_size);
        if (!copy_n_bytes(cas_file_aes_portion, index, decrypted_data_size, cas_fp))
            return 4;

        // copying salt
        if (!copy_n_bytes(salt, index + aes_file_size - (sizeof(iv) + sizeof(salt)), sizeof(salt), cas_fp))
            return 4;

        // copying IV
        if (!copy_n_bytes(iv, index + aes_file_size - sizeof(iv), sizeof(iv), cas_fp))
            return 4;

        aes_init(password, strlen(password), salt, key, iv);
        decrypt_data(key, iv, cas_file_aes_portion, decrypted_data_size, cas_file_decrypted, &decrypted_data_size);
        decrypted_data_size = aes_file_size - (sizeof(iv) + sizeof(salt));
        free(cas_file_aes_portion);

        // for (int i = 0; i < decrypted_data_size; i++)
        //     printf("char: %d\n", cas_file_decrypted[i]);
        // exit(0);

        aes_payload_size = decrypted_data_size + size_of_file + (CAS_V1_ASSET_PADDING * 2) + CAS_V1_ASSET_SEP_LENGTH + size_of_file_id;
        cas_file_decrypted = (uchar *) realloc(cas_file_decrypted, aes_payload_size);
        create_aes_payload_buffer(cas_file_decrypted, size_of_file, file_id, size_of_file_id, decrypted_data_size, fp);

        RAND_bytes(iv, IV_LENGTH);
        RAND_bytes(salt, SALT_LENGTH);
        aes_init(password, strlen(password), salt, key, iv);

        int file_encryption_size = aes_payload_size;
        cas_file_aes_portion = (uchar *) malloc(aes_payload_size + sizeof(iv) + sizeof(salt));
        encrypt_data(key, iv, cas_file_decrypted, aes_payload_size, cas_file_aes_portion, &file_encryption_size);
        memcpy(cas_file_aes_portion + aes_payload_size, salt, sizeof(salt));
        memcpy(cas_file_aes_portion + aes_payload_size + sizeof(salt), iv, sizeof(iv));

        for (int i = 0; i < sizeof(iv); i++)
            printf("re-iv[%d]: %d\t\tre-aes-iv[%d]: %d\n", i, iv[i], i, *(cas_file_aes_portion + aes_payload_size + sizeof(salt) + i));

        for (int i = 0; i < sizeof(salt); i++)
            printf("re-salt[%d]: %d\t\tre-aes-salt[%d]: %d\n", i, salt[i], i, *(cas_file_aes_portion + aes_payload_size + i));

        free(cas_file_decrypted);
    }

    // cloning last 1088 bytes (64 AES l-padding + 1024 junk)
    uchar cas_file_last_portion[CAS_V1_AES_PAD_SIZE + CAS_V1_JUNK_SIZE];
    if (!copy_n_bytes(cas_file_last_portion, index + aes_file_size, sizeof(cas_file_last_portion), cas_fp))
        return 4;
    fclose(cas_fp);

    // updating aes payload size
    aes_payload_size += sizeof(salt) + sizeof(iv);

    // re-writing cas file
    get_cas_file_path(cas_file_path, sizeof(cas_file_path));
    cas_fp = fopen(cas_file_path, "wb+");
    fwrite((void *) cas_file_first_portion, index, 1, cas_fp);
    fseek(cas_fp, index, SEEK_SET);
    fwrite((void *) cas_file_aes_portion, aes_payload_size, 1, cas_fp);
    fseek(cas_fp, index + aes_payload_size, SEEK_SET);
    fwrite((void *) cas_file_last_portion, CAS_V1_AES_PAD_SIZE + CAS_V1_JUNK_SIZE, 1, cas_fp);
    free(cas_file_aes_portion);
    fclose(cas_fp);
    return 0;
}