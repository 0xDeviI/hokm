/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Wednesday, February 14th 2024, 3:15:24 am
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "libs/cards/card.h"
#include "libs/lobby/lobby.h"
#include "libs/player/player.h"
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
#include "libs/menu/menu.h"
#include "game/menu/menu.h"
#include "game/splash/splash.h"
#include "libs/audio/audio.h"
#include "libs/crypto/mspc/mspc.h"
#include "libs/casset/api.h"

// menu:

// 1. Create Game
// 2. Join Game
// 3. Global Games
// 4. Settings
// 5. Credit
// 6. Exit

int main(int argc, char* argv[]) {
    // Screen screen;
    // init_screen(&screen, 1);
    // draw_screen_frame(&screen);
    // endwin();
    init_io_system(argv);
    init_mspc();

    uchar *txt_assset;
    get_asset("test.txt", txt_assset);

    // unsigned char salt[8];
    // unsigned char key[AES_KEY_SIZE / 8];
    // unsigned char iv[AES_BLOCK_SIZE];
    // unsigned char plaintext_chunk[CHUNK_SIZE];
    // unsigned char ciphertext_chunk[CHUNK_SIZE + EVP_MAX_BLOCK_LENGTH]; // Extra space for padding
    // int chunk_len;
    // int ciphertext_len;
    // const char *password = "MySecretPassword";
    
    // FILE *input_file = fopen("input.txt", "rb");
    // if(!input_file) {
    //     fprintf(stderr, "Error opening input file.\n");
    //     return 1;
    // }

    // FILE *output_file = fopen("output.enc", "wb");
    // if(!output_file) {
    //     fprintf(stderr, "Error opening output file.\n");
    //     fclose(input_file);
    //     return 1;
    // }

    // // Generate random salt
    // RAND_bytes(salt, sizeof(salt));

    // // Derive key from password and salt
    // if(pbkdf2(password, salt, sizeof(salt), key, sizeof(key)) != 0) {
    //     fprintf(stderr, "Error deriving key from password.\n");
    //     fclose(input_file);
    //     fclose(output_file);
    //     return 1;
    // }

    // // Initialize IV with random data
    // RAND_bytes(iv, AES_BLOCK_SIZE);

    // while ((chunk_len = fread(plaintext_chunk, 1, CHUNK_SIZE, input_file)) > 0) {
    //     // Encrypt data chunk by chunk
    //     ciphertext_len = encrypt_data_chunk(plaintext_chunk, chunk_len, key, iv, ciphertext_chunk);

    //     // Write encrypted data chunk to output file
    //     fwrite(ciphertext_chunk, 1, ciphertext_len, output_file);
    // }

    // printf("Data encrypted and written to output.enc\n");

    // fclose(input_file);
    // fclose(output_file);

    // Audio *audio = create_audio("assets/music/IDK_Moskau_8_bit.wav", 1);
    // play_audio(audio);
    
    // splash_init();

    pthread_exit(NULL);
    clear_thread_mem_pool();
    return 0;
}
