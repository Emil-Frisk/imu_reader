# Description
This project is made for reading and fusing multiple IMU sensor data for a miniature excavator model. The IMU reader uses Raspberry Pi Pico and communication is done through I2C. Maxiumum read frequency is 160 Hz.


 **Note:** Incomplite implementation of BNO08X-sensor reading. SPI communication doesn't work yet. Reading sensor data via I2C works only speed of 10hz. 
# Architecture

![Architecture](imgs/architecture.png)

# Constributors
- [Emil](https://github.com/https://github.com/Emil-Frisk)
- [Vann](https://github.com/vann1)