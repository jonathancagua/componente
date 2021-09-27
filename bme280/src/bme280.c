#include "bme280.h"
#include <stddef.h>

static int8_t bme280_reg_get(struct bme280_dev *dev, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t resp = ERROR_PTR_NULL;
    if((dev == NULL) && (reg_data == NULL)) return(resp);
    resp = ERROR_NOT;

    dev->inter_resp = dev->fread(reg_addr, reg_data, len, dev->ptrInt);

    if (dev->inter_resp != BME280_INTF_RESP_SUCCESS) resp = ERROR_LOGIN_FAIL;

    return resp;
}
static int8_t bme280_reg_set(struct bme280_dev *dev , uint8_t reg_addr, uint8_t *reg_data, uint8_t len)
{
    int8_t resp = ERROR_PTR_NULL;
    if((dev == NULL)) return(resp);
    resp = ERROR_NOT;

    dev->inter_resp = dev->fwrite(reg_addr, reg_data, len, dev->ptrInt);
    if (dev->inter_resp != BME280_INTF_RESP_SUCCESS) resp = ERROR_LOGIN_FAIL;
}
static int8_t bme280_reset_sw(struct bme280_dev *dev)
{
    uint8_t status_reg = 0;
    uint8_t retry_ = 5;
    uint8_t cmd_rst_procedure = BME280_RESET_PW_ON;
    int8_t resp = ERROR_PTR_NULL;
    if(dev == NULL) return(resp);
    resp = bme280_reg_set(dev, BME280_ADDR_RESET, &cmd_rst_procedure, 1);
    if (resp == ERROR_NOT)
    {
        do
        {
            //Table 1, startup time is 2 ms.
            dev->delay_ms(2, dev->ptrInt);
            resp = bme280_reg_get(dev, BME280_ADDR_STATUS, &status_reg, 1 );
        } while ((resp == ERROR_NOT) && (retry_--) && (status_reg & BME280_STATUS_IM_UPDATE));
        if (status_reg & BME280_STATUS_IM_UPDATE) resp = ERROR_NVM_NOT_COPY;
    }
    return(resp);
}
int8_t bme280_init(struct bme280_dev *dev)
{
    uint8_t chip_id = 0;
    int8_t resp = ERROR_PTR_NULL;
    if(dev == NULL) return(resp);

    while(1){
        resp = bme280_reg_get(dev,BME280_ADDR_CHIP_ID, &chip_id, 1);
        if((resp == ERROR_NOT) && (chip_id == BME280_CHIP_ID)){
            //Se pudo leer bien el id del sensor
            dev->chip_id = chip_id;
            //ahora hacemos un reset de sw
            if(bme280_reset_sw(dev) == ERROR_NOT){
                //Calibrar
            }
        }
    }

}
