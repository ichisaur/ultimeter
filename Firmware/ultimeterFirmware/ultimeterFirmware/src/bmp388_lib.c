/*
 * bmp388_lib.c
 *
 * Created: 5/15/2019 7:16:11 PM
 *  Author: ichis
 */ 

#include "bmp388_lib.h"

bool  bmp_begin(uint8_t addr = BMP3XX_DEFAULT_ADDRESS, TwoWire *theWire=&Wire) {
    int8_t rslt = BMP3_OK;

    if (BMP3_CHIP_ID != read8(BMP3_I2C_ADDR_PRIM, BMP3_CHIP_ID_ADDR)) {
        return false;
    }

    bmp_setTemperatureOversampling(BMP3_NO_OVERSAMPLING);
    bmp_setPressureOversampling(BMP3_NO_OVERSAMPLING);
    bmp_setIIRFilterCoeff(BMP3_IIR_FILTER_DISABLE);

    return true;
}


float bmp_readTemperature(void) {

    int8_t rslt;

    uint16_t setting_sel = 0;

    uint8_t sensor_comp = 0;

    settings_sel |= BMP3_TEMP_EN_SEL;
    sensor_comp |= BMP3_TEMP;

      settings_sel |= BMP3_PRESS_EN_SEL;
sensor_comp |= BMP3_PRESS;


    set_pwr_ctrl_settings(desired_settings, dev);
    set_odr_filter_settings(desired_settings, dev);
    set_int_ctrl_settings(desired_settings, dev);
    set_advance_settings(desired_settings, dev);
}
    float bmp_readPressure(void);
    float bmp_readAltitude(float seaLevel);

    bool bmp_setTemperatureOversampling(uint8_t os);
    bool bmp_setPressureOversampling(uint8_t os);
    bool bmp_setIIRFilterCoeff(uint8_t fs);
    bool bmp_setOutputDataRate(uint8_t odr);

    /// Perform a reading in blocking mode
bool bmp_performReading(void);