# 包含文件说明
1. opl_mp_win10.exe  : Opulinks量产工具Win10版本
   opl_mp_winxp.exe  : Opulinks量产工具Win XP版本 
2. mp_multi_download.ini  : Opulinks多设备固件下载程序配置文件
3. OPL1000-multiple-dev-download-tool-user-guide.pdf： 使用说明文档
4. Mega2560_no_relay.pdf ： Arduino 控制板电路图，直接使用PWM输出复位信号。
5. opl1000_ota.bin ：用于示例下载的固件文件，实际使用时用客户自己的固件Bin文件，并修改ini文件里面的file参数。
6. mp_mega2560.pde： Arduino Mega2560 固件，需要烧录到Mega2560板子中。
7. readme.md： 本说明文档

# 修改配置文件mp_multi_download.ini参数
1. 修改 file 参数 给出固件文件所在路径和文件名。
2. 定义下载串口的工作波特率。有四个速率可选，分别为115200的1,2,4,8倍。 
3. 根据实际连接定义 Arduino 控制板控制串口和监控串口编号。
   注意控制串口波特率 control_baudrate 固定为115200bps, 监控串口波特率monitor_baudrate固定为9600bps。
4. 给出夹具上板子的串口编号和对应的控制通道。如何定义参考“使用说明文档”。
5. 定义BLE_MAC 和WIFI_MAC 两个Section MAC 起始和终止地址。

# 平台所使用设备购买链接：
1． USB 转UART 转接器。推荐使用 USB 转4 UART 板子。当夹具放置10个设备时需要4个这样的转接板。
    淘宝链接：
    https://item.taobao.com/item.htm?spm=a1z09.2.0.0.65192e8d1Ed2gW&id=547175165933&_u=k1pcde1d0b5
2． Arduino MEGA2560 主板
    https://item.taobao.com/item.htm?spm=a230r.1.14.47.5855114bzW70xe&id=569798964993&ns=1&abbucket=14#detail
3.  Arduino MEGA2560 扩展板
    https://item.taobao.com/item.htm?spm=a1z09.2.0.0.53592e8dZUdsTh&id=529407009307&_u=51pcde1fa58


