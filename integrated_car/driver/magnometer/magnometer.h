/**
 * @file magnometer.h
 * @brief Header file containing functions and structures for interacting with the accelerometer and magnetometer.
 */

/**
 * @brief Structure representing the x, y, and z values of an accelerometer reading.
 */
typedef struct {
   int16_t x;
   int16_t y;
   int16_t z;
} accel_t;

/**
 * @brief Structure representing the x, y, and z values of a magnetometer reading.
 */
typedef struct { 
   int16_t x;
   int16_t y;
   int16_t z;
} mag_t;

/**
 * @brief Function to set up the accelerometer.
 */
void lsm303dlh_acc_setup() {
   uint8_t buffer[2] = {CTRL_REG_4, 0x00};
   
   // Configuration write through I2C
   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_A,
      buffer,
      2,
      true
   );

   buffer[0] = CTRL_REG_1;
   buffer[1] = 0x27;
   
   // Apply configuration settings to write through I2C
   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_A,
      buffer,
      2,
      false
   );

}

/**
 * @brief Function to set up the magnetometer.
 */
void lsm303dlh_mag_setup() {
   uint8_t buffer[2] = {MAG_CRA, 0x10}; // 15 Hz refreshrate
   
   // Configuration write through I2C
   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_B,
      buffer,
      2,
      true
   );

   buffer[0] = MAG_CRB;
   // Gain - range +-8.1 Gauss, Gain X/Y and Z [LSB/Gauss] 230, GainZ [LSB/Gauss] 205
   buffer[1] = 0xE0; 
   
   // Configuration write through I2C with range values
   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_B,
      buffer,
      2,
      true
   );

   buffer[0] = MAG_MR;
   // Continuous-conversion mode
   buffer[1] = 0x00;
   
   // Configuration continuous conversion write through I2C
   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_B,
      buffer,
      2,
      false
   );

}

/**
 * @brief Function to read from the accelerometer.
 * @param acc Pointer to the accel_t structure to store the accelerometer readings.
 */
void lsm303dlh_read_acc(accel_t *acc) {

   // Initialise buffers and variables
   uint8_t buffer[6];
   int16_t accel[3];
   uint8_t reg = ACC_REG;

   // Write Register Address
   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_A,
      &reg,
      1,
      true
   );

   // Read accelerometer data
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

   // Set x, y and z values
   acc->x = accel[0];
   acc->y = accel[1];
   acc->z = accel[2];
}

/**
 * @brief Function to read from the magnetometer.
 * @param mag Pointer to the mag_t structure to store the magnetometer readings.
 */
void lsm303dlh_read_mag(mag_t *mag) {

   // Initialise buffers and variables
   uint8_t buffer[6];
   int16_t magnet[3];
   uint8_t reg = MAG_REG;

   // Write Register Address
   i2c_write_blocking(
      I2C_PORT_MAGNETOMETER,
      INTERFACE_B,
      &reg,
      1,
      true
   );

   // Read magnometer data
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

   // Set x, y and z values
   mag->x = magnet[0];
   mag->y = magnet[1];
   mag->z = magnet[2];
}

/**
 * @brief Function to initialize I2C communication for the magnetometer, as well as GPIO functions and pull-up.
 */
void init_i2c_default() {
   i2c_init(I2C_PORT_MAGNETOMETER, I2C_BAUD * 1000);
   gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
   gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
   gpio_pull_up(I2C_SDA_PIN);
   gpio_pull_up(I2C_SCL_PIN);
}

/**
 * @brief Function to calculate the angle based on x and y readings.
 * @param x The x-axis value.
 * @param y The y-axis value.
 * @return The calculated angle in degrees.
 */
float get_angle(int x, int y)
{
    // Calculate angle and change to degrees
    float angle = 90 - atan2(y, x) * (180.0 / PI);

    // make sure the angle is always positive, and between 0 and 360 degrees
    if (angle < 0)
    {
        angle += 360.0;
    }
    return angle;
}
