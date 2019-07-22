# -*- coding: utf-8 -*-
from ctypes import *
import os

os.chdir('C:\\Users\\alvinchiang\\Desktop\\ATE_Opulink\\Opulink_ATEDLL')
#C:\\Users\alvinchiang\Desktop\ATE_Opulink\testconsole
myDll = windll.LoadLibrary('opulink_ate.dll')

status= myDll.ATE_Init();
print status
print myDll.ATE_IQ_Connect();

myDLL.ATE_Test_WIFI_Tx(2)



