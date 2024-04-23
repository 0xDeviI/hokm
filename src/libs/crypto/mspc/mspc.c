
/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Sunday, March 31st 2024, 1:32:48 am
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


#include "mspc.h"


void get_mspc_file_path(uchar *mspc_file_path, ushort size_of_mspc_file_path) {
    get_cwd_path(mspc_file_path, size_of_mspc_file_path);
    strcat(mspc_file_path, MSPC_2080V_FILE_NAME);
}


uchar is_mspc_exists(void) {
    uchar mspc_file_path[SYS_MAX_EXE_PATH_LENGTH];
    get_mspc_file_path(mspc_file_path, sizeof(mspc_file_path));
    return access(mspc_file_path, F_OK) == 0;
}


void get_mspc_key(uchar *key) {
    char key_size = SHA256_HASH_SIZE * 2;
    char mspc_file_content[MSPC_2080V_FILE_LENGTH];
    char mspc_file_path[SYS_MAX_EXE_PATH_LENGTH];
    get_mspc_file_path(mspc_file_path, sizeof(mspc_file_path));
    
    FILE* mspc_fp;
    mspc_fp = fopen(mspc_file_path, "rb");
    
    if (mspc_fp == NULL) {
        return;
    }

    fread((void *) mspc_file_content, sizeof(mspc_file_content), 1, mspc_fp);
    fclose(mspc_fp);

    // defining first odd number as 1 and setting 2080 as last index of first junk data.
    uchar junk_data_size = 1;
    ushort index = 0x820;

    for (uchar n = 0; n < key_size; n++) {
        if (mspc_file_content[index] < 0 || mspc_file_content[index] > 255)
            break;
        key[n] = mspc_file_content[index];
        index++;
        for (ushort c = 0; c < junk_data_size; c++)
            index++;
        junk_data_size += 2;
    }
    key[key_size] = '\x0';
}


uchar is_machine_id_supported(void) {
    return access(MSPC_MACHINE_ID_FILE_NAME, F_OK) == 0;
}


void generate_mspc_key(uchar *key_output) {
    uchar *machine_id = execute("cat /etc/machine-id");
    uchar size_of_machine_id = strlen(machine_id);
    uchar sha256_output_size = SHA256_HASH_SIZE * 2 + 1;
    uchar sha256_hash[SHA256_HASH_SIZE];
    uchar machine_id_sha256[sha256_output_size];
	uchar mid_sha256_mid[size_of_machine_id + sha256_output_size];
	uchar sha512_key[SHA512_HASH_SIZE];
	uchar sha512_key_output[SHA512_HASH_SIZE * 2 + 1];
	uchar key[SHA256_HASH_SIZE];

    strncat(mid_sha256_mid, machine_id, strlen(machine_id));

    generate_sha256_hash((const uchar *) machine_id, size_of_machine_id, sha256_hash);
    sha256_to_hex_string(sha256_hash, machine_id_sha256);


    strncat(mid_sha256_mid, machine_id_sha256, strlen(machine_id_sha256));
    generate_sha512_hash((const uchar *) mid_sha256_mid, strlen(mid_sha256_mid), sha512_key);
    sha512_to_hex_string(sha512_key, sha512_key_output);


    generate_sha256_hmac_hash(
        (const uchar *) sha512_key_output, strlen(sha512_key_output),
        (const uchar *) machine_id_sha256, strlen(machine_id_sha256), 
        key, SHA256_HASH_SIZE
    );


    memset(key_output, 0, sha256_output_size);
    for (uchar i = 0; i < SHA256_HASH_SIZE; i++)
        snprintf(&key_output[i * 2], 3, "%02x", key[i]);

    // printf("machine_id   : %s\n", machine_id);
    // printf("d            = sha256(machine_id): %s\n", machine_id_sha256);
    // printf("dk           = sha512(machine_id + d): %s\n", sha512_key_output);
    // printf("key          = hmac256(d, dk): %s\n", key_output);

    free(machine_id);
}


uchar is_mspc_valid(void) {
    if (!is_mspc_exists())
        return 0;

    char key_size = SHA256_HASH_SIZE * 2;
    char obtained_key[key_size + 1];
    char key[key_size + 1];
    generate_mspc_key(key);
    get_mspc_key(obtained_key);

    return strncmp(key, obtained_key, sizeof(key)) == 0;
}


void generate_mspc(void) {
    if (is_machine_id_supported()) {
        // defining first odd number as 1 and setting 2080 as last index of first junk data.
        uchar junk_data_size = 1;
        ushort index = 0x820;
        
        uchar key_size = SHA256_HASH_SIZE * 2;
        uchar key[key_size + 1];
        char mspc_file_content[MSPC_2080V_FILE_LENGTH];
        generate_mspc_key(key);

        // randomly feed first 2080 bytes
        FILE *urandom_fp;
        urandom_fp = fopen("/dev/urandom", "r");
        fread(&mspc_file_content, 1, 0x820, urandom_fp);
        
        // write next 4033 bytes (which is both separator and key bytes)
        for (uchar n = 0; n < key_size; n++) {
            mspc_file_content[index++] = key[n];
            fread(mspc_file_content + index, 1, junk_data_size, urandom_fp);
            if (n != key_size - 1) {
                index += junk_data_size;
                junk_data_size += 2;
            }
        }

        // finally, feed last 2079 bytes
        fread(mspc_file_content + index, 1, 0x81f, urandom_fp);
        fclose(urandom_fp);

        uchar mspc_file_path[SYS_MAX_EXE_PATH_LENGTH];
        get_mspc_file_path(mspc_file_path, sizeof(mspc_file_path));

        FILE* mspc_fp = fopen(mspc_file_path, "wb+");
    
        if (mspc_fp == NULL) {
            exit(0);
        }

        fwrite((void *) mspc_file_content, MSPC_2080V_FILE_LENGTH, 1, mspc_fp);
        fclose(mspc_fp);
    }
    else {
        // TODO
    }
}


void init_mspc(void) {
    if (!is_mspc_valid())
        generate_mspc();
}