/*
 * crc.h
 *
 *  Created on: May 12, 2022
 *      Author: marco
 *  Sep.6,2024  MHG Limit to CRC16, Little endian
 */

#ifndef INCLUDE_CRC_H_
#define INCLUDE_CRC_H_

#include <stdint.h>
#define LITTLE_ENDIAN_TARGET
uint16_t GetCRC(uint8_t *puchMsg, uint16_t usDataLen);


#endif /* CRC_H_ */
