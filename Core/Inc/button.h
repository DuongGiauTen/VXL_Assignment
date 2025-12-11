#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#define NORMAL_STATE GPIO_PIN_SET // Vì dùng Pull-up
#define PRESSED_STATE GPIO_PIN_RESET

extern int button1_flag;
extern int button2_flag;
extern int button3_flag;
extern int button4_flag; // Thêm nút 4

int isButton1Pressed();
int isButton2Pressed();
int isButton3Pressed();
int isButton4Pressed(); // Thêm nút 4

void getKeyInput();

#endif
