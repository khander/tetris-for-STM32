/*
 * Matrix.cpp
 *
 *  Created on: Mar 16, 2026
 *      Author: khaus
 */

#include <matrix.h>

Matrix::Matrix(uint8_t x_in, uint8_t y_in) : x(x_in), y(y_in){
	// TODO Auto-generated constructor stub
	for(uint8_t i = 0; i < x; i++){
		for(uint8_t j = 0; j < y; j++){
			matrix[i][j] = 0;
		}
	}
	for(uint8_t i = 0; i < Y_reg; i++){
		transmit_minus[i] = 255;
	}
	for(uint8_t i = 0; i < X_reg; i++){
		transmit_plus[i] = 0;
	}
	transmit_zero[0] = transmit_zero[1] = 0;

	matrix[0][0] = matrix[2][2] = 1;
}

bool Matrix::put_pixel(uint8_t x_in, uint8_t y_in, bool value){
	if(x_in > x || x_in < 0 || y_in > y || y_in < 0) return 1;
	matrix[x_in][y_in] = value;
	return 0;
}

void Matrix::get_transmit_plus(uint8_t y_get){
	for(uint8_t i = 0; i < X_reg * 8; i++){
		transmit_plus[i / 8] <<= 1;
		transmit_plus[i / 8] += (matrix[i][y_get]);
	}
}

void delay_us(uint32_t us) {
    uint32_t startTick = DWT->CYCCNT;
    uint32_t delayTicks = us * (SystemCoreClock / 1000000);
    while (DWT->CYCCNT - startTick < delayTicks);
}

uint8_t Matrix::print(SPI_HandleTypeDef *hspi1, SPI_HandleTypeDef *hspi2){

	for(uint8_t i = 0; i < 24; i++){
	if(i % 8 == 0) {
		transmit_minus[i / 8] &= 0b01111111;
	}
	get_transmit_plus(i);

	HAL_SPI_Transmit(hspi1,(uint8_t*)transmit_minus, 3, 100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);

	HAL_SPI_Transmit(hspi2, (uint8_t*)transmit_plus, 2, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);

	delay_us(300);							//delay in microseconds
	HAL_SPI_Transmit(hspi2, (uint8_t*)transmit_zero, 2, 100);	//turning led's off to avoid lighting neighbor led's
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);

	transmit_minus[i / 8] >>= 1;
	transmit_minus[i / 8] |= 0b10000000;	//going from bottom to top because of soldering issue
	}

	return 0;
}


Matrix::~Matrix() {
	// TODO Auto-generated destructor stub
}

