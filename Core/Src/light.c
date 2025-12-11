/*
 * light.c
 *
 *  Created on: Dec 11, 2025
 *      Author: DUONG DEP TRAI
 */
#include "light.h"


void lightTraffic1(int state){
	if (state == OFF){
		HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, RESET);
		HAL_GPIO_WritePin(D2_GPIO_Port, D2_Pin, RESET);
	}
	if (state == RED){
		HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, SET);
		HAL_GPIO_WritePin(D2_GPIO_Port, D2_Pin, SET);
	}
	if (state == YELLOW){
		HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, RESET);
		HAL_GPIO_WritePin(D2_GPIO_Port, D2_Pin, SET);
	}
	if (state == GREEN){
		HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, SET);
		HAL_GPIO_WritePin(D2_GPIO_Port, D2_Pin, RESET);
	}
}

void lightTraffic2(int state){
	if (state == OFF){
		HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, RESET);
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, RESET);
	}
	if (state == RED){
		HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, SET);
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, SET);
	}
	if (state == YELLOW){
		HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, RESET);
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, SET);
	}
	if (state == GREEN){
		HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, SET);
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, RESET);
	}
}
void Test(){
	lightTraffic1(3);
}

