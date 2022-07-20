/* 
Copyright (c) 2017 Silan MEMS. All Rights Reserved.
*/
#ifndef _GSENSOR_H_
#define _GSENSOR_H_

#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <GsensorAdapter.h>

#define I2C_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL        0x0         /*!< I2C ack value */
#define NACK_VAL       0x1         /*!< I2C nack value */

/**********sc7a20**********/
#define IIC_ADDR                  0x19

#define SL_SC7A20_CHIP_ID_ADDR    (uint8_t)0X0F
#define SL_SC7A20_CHIP_ID_VALUE   (uint8_t)0X11
#define SL_SC7A20_POWER_ODR_REG   (uint8_t)0X20
#define SL_SC7A20_FIFO_CTRL_REG   (uint8_t)0X2E
#define SL_SC7A20_FIFO_SRC_REG    (uint8_t)0X2F
#define SL_SC7A20_SPI_OUT_X_L     (uint8_t)0X27

#define OUT_X_L_REG     (uint8_t)0X28
#define OUT_X_H_REG     (uint8_t)0X29
#define OUT_Y_L_REG     (uint8_t)0X2A
#define OUT_Y_H_REG     (uint8_t)0X2B
#define OUT_Z_L_REG     (uint8_t)0X2C
#define OUT_Z_H_REG     (uint8_t)0X2D

#define SC7A20_REG2_NUM        4

const uint8_t sc7a20_init_reg2[12]=
{
    //0x2E,0x00,0x00,//
    0x20,0x47,0x00,//使能x,y,z轴，LPen =0正常模式，50Hz,
    0x23,0x18,0x00,//连续更新数据块，大端数据，4G量程，
    0x24,0xC0,0x00,//使能FIFO模式
    0x2E,0x4F,0x00,//FIFO 模式，阈值是F
};

/*!
 * @brief The class of gsensor sc7a20
 */
class Gsensor_Sc7a20: public GsensorAdapter{

    public:
        Gsensor_Sc7a20(Adafruit_I2CDevice *i2cdevice);
        virtual uint8_t getFifoBuf(signed short *x_buf,signed short *y_buf,signed short *z_buf) override;
        virtual uint8_t getGsensorvalue(signed short *x,signed short *y,signed short *z) override;
        virtual bool init() override;

    private:
        bool writeReg(uint8_t reg_add, uint8_t data);
        bool readReg(uint8_t reg_add, uint8_t *p_data);
};

#endif

