typedef struct {
   int16_t x;
   int16_t y;
   int16_t z;
} accel_t;

typedef struct { // TODO
   int16_t x;
   int16_t y;
   int16_t z;
} mag_t;

void lsm303dlh_acc_setup() {
   uint8_t buffer[2] = {CTRL_REG_4, 0x00};
   
   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_A,
      buffer,
      2,
      true
   );

   buffer[0] = CTRL_REG_1;
   buffer[1] = 0x27;
   
   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_A,
      buffer,
      2,
      false
   );

}


// function --------------------------------------------------------------
void lsm303dlh_mag_setup() {
   uint8_t buffer[2] = {MAG_CRA, 0x10}; // 15 Hz refreshrate
   
   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_B,
      buffer,
      2,
      true
   );

   buffer[0] = MAG_CRB;
   buffer[1] = 0xE0; // Gain - range +-8.1 Gauss, Gain X/Y and Z [LSB/Gauss] 230, GainZ [LSB/Gauss] 205
   
   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_B,
      buffer,
      2,
      true
   );

   buffer[0] = MAG_MR;
   buffer[1] = 0x00; // Continuous-conversion mode
   
   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_B,
      buffer,
      2,
      false
   );

}

// function --------------------------------------------------------------
void lsm303dlh_read_acc(accel_t *acc) {
   uint8_t buffer[6];
   int16_t accel[3];
   uint8_t reg = ACC_REG;

   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_A,
      &reg,
      1,
      true
   );

   i2c_read_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_A,
      buffer,
      6,
      false
   );

   // merge uint8_t to int16_t
   for (int i = 0; i < 3; i++) {
      accel[i] = ((buffer[i * 2] | buffer[(i * 2) + 1]  << 8));
   }

   acc->x = accel[0];
   acc->y = accel[1];
   acc->z = accel[2];
}

// function --------------------------------------------------------------
void lsm303dlh_read_mag(mag_t *mag) {
   uint8_t buffer[6];
   int16_t magnet[3];
   uint8_t reg = MAG_REG;

   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_B,
      &reg,
      1,
      true
   );

   i2c_read_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_B,
      buffer,
      6,
      false
   );

   // merge uint8_t to int16_t
   for (int i = 0; i < 3; i++) {
      magnet[i] = ((buffer[i * 2] << 8 | buffer[(i * 2) + 1] ));
   }

   mag->x = magnet[0];
   mag->y = magnet[1];
   mag->z = magnet[2];
}

void init_i2c_default() {
   i2c_init(I2C_PORT_MAGNETOMETER, I2C_BAUD * 1000);
   gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
   gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
   gpio_pull_up(I2C_SDA_PIN);
   gpio_pull_up(I2C_SCL_PIN);
}

float get_angle(int x, int y)
{
    float angle = 90 - atan2(y, x) * (180.0 / PI);
    // make sure the angle is always positive, and between 0 and 360 degrees
    if (angle < 0)
    {
        angle += 360.0;
    }
    return angle;
}
