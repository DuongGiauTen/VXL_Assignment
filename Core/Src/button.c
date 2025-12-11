/* button.c */
#include "button.h"

int KeyReg0[4] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
int KeyReg1[4] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
int KeyReg2[4] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
int KeyReg3[4] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};

int TimeOutForKeyPress[4] =  {200, 200, 200, 200}; // Thời gian nhấn giữ (2s vì timer gọi 10ms)
int button1_flag = 0;
int button2_flag = 0;
int button3_flag = 0;
int button4_flag = 0;

// Hàm kiểm tra cờ
int isButton1Pressed(){
    if(button1_flag == 1){
        button1_flag = 0;
        return 1;
    }
    return 0;
}

int isButton2Pressed(){
    if(button2_flag == 1){
        button2_flag = 0;
        return 1;
    }
    return 0;
}

int isButton3Pressed(){
    if(button3_flag == 1){
        button3_flag = 0;
        return 1;
    }
    return 0;
}

int isButton4Pressed(){
    if(button4_flag == 1){
        button4_flag = 0;
        return 1;
    }
    return 0;
}

void subKeyProcess(int index){
    if(index == 0) button1_flag = 1;
    if(index == 1) button2_flag = 1;
    if(index == 2) button3_flag = 1;
    if(index == 3) button4_flag = 1;
}

void getKeyInput(){
    // Đọc trạng thái vật lý
    int current_val[4];
    current_val[0] = HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin);
    current_val[1] = HAL_GPIO_ReadPin(BUTTON2_GPIO_Port, BUTTON2_Pin);
    current_val[2] = HAL_GPIO_ReadPin(BUTTON3_GPIO_Port, BUTTON3_Pin);
    current_val[3] = HAL_GPIO_ReadPin(BUTTON4_GPIO_Port, BUTTON4_Pin);

    for(int i = 0; i < 4; i++){
        KeyReg0[i] = KeyReg1[i];
        KeyReg1[i] = KeyReg2[i];
        KeyReg2[i] = current_val[i];

        if ((KeyReg0[i] == KeyReg1[i]) && (KeyReg1[i] == KeyReg2[i])){
            if (KeyReg3[i] != KeyReg2[i]){
                KeyReg3[i] = KeyReg2[i];
                if (KeyReg3[i] == PRESSED_STATE){
                    TimeOutForKeyPress[i] = 200; // 2s
                    subKeyProcess(i);
                }
            } else {
                TimeOutForKeyPress[i]--;
                if (TimeOutForKeyPress[i] == 0){
                    TimeOutForKeyPress[i] = 50; // Nhanh hơn khi nhấn giữ (0.5s)
                    if (KeyReg3[i] == PRESSED_STATE){
                        subKeyProcess(i);
                    }
                }
            }
        }
    }
}
