#include "bme280.h"
#include <stddef.h>

int8_t bme280_get_regs(struct bme280_dev *dev, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t resp = ERROR_PTR_NULL;
    if((dev == NULL) && (reg_data == NULL)) return(resp);
    resp = ERROR_NOT;

    dev->inter_resp = dev->fread(reg_addr, reg_data, len, dev->ptrInt);

    if (dev->inter_resp != BME280_INTF_RESP_SUCCESS)
    {
        resp = ERROR_LOGIN_FAIL;
    }

    return resp;
}


int8_t bme280_init(struct bme280_dev *dev)
{
    uint8_t chip_id = 0;
    int8_t resp = ERROR_PTR_NULL;
    if(dev == NULL) return(resp);

    while(1){
        resp = bme280_get_regs(dev,BME280_ADDR_CHIP_ID, &chip_id, 1);
    }

}
