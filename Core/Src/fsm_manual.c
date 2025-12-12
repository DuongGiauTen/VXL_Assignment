/* fsm_manual.c */
#include "fsm_manual.h"
#include "main.h"
#include "button.h"
#include "light.h"
#include "i2c-lcd.h"
#include <stdio.h>

int blink_flag = 0;

// Biến lưu giá trị tạm thời khi điều chỉnh
static int temp_val = 0;

void fsm_manual_run() {
    char buffer[20];

    // --- XỬ LÝ CHUYỂN CHẾ ĐỘ & NẠP GIÁ TRỊ TẠM ---
    if (isButton1Pressed()) {

        switch (status) {
            case RED_GREEN:
            case RED_YELLOW:
            case GREEN_RED:
            case YELLOW_RED:
            case INIT:
                status = MAN_RED_CONFIG;
                temp_val = time_red; // Nạp thời gian Đỏ hiện tại vào biến tạm
                break;

            case MAN_RED_CONFIG:
                status = MAN_YELLOW_CONFIG;
                temp_val = time_yellow; // Nạp thời gian Vàng hiện tại
                break;

            case MAN_YELLOW_CONFIG:
                status = MAN_GREEN_CONFIG;
                temp_val = time_green; // Nạp thời gian Xanh hiện tại
                break;

            case MAN_GREEN_CONFIG:
                status = INIT; // Reset về Auto
                break;

            default:
                status = MAN_RED_CONFIG;
                temp_val = time_red;
                break;
        }
    }

    // --- LOGIC CÁC TRẠNG THÁI ---
    switch (status) {
        case MAN_RED_CONFIG:
            // 1. Nháy đèn Đỏ
            if (blink_flag == 0) {
                lightTraffic1(RED); lightTraffic2(RED);
            } else {
                lightTraffic1(OFF); lightTraffic2(OFF);
            }

            // 2. Hiển thị giá trị TẠM THỜI (temp_val)
            lcd_goto_XY(1, 0); lcd_send_string("Mode: Modify RED");
            lcd_goto_XY(2, 0); sprintf(buffer, "Value: %02d      ", temp_val);
            lcd_send_string(buffer);

            // 3. Tăng giá trị biến TẠM
            if (isButton2Pressed()) {
                temp_val++;
                if (temp_val > 99) temp_val = 1;
            }
            if (isButton4Pressed()) {
                temp_val--;
                if (temp_val > 99) temp_val = 1;
                if (temp_val < 0) temp_val = 99;
            }

            // 4. Lưu giá trị thật (Button 3)
            if (isButton3Pressed()) {
                time_red = temp_val; // Gán chính thức

                // Logic tính lại Xanh: Xanh = Đỏ - Vàng
                // (Đảm bảo logic dương)
                if (time_red > time_yellow) {
                    time_green = time_red - time_yellow;
                } else {
                    // Nếu Đỏ < Vàng, reset Vàng về 1 để tính Xanh hợp lý (hoặc tùy logic bạn chọn)
                    time_yellow = 1;
                    time_green = time_red - time_yellow;
                }

                lcd_goto_XY(1, 0);
                lcd_send_string("    CONFIRMED   ");
                // Lưu ý: Dòng chữ này sẽ bị ghi đè rất nhanh ở vòng lặp sau
                // Nhưng người dùng sẽ thấy nháy lên 1 cái báo hiệu.
            }
            break;

        case MAN_YELLOW_CONFIG:
            if (blink_flag == 0) {
                lightTraffic1(YELLOW); lightTraffic2(YELLOW);
            } else {
                lightTraffic1(OFF); lightTraffic2(OFF);
            }

            lcd_goto_XY(1, 0); lcd_send_string("Mode: Modify YEL");
            lcd_goto_XY(2, 0); sprintf(buffer, "Value: %02d      ", temp_val);
            lcd_send_string(buffer);

            if (isButton2Pressed()) {
                temp_val++;
                if (temp_val > 99) temp_val = 1;
            }

            if (isButton4Pressed()) {
                temp_val--;
                if (temp_val > 99) temp_val = 1;
                if (temp_val < 0) temp_val = 99;
            }

            if (isButton3Pressed()) {
                time_yellow = temp_val; // Gán chính thức

                // Đỏ = Xanh + Vàng (Thay đổi Vàng -> Cập nhật Đỏ, giữ Xanh)
                time_red = time_green + time_yellow;
                // Nếu vượt quá 99 thì xử lý giới hạn (tùy chọn)
                if(time_red > 99){
                	time_red = 99;
                	time_yellow = time_red - time_green;
                }

                lcd_goto_XY(1, 0);
                lcd_send_string("    CONFIRMED   ");
            }
            break;

        case MAN_GREEN_CONFIG:
            if (blink_flag == 0) {
                lightTraffic1(GREEN); lightTraffic2(GREEN);
            } else {
                lightTraffic1(OFF); lightTraffic2(OFF);
            }

            lcd_goto_XY(1, 0); lcd_send_string("Mode: Modify GRN");
            lcd_goto_XY(2, 0); sprintf(buffer, "Value: %02d      ", temp_val);
            lcd_send_string(buffer);

            if (isButton2Pressed()) {
                temp_val++;
                if (temp_val > 99) temp_val = 1;
            }
            if (isButton4Pressed()) {
                temp_val--;
                if (temp_val > 99) temp_val = 1;
                if (temp_val < 0) temp_val = 99;
            }

            if (isButton3Pressed()) {
                time_green = temp_val; // Gán chính thức

                // Đỏ = Xanh + Vàng (Thay đổi Xanh -> Cập nhật Đỏ, giữ Vàng)
                time_red = time_green + time_yellow;
                if(time_red > 99){
                	time_red = 99;
                	time_yellow = time_red - time_green;
                }

                lcd_goto_XY(1, 0);
                lcd_send_string("    CONFIRMED   ");
            }
            break;

        default:
            break;
    }
}

void toggle_blink() {
    blink_flag = 1 - blink_flag;
}
