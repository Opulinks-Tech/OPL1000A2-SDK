# 包含文件说明(V1.0)
1. **cm_env_config.exe**  :   Opulinks 的 wpa2_station 工程编辑，编译，和下载工具, 旨在为用户提供一款便捷的低功耗测量工具。
2. **cm_env_config.ini**  :     Opulinks current_measure 工具配置文件


# 修改配置文件cm_env_config.ini参数
1. **prj_folder 参数**：               wpa2_station工程所在的绝对路径，例如，E:\SL1000\SDK\sh_sdk2\A1\SDK\APS_PATCH\examples\wifi\wpa2_station\
2. **bin_work_dir 参数**：         包含binary文件的工作目录。该目录必须包含相应版本的opl1000_m0.bin和PatchData.txt文件，例如，E:\SL1000\ATE_develop\Patch_File\EVB\ 
3. **wifi_AP_SSID 参数**：         真实有效的 WiFi AP 的ssid. 例如，Opulinks-TEST-AP
4. **wifi_AP_pass 参数**：         连接上述 WiFi AP 的密码. 例如，12345678
5. **compiler_path 参数**：      用于编译wpa2_station工程的编译器所在的路径. 例如，C:\Keil_v5\UV4\UV4.exe
6. **uart_port 参数**：               用于下载opl1000.bin固件文件的真实有效的AT串口. 例如，COM12
7. **dtim_skip_count 参数**：  用于配置忽略的DTIM周期数。 可设置范围为1-100，例如，9
8. **rf_lp_mode 参数**：            配置OPL1000的工作模式. 有LP(Low Power mode)和HP(High Power mode) 两种模式可选，共支持下面四种输入：HP,LP,hp,和lp. 例如，LP

# 使用指南：
**cm_env_config.exe提供的功能如下**：

1. 读取并显示当前目录下的cm_env_config.ini 文件，等待用户的输入以确定是否需要更新配置；

2. 若用户选择更新上述8个参数，终端将会依次输出文本提示用户输入具体内容。对于每个参数，用户可直接回车表示不更新该参数，并且最多有两次输入该参数的机会，若两次输入值均不被接受，将会不更新该参数并继续；
3. 若上述参数有更新，所有的更新将会写回cm_env_config.ini 文件。之后将更新文件，编译工程，和生成相应的opl1000.bin固件文件；
4. 在最后下载opl1000.bin固件文件阶段，需要用户配合按复位按钮。请在终端输出提示信息之后的5秒内按复位键。若错过5秒的时间点，本次下载失败；然后显示是否再试一次的提示信息；
5. 下载过程完成后，该工具提示下载是否成功并显示所使用的配置参数的内容。

