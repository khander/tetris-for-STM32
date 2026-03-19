/*
 * Matrix.h
 *
 *  Created on: Mar 16, 2026
 *      Author: khaus
 */

#ifndef SRC_MATRIX_H_
#define SRC_MATRIX_H_

#define X 16
#define Y 24
#define Y_reg Y/8
#define X_reg X/8

#include "stm32f1xx_hal.h"

class Matrix {
private:
	bool matrix[X][Y];
	uint8_t x, y;
	uint8_t transmit_minus[Y_reg];
	uint8_t transmit_plus[X_reg];

public:
	Matrix(uint8_t x_in = X, uint8_t y_in = Y);
	virtual ~Matrix();
	bool put_pixel(uint8_t x_in, uint8_t y_in, bool value);
	uint8_t print(SPI_HandleTypeDef *hspi1, SPI_HandleTypeDef *hspi2);
	void get_transmit_plus(uint8_t y_get);
};

#endif /* SRC_MATRIX_H_ */
