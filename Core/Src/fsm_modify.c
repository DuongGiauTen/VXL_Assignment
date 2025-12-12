/*
 * fsm_MANUAL.c
 *
 * Created on: Dec 12, 2025
 * Author: Duong Dep Trai
 */

#include "fsm_MANUAL.h"
#include "main.h"
#include "button.h"
#include "light.h"
#include "i2c-lcd.h"
#include <stdio.h>

void fsm_modify(void){
    if (isButton4Pressed()  && (status < 10 || status >13)) {
        switch (status) {
            case RED_GREEN_MOD:
                status = RED_YELLOW_MOD;
                lightTraffic1(RED);
                lightTraffic2(YELLOW);
                lcd_clear_display();
                lcd_goto_XY(1, 0); lcd_send_string("MANUAL MODE");
                lcd_goto_XY(2, 0); lcd_send_string("W1:RED  W2:YEL");
                break;

            case RED_YELLOW_MOD:
                status = GREEN_RED_MOD;
                lightTraffic1(GREEN);
                lightTraffic2(RED);
                lcd_clear_display();
                lcd_goto_XY(1, 0); lcd_send_string("MANUAL MODE");
                lcd_goto_XY(2, 0); lcd_send_string("W1:GRN  W2:RED");
                break;

            case GREEN_RED_MOD:
                status = YELLOW_RED_MOD;
                lightTraffic1(YELLOW);
                lightTraffic2(RED);
                lcd_clear_display();
                lcd_goto_XY(1, 0); lcd_send_string("MANUAL MODE");
                lcd_goto_XY(2, 0); lcd_send_string("W1:YEL  W2:RED");
                break;

            case YELLOW_RED_MOD:
                status = RED_GREEN_MOD;
                lightTraffic1(RED);
                lightTraffic2(GREEN);
                lcd_clear_display();
                lcd_goto_XY(1, 0); lcd_send_string("MANUAL MODE");
                lcd_goto_XY(2, 0); lcd_send_string("W1:RED  W2:YEL");
                break;

            default:
                // Khoi tao khi tu Auto chuyen sang MANUAL
                status = RED_GREEN_MOD;
                lightTraffic1(RED);
                lightTraffic2(GREEN);
                lcd_clear_display();
                lcd_goto_XY(1, 0); lcd_send_string("MANUAL MODE");
                lcd_goto_XY(2, 0); lcd_send_string("W1:RED  W2:GRN");
                break;
        }
    }
    if (isButton3Pressed()  && (status >13)) {
    	status = INIT;
    }

}
