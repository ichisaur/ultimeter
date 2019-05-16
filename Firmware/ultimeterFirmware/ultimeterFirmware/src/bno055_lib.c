/*
 * bno055_lib.c
 *
 * Created: 5/15/2019 7:13:29 PM
 *  Author: ichis
 */ 

#include "bno055_lib.h"

bool bno_begin(void) {
    uint8_t uint8_t id = bno_read8(BNO055_CHIP_ID_ADDR);
    if (id != BNO055_ID) {
    delay(1000); // hold on for boot
    id = bno_read8(BNO055_CHIP_ID_ADDR);
    if (id != BNO055_ID) {
      return false; // still not? ok bail
    }
    }

    bno_setMode(OPERATION_MODE_CONFIG);

    bno_write8(BNO055_SYS_TRIGGER_ADDR, 0x20);


    while (bno_read8(BNO055_CHIP_ID_ADDR); != BNO055_ID) {
        delay(10);
    }
    delay(50);

    bno_write8(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);
    delay(10);

    bno_setMode(OPERATION_MODE_NDOF);
    return true;
}


void bno_setMode(adafruit_bno055_opmode_t mode) {
    bno_mode = mode;
    bno_write8(BNO055_OPR_MODE_ADDR, _mode);
    delay(30);
}


void bno_getSystemStatus(uint8_t *system_status, uint8_t *self_test_result, uint8_t *system_error) {
    bno_write8(BNO055_PAGE_ID_ADDR, 0);

    if (system_status != 0)
        *system_status = bno_read8(BNO055_SYS_STAT_ADDR);

    if (self_test_result != 0)
        *self_test_result = bno_read8(BNO055_SELFTEST_RESULT_ADDR);

    if (system_error != 0)
        *system_error = bno_read8(BNO055_SYS_ERR_ADDR);

     delay(200);

}
void bno_getCalibration(uint8_t *system, uint8_t *gyro, uint8_t *accel, uint8_t *mag) {

  uint8_t calData = bno_read8(BNO055_CALIB_STAT_ADDR);
  if (sys != NULL) {
    *sys = (calData >> 6) & 0x03;
  }
  if (gyro != NULL) {
    *gyro = (calData >> 4) & 0x03;
  }
  if (accel != NULL) {
    *accel = (calData >> 2) & 0x03;
  }
  if (mag != NULL) {
    *mag = calData & 0x03;
}

}
//todo: look at this one
double * bno_getVector(adafruit_vector_type_t vector_type) {
    uint8_t buffer[6];

    double xyz[3];
    memset(buffer,0,6);

    int16_t x, y, z;

    int i = 0;
    for (i = 0; i<6; i++) {
        buffer[i]=bno_read8(vector_type + i);
    }

    x = ((int16_t)buffer[0]) | (((int16_t)buffer[1]) << 8);
    y = ((int16_t)buffer[2]) | (((int16_t)buffer[3]) << 8);
    z = ((int16_t)buffer[4]) | (((int16_t)buffer[5]) << 8);

  case VECTOR_GYROSCOPE:
    /* 1dps = 16 LSB */
    xyz[0] = ((double)x) / 16.0;
    xyz[1] = ((double)y) / 16.0;
    xyz[2] = ((double)z) / 16.0;
    break;
  case VECTOR_EULER:
    /* 1 degree = 16 LSB */
    xyz[0] = ((double)x) / 16.0;
    xyz[1] = ((double)y) / 16.0;
    xyz[2] = ((double)z) / 16.0;
    break;
  case VECTOR_ACCELEROMETER:
    /* 1m/s^2 = 100 LSB */
    xyz[0] = ((double)x) / 100.0;
    xyz[1] = ((double)y) / 100.0;
    xyz[2] = ((double)z) / 100.0;
    break;
  case VECTOR_LINEARACCEL:
    /* 1m/s^2 = 100 LSB */
    xyz[0] = ((double)x) / 100.0;
    xyz[1] = ((double)y) / 100.0;
    xyz[2] = ((double)z) / 100.0;
    break;
  case VECTOR_GRAVITY:
    /* 1m/s^2 = 100 LSB */
    xyz[0] = ((double)x) / 100.0;
    xyz[1] = ((double)y) / 100.0;
    xyz[2] = ((double)z) / 100.0;
    break;
  }

return xyz;

}
int8_t bno_getTemp() {
    int8_t temp = (int8_t)(bno_read8(BNO055_TEMP_ADDR));
    return temp;
}
bool bno_isFullyCalibrated() {
    uint8_t system, gyro, accel, mag;
  getCalibration(&system, &gyro, &accel, &mag);

  switch (bno_mode) {
  case OPERATION_MODE_ACCONLY:
    return (accel == 3);
  case OPERATION_MODE_MAGONLY:
    return (mag == 3);
  case OPERATION_MODE_GYRONLY:
  case OPERATION_MODE_M4G: /* No magnetometer calibration required. */
    return (gyro == 3);
  case OPERATION_MODE_ACCMAG:
  case OPERATION_MODE_COMPASS:
    return (accel == 3 && mag == 3);
  case OPERATION_MODE_ACCGYRO:
  case OPERATION_MODE_IMUPLUS:
    return (accel == 3 && gyro == 3);
  case OPERATION_MODE_MAGGYRO:
    return (mag == 3 && gyro == 3);
  default:
    return (system == 3 && gyro == 3 && accel == 3 && mag == 3);
}
}
void bno_enterSuspendMode() {
      adafruit_bno055_opmode_t modeback = bno_mode;

  /* Switch to config mode (just in case since this is the default) */
  setMode(OPERATION_MODE_CONFIG);
  delay(25);
  bno_write8(BNO055_PWR_MODE_ADDR, 0x02);
  /* Set the requested operating mode (see section 3.3) */
  setMode(modeback);
delay(20);
    
}
void bno_enterNormalMode() {
    adafruit_bno055_opmode_t modeback = bno_mode;

  /* Switch to config mode (just in case since this is the default) */
  setMode(OPERATION_MODE_CONFIG);
  delay(25);
  bno_write8(BNO055_PWR_MODE_ADDR, 0x00);
  /* Set the requested operating mode (see section 3.3) */
  setMode(modeback);
delay(20);
}

uint8_t bno_read8(adafruit_bno055_reg_t reg) {
    return read8(BNO055_ADDRESS, reg);
}

uint8_t bno_write8(adafruit_bno055_reg_t reg, uint32_t value) {
    return write8(BNO055_ADDRESS, reg, value);

}