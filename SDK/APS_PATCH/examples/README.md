- ## 示例工程：

| **第一级目录** | **第二级目录**            | **说明**                                                     |
| -------------- | ------------------------- | ------------------------------------------------------------ |
| bluetooth      |                           |                                                              |
|                | ancs                      | Apple   Notification Center Service  功能演示                |
|                | ble_adv                   | BLE  Slave设备发广播                                         |
|                | gatt_client               | GATT   Client 功能演示                                       |
|                | gatt_server               | GATT   Server 功能演示                                       |
|                | gatt_server_service_table | GATT   Server 服务表功能演示                                 |
| get_started    |                           |                                                              |
|                | hello_world               | 多任务间通信打印Hello   World 字符串输出                     |
|                | blink                     | 控制LED灯闪烁功能演示                                        |
|                | log                       | 展示如何打开/关闭固件内部模块以及用户任务的log输出信息       |
| peripheral     |                           |                                                              |
|                | auxadc                    | 辅助ADC使用例程                                              |
|                | gpio                      | Gpio设置演示                                                 |
|                | i2c                       | I2C   master 功能演示                                        |
|                | i2c_slave                 | I2C   slave 功能演示，和外部I2C   Master通信                 |
|                | pwm                       | PWM   端口设置展示                                           |
|                | spi_master                | SPI   master功能演示，访问Flash和外部SPI   slave设备         |
|                | timer_group               | Timer   功能演示                                             |
|                | uart                      | UART配置和通信演示                                           |
| protocols      |                           |                                                              |
|                | http_request              | http   访问example.com功能演示，基于CMSIS   RTOS             |
|                | http_request_freertos     | http   访问example.com功能演示，基于Free   RTOS              |
|                | tcp_client                | TCP   client 功能演示                                        |
|                | sntp_get_calender         | SNTP   获取日期的演示用例                                    |
|                | mdns                      | mDNS 业务广播演示用例                                        |
|                | mqtt                      | Mqtt 发布和订阅消息                                          |
|                | https_request             | https   访问www.howsmyssl.com功能演示，基于CMSIS   RTOS      |
| wifi           |                           |                                                              |
|                | iperf                     | 网络性能测试工具iperf的演示用例                              |
|                | wpa2_station              | AP为WPA2   加密模式，OPL1000作为   WIFI Station连接AP功能演示 |
|                | wpa2_station_gpio         | 在wpa2_station   示例基础上添加GPIO   外设的使用             |
| system         |                           |                                                              |
|                | ota_wifi                  | 基于WIFI的空中下载演示例程                                   |
|                | ali_blewifi               | 基于BLEWIFI的,OPL1000作为透传模块和阿里云之间发布/接收MQTT消息的演示例程 |
|                | ota_wifi                  | 读写FIM flash的演示例程                                      |