
#include "esp_log.h"
#include "TkGsrSc7a20.h"

static const char *TAG = "sc7a20";

Gsensor_Sc7a20::Gsensor_Sc7a20(Adafruit_I2CDevice *i2cdevice) : GsensorAdapter(i2cdevice) {

}

bool Gsensor_Sc7a20::readReg(uint8_t reg_add, uint8_t *p_data)
{
    return i2cdevice->write_then_read(&reg_add, sizeof(reg_add), p_data, 1, true);
}

bool Gsensor_Sc7a20::writeReg(uint8_t reg_add, uint8_t data)
{
    return i2cdevice->write( &data, sizeof(data), true, &reg_add, sizeof(reg_add));
}

bool Gsensor_Sc7a20::init() {

    uint8_t  gsensor_id = 0;
    uint8_t  i = 0;
    bool res = true;
  //配置参数
    for(i=0;i<SC7A20_REG2_NUM;i++)
    {
       res &= writeReg(sc7a20_init_reg2[3*i],sc7a20_init_reg2[3*i+1]);
    }
    //校验reg值
   /* for(i=0;i<SC7A20_REG2_NUM;i++)
    {
       res &= readReg(sc7a20_init_reg2[3*i], &sc7a20_init_reg2[3*i+2]);
    }*/
    //读取gsensor id （sc7a20 ID 是0x11）
    res &= readReg(SL_SC7A20_CHIP_ID_ADDR,&gsensor_id);

    //ESP_LOGI(TAG, "sc7a20_init res = %d,id:%d\n", res,gsensor_id);
    printf("sc7a20_init ,res = %d,id:%d\n", res,gsensor_id);
    return res;
}
//获取gsensor FIFO数据
uint8_t Gsensor_Sc7a20::getFifoBuf(signed short *x_buf,signed short *y_buf,signed short *z_buf)
{
    uint8_t  i=0,m=0;
    uint8_t  XL_BUF = 0;
    uint8_t  XH_BUF = 0;
    uint8_t  YL_BUF = 0;
    uint8_t  YH_BUF = 0;
    uint8_t  ZL_BUF = 0;
    uint8_t  ZH_BUF = 0;

    uint8_t  fifo_nums; //fifo 数据量
   // signed short x = 0,y = 0,z = 0;
    //获取FIFO数据长度
    readReg(SL_SC7A20_FIFO_SRC_REG,&fifo_nums);//获取FIFO数量
    fifo_nums = fifo_nums & 0x1f; //限长

    for(i=0;i<fifo_nums;i++)
    {
         readReg(OUT_X_L_REG, &XL_BUF);
         readReg(OUT_X_H_REG, &XH_BUF);
         readReg(OUT_Y_L_REG, &YL_BUF);
         readReg(OUT_Y_H_REG, &YH_BUF);
         readReg(OUT_Z_L_REG, &ZL_BUF);
         readReg(OUT_Z_H_REG, &ZH_BUF);

         x_buf[i] =(signed short )((XH_BUF << 8 ) | XL_BUF);
         y_buf[i] =(signed short )((YH_BUF << 8 ) | YL_BUF);
         z_buf[i] =(signed short )((ZH_BUF << 8 ) | ZL_BUF);

         printf("Gsensor_Sc7a20 ,i:%d,X:%d,Y:%d,Z:%d\n",i,x_buf[i],y_buf[i],z_buf[i]);
     }

    //重载数据
    writeReg(SL_SC7A20_FIFO_CTRL_REG,0x00);
    writeReg(SL_SC7A20_FIFO_CTRL_REG,0x4F);

    return fifo_nums;
}

uint8_t Gsensor_Sc7a20::getGsensorvalue(signed short *x,signed short *y,signed short *z)
{
    signed short X[32] = {0};
    signed short Y[32] = {0};
    signed short Z[32] = {0};
    uint8_t len =0;
    len = getFifoBuf(X,Y,Z);
    if(len)
    {
       x = X+(len-1);
       y = Y+(len-1);
       z = Z+(len-1);
       return 1;
    }
    return 0;
   
}
 #if 0
static void Gsensor_Task(void *arg) 
{
    uint8_t ret ;
    signed short X[32] = {0};
    signed short Y[32] = {0};
    signed short Z[32] = {0};
    uint8_t len =0;
    uint8_t i = 0;
    uint8_t who_am_i = 0 ;

	printf("Gsensor_Task ok !\n\n");

    who_am_i = sc7a20_init();
    printf("sc7a20 who_am_i:%d\n\n",who_am_i);

    while (1)
    {
        len = sc7a20_read_fifo_buf(&X,&Y,&Z);
        if(len)
        {
          for(i = 0; i < len ; i++)
          {
               printf("i:%d,X:%d,Y:%d,Z:%d\n",i,X[i],Y[i],Z[i]);
          }
        }       
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}
#endif