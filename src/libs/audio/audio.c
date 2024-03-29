/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Friday, March 22nd 2024, 9:30:06 pm
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


#include "audio.h"


Audio *create_audio(uchar *audio_path, uchar is_repeatable) {
    if (strlen(audio_path) < AUDIO_PATH_MAX_NAME) {
        Audio *new_audio = (Audio *) malloc(sizeof(Audio));
        new_audio->is_playing = 0;
        new_audio->is_repeatable = is_repeatable;
        strcpy(new_audio->audio_path, audio_path);
        return new_audio;
    }
    else {
        return NULL;
    }
}


void *play_audio_thread(void *arg) {
    AudioData *audio_data = (AudioData *) arg;
    uint iteration = 0;
    ushort reduction_ms_rate = 100;
    while (audio_data->current_audio_length > 0) {
        SDL_Delay(reduction_ms_rate);
        audio_data->current_audio_length -= ++iteration * reduction_ms_rate;
    }
    SDL_CloseAudioDevice(*audio_data->device_id);
    SDL_FreeWAV(audio_data->wav_buffer);
    SDL_Quit();
}


uchar play_audio(Audio *audio) {
    SDL_Init(SDL_INIT_AUDIO);

    if (SDL_LoadWAV(audio->audio_path, &audio->wav_spec, &audio->wav_buffer, &audio->wav_length) == NULL) {
        printf("Error loading WAV file: %s\n", SDL_GetError());
        return 1;
    }

    AudioData audio_data;
    audio_data.wav_buffer = audio->wav_buffer;
    audio_data.current_audio_length = audio->wav_length;

    SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(NULL, 0, &audio->wav_spec, NULL, 0);
    if (device_id == 0) {
        printf("Failed to open audio device: %s\n", SDL_GetError());
        return 1;
    }
    audio_data.device_id = &device_id;


    int success = SDL_QueueAudio(device_id, audio->wav_buffer, audio->wav_length);
    if (success < 0) {
        printf("Failed to queue audio: %s\n", SDL_GetError());
        return 1;
    }

    SDL_PauseAudioDevice(device_id, 0);
    create_thread(play_audio_thread, (void *) &audio_data);
}


uchar pause_audio(Audio *audio) {

}


uchar stop_audio(Audio *audio) {

}