/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Thursday, March 21st 2024, 12:29:39 am
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


#include "splash.h"


uchar *HOKM_LARGE_LOGO[22] = {
"                                       :=#@@@=:                          \n",   
"                                     :=#@@@=                              \n",
"                                :=*#@@@@@@@=                              \n",
"                           .=*#@@@@@@@@@@#+.                              \n",
"                          #@@@@@@@@@#+-.                                  \n",
"                         .@@@@@@@+.                   :-==-.              \n",
"            .---:.        *@@@@@@@#+:              -#@@@@@@@@#+-.         \n",
"         .*@@@@@@@@*:      :+@@@@@@@@#:          :#@@@@@@@@@@@@@@@#+-.    \n",
"        =@@@@@@@@@@@@*        :+#@@@@@@#.       -@@@@@@+==+#@@@@@@@@@@@##-\n",
"       +@@@@@*==*@@@@@+          :+@@@@@@:      .-*#@=       .-+@@@@@@@@@=\n",
"      =@@@@#.    :@@@@@.            +@@@@@:                .-+#@@@@@@@@@@-\n",
"   +#@@@@@@#-.   :@@@@@#:        .:-#@@@@@@+:       .:-=+#@@@@@@@@@@*=:   \n",
" .@@@@@@@@@@@@@##@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*=.       \n",
" #@@@@@##@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#=#@@@@@@@@@@@@@@@@@@#*=:           \n",
".@@@@@=   :=*#@@@@#+. =*##########*+=:   :=*##########*+=:.               \n",
"-@@@@@.                                                                   \n",
"=@@@@@                                                                    \n",
"=@@@@@                                                                    \n",
"-@@@@@.                                                                   \n",
":@@@@@.                                                                   \n",
":@@@@@-                                                                   \n",
" :::::                                                                    \n"
};


void splash_wipe(void) {
    sleepmsf(28000);
    vtgclear();
}


void splash_init(void) {
    vtgclear();
    for (uchar i = 0; i < 22; i++) {
        prepare_to_print_center(&GLOBALSCR->location, &GLOBALSCR->size, 76, 22, i);
        add_screen_style(GLOBALSCR, &GLOBALSCR->location, 76, COLOR_WHITE, COLOR_BLACK, A_BOLD | A_ITALIC);
        vtgprintf(HOKM_LARGE_LOGO[i]);
    }

    splash_wipe();
}