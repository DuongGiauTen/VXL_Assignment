/*
 * fsm_automatic.c
 *
 *  Created on: Dec 11, 2025
 *      Author: my pc
 */

/* fsm_automatic.c */
#include "fsm_automatic.h"
#include "main.h" // Để dùng HAL_GPIO
#include "light.h" // Giả sử bạn sẽ viết hàm điều khiển đèn module 2 ở đây
#include "i2c-lcd.h"
#include <stdio.h> // Để dùng sprintf

// Hàm điều khiển đèn Module 2 (Bạn nên chuyển vào light.c sau này)


void update_time_counter() {
    if (time_counter_red > 0) time_counter_red--;
    if (time_counter_green > 0) time_counter_green--;
    if (time_counter_yellow > 0) time_counter_yellow--;
}

void fsm_automatic_run() {
    char buffer[20];

    // Chỉ chạy khi ở chế độ tự động (status < 10)
    if (status >= 10) return;

    switch (status) {
        case INIT:
            // Khởi tạo trạng thái đầu: Đỏ 1 - Xanh 2
        	lcd_clear_display();
            status = RED_GREEN;
            time_counter_red = time_red;   // 5s
            time_counter_green = time_green; // 3s
            time_counter_yellow = time_yellow; // 2s
            break;

        case RED_GREEN:
            // Điều khiển đèn
            lightTraffic1(RED);
            lightTraffic2(GREEN);

            // Hiển thị LCD
            // Dòng 1: Thời gian còn lại của Đỏ 1
            // Dòng 2: Thời gian còn lại của Xanh 2
            // Lưu ý: time_counter_red đếm từ 5 về 0. time_counter_green đếm từ 3 về 0.
            // Cần logic hiển thị đúng. Thường hiển thị time_counter_red cho đèn 1, và time_counter_green cho đèn 2.

            lcd_goto_XY(1, 0);
            sprintf(buffer, "WAY1: RED: %02d   ", time_counter_red);
            lcd_send_string(buffer);

            lcd_goto_XY(2, 0);
            sprintf(buffer, "WAY2: GRN: %02d   ", time_counter_green);
            lcd_send_string(buffer);



            // Chuyển trạng thái khi Đèn 2 hết Xanh (3s) -> Sang Vàng
            if (time_counter_green <= 0) {
                status = RED_YELLOW;
                time_counter_green = time_red; // Reset cho vòng sau
                // time_counter_red vẫn tiếp tục đếm
                time_counter_yellow = time_yellow;
            }
            break;

        case RED_YELLOW:
            lightTraffic1(RED);
            lightTraffic2(YELLOW);

            lcd_goto_XY(1, 0);
            sprintf(buffer, "WAY1: RED1: %02d   ", time_counter_red);
            lcd_send_string(buffer);

            lcd_goto_XY(2, 0);
            sprintf(buffer, "WAY2: YEL2: %02d   ", time_counter_yellow);
            lcd_send_string(buffer);

            // Chuyển trạng thái khi Đèn 2 hết Vàng (2s) -> Cả 2 đổi màu
            if (time_counter_yellow <= 0) { // Hết 2s Vàng -> Tổng 5s
                status = GREEN_RED;
                time_counter_red = time_red; // Reset Đỏ 1
                time_counter_yellow = time_green; // Reset Vàng 2
            }
            break;

        case GREEN_RED:
            lightTraffic1(GREEN);
            lightTraffic2(RED);

            lcd_goto_XY(1, 0);
            sprintf(buffer, "WAY1: GRN1: %02d   ", time_counter_green);
            lcd_send_string(buffer);

            lcd_goto_XY(2, 0);
            sprintf(buffer, "WAY2: RED2: %02d   ", time_counter_red);
            lcd_send_string(buffer);

            if (time_counter_green <= 0) {
                status = YELLOW_RED;
                time_counter_green = time_red;
                time_counter_yellow = time_yellow;
            }
            break;

        case YELLOW_RED:
            lightTraffic1(YELLOW);
            lightTraffic2(RED);

            lcd_goto_XY(1, 0);
            sprintf(buffer, "WAY1: YEL1: %02d   ", time_counter_yellow);
            lcd_send_string(buffer);

            lcd_goto_XY(2, 0);
            sprintf(buffer, "WAY2: RED2: %02d   ", time_counter_red);
            lcd_send_string(buffer);

            if (time_counter_yellow <= 0) {
                status = RED_GREEN;
                time_counter_red = time_red;
                time_counter_green = time_green;
            }
            break;

        default:
            break;
    }
}
