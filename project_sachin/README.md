# FPGA_Lab_Project

This project aims to receive signal samples over WiFi into onboard-ESP32 and then send it to ARM processor on Vaman to perform two signal processing operations:

* Convolution
* Correlation

After these operations, the output samples are sent back to ESP32 over UART and displayed on the webserver.

For visualization purpose, A sample x[n] h[n] and y[n] is as shown below:

![alt text](https://github.com/sachinomdubey/FPGA_Lab/blob/main/project_sachin/Figures/plot_fpga_lab.jpg)

