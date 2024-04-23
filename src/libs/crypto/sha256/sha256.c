/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Sunday, March 31st 2024, 2:21:18 am
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


#include "sha256.h"


void generate_sha256_hash(const uchar *input, size_t input_len, uchar *hash) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md = EVP_sha256();
    uint hash_len;

    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, input, input_len);
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);
}


void generate_sha256_hmac_hash(const uchar *key, size_t key_len, const uchar *input, size_t input_len, uchar *hash, size_t hash_size) {
	EVP_MAC *mac = EVP_MAC_fetch(NULL, "HMAC", NULL);
    const char *digest = "SHA256";
    EVP_MAC_CTX *ctx = NULL;
    OSSL_PARAM params[3];
    size_t params_n = 0;

    params[params_n++] = OSSL_PARAM_construct_utf8_string("digest", (char*)digest, 0);
    params[params_n] = OSSL_PARAM_construct_end();

    if (mac == NULL || key == NULL || (ctx = EVP_MAC_CTX_new(mac)) == NULL ||
        !EVP_MAC_init(ctx, key, key_len, params))
        goto err;

    if (!EVP_MAC_update(ctx, input, input_len) ||
        !EVP_MAC_final(ctx, hash, &input_len, EVP_MAX_MD_SIZE))
        goto err;

    EVP_MAC_CTX_free(ctx);
    EVP_MAC_free(mac);
    return;

err:
    EVP_MAC_CTX_free(ctx);
    EVP_MAC_free(mac);
    fprintf(stderr, "Error in generating HMAC hash\n");
    ERR_print_errors_fp(stderr);
}


void sha256_to_hex_string(uchar *hash, uchar *output) {
    uchar hex_digits[] = "0123456789abcdef";
    for (uchar i = 0; i < SHA256_HASH_SIZE; i++) {
        output[i * 2] = hex_digits[hash[i] >> 4];
        output[i * 2 + 1] = hex_digits[hash[i] & 0x0F];
    }
    output[SHA256_HASH_SIZE * 2] = '\0';
}