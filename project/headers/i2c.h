#include <stm32f10x.h>

void i2c_init(void);
void i2c_enable(void);
void i2c_periph_set_ownaddr(void);
void i2c_periph_set_ack(void);
void i2c_waitForReady(void);
void i2c_sendStart(void);
void i2c_sendStop(void);
uint8_t i2c_sendAddr(uint8_t addr);
uint8_t i2c_sendAddrForWrite(uint8_t addr);
uint8_t i2c_sendAddrForRead(uint8_t addr);
uint8_t i2c_sendData(uint8_t data);
uint8_t i2c_readData(uint8_t ack);
void i2c_periph_ack_on_addr(void);
void i2c_sendbyte(uint8_t addr, uint8_t data);

