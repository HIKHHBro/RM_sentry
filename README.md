# sentry（哨兵代码）v2.0
## 文件层次

* Drivers(HAL库驱动层)
* Middlewares(freertos层)
* MDK-ARM （keil工程文件和编译文件）
* User（用户层）
  + bsp (用户底层配置层)
  + drivers（用户模块设备层）
    - counters(控制器等算法)
    - data_struture(用户自定义数据结构)
    - modules（模块和外设）
  + app（用户层控制层）
  + task(用户任务层)
* Inc（主函数和中断层头文件）
* Src（主函数和中断层源文件）

## 库说明

STM32CubeF4 Firmware Package V1.19.0
## 系统支持
FreeRTOS
#### 系统用户接口
 userFreeRTOSConfig.h
## 开发板支持

RM新板,RM旧板,彬哥第一代板（f427IIHx）

## BSP层接口支持(userdriverconfig.h)
外设|引脚宏接口|端口接口
-|-|-
LED|LED_x(x=1,2,3…)|LED_GPIO
蜂鸣器|BUZZER|BUZZER_GPIO
电源管理|POWER_x(x=1,2,3…)|POWER_GPIO
KEY|KEY_x(x=1,2,3…)|KEY_GPIO
激光|LASER|LASER_GPIO
## 代码命名规则说明

类型|命名规则|示例
-|-|-
 函数名|大驼峰|MyName
 普通变量|全小写,连接加下划线|my_name
 结构体声明|小驼峰加后缀Struct|myNameStruct
 结构体定义|小驼峰加后缀_t|myName_t
 枚举声明|小驼峰加后缀Enum|myNameEnum
 枚举定义|小驼峰加后缀_e|myName_e
 指针类型|相应类型加前缀p|普通变量指针类型pmy_name，结构体指针类型pmyName_t

## 底层外设支持（bsp）

外设|说明
-|-
串口|串口空闲中断+DMA+消息队列不定长度接收
can|can中断+消息队列接收

## 模块设备支持（modules）

模块设备|模块名称
-|-
电机|6623  3508  2006 maxion 6020
遥控|大疆遥控dbus
陀螺仪|GY555
上位机|匿名v2.6 平衡小车之家
编码器|增量AB相欧姆龙 E6A2-CW3C
蓝牙|HC05
超声波|HC-S04

## 数据结构和算法（data_struture，counters）

数据结构和算法|算法名称
-|-
pid控制器|普通pid，模糊pid, 专家pid
数据结构|循环队列
滤波器|加权滑动平均滤波


## 控制流程手稿
  1. 所有任务初始化，然后挂起，进入模块自检，判断初始化
     1. 如果失败，提示堆栈溢出等
     2. 进入数据解析，数据接收正常判断 
        1. 如果重要模块离线，则蜂鸣器报警和led灯提示，然后创建oled显示错误任务，打印错误信息,挂起所有任务,删除系统初始化任务
        2. 如果次要模块离线，则关闭相应功能(led提示响应错误)
     3. 创建数据校准任务
     4. 删除系统初始化任务
     * 最大总任务数(系统初始化，解析底盘，云台，数据校准或(oled显示))5个
     * 最大在线任务(系统初始化，解析，oled显示)2个
  2. 进入数据校准任务，检查哪个模块正常并且选择
     1. 手动对位校准 (遥控选择left =1)
        1. 遥控碰边，轻触开关时间持续t ms，设置编码器零点
        2. 以恒定速度回跑半米，检验激光测距和超声波测距，和编码器的值相比较
           1. 距离从小到大，判断其变化率是否稳定，各采取50个数据，数据间隔为t，求出变化率的平均值，再求方差，取方差小的，即波动性小
           2. 然后再回跑半米，从大到小
        3. 开启所有任务，删除校准任务
     2. 自动对位校准(left=2)
         1. 以恒定速度回跑半米，检验激光测距和超声波测距，和编码器的值相比较
            1. 距离从小到大，判断其变化率是否稳定，各采取50个数据，数据间隔为t，求出变化率的平均值，再求方差，取方差小的，即波动性小
            2. 然后再回跑半米，从大到小
         2. 到达柱子，碰到轻触开关并且持续tms，设置编码器零点
         3. 开启所有任务，删除校准任务，
            1. 如果跑飞，数据不对，马上切换遥控自检
     3. 开启所有任务，删除自动自检任务
     4. 如果没有剩余时间，开启紧急模式，删除校准任务，生成随机数，在过道弯道中间随机速度跑
     * 最大总任务数()
     * 最大在线任务()
  2. 自动控制
  3. 跑飞检测和控制


## 模块功能要求
  1. 编码器，可变方向增加记数

## 调试问题记录
  **Data**  : *2019-02-23 01:25*  
  **author**: *HIKpingtouge*  
  **log**   :  
  1. 串口1接受不正常，遥控的值跳动严重  
     1. 换老旧版本的库就可以，1.24.0版库不行，神他妈坑爹  
     2. can 发送失败，发送值大于600，就发送失败，解决办法同上，（曾经裸机代码的时候也遇到这个问题，待测试用1.24.0配置的can是否能发送）   
  **Data**  : *2019-03-20 01:25*  
  **author**: *HIKpingtouge*  
  **log**   :  
  1. can接一个电机正常，接上多个电机，就发送不正常  
    1. 多个can接收函数放在一个接收任务中，使得can接收严重掉帧，所以使得电机看起来不正常  
  **Data**  : *2019-03-22 01:25*    
  **author**: *HIKpingtouge*    
  **log**   :    
  1. 编写完拨弹处理    
  **Data**  : *2019-03-23 01:25*    
  **author**: *HIKpingtouge*    
  **log**   :    
  1. 解决yaw轴云台以恒定速度转到是反弹问题    
    1. 问题1: 编码器的值累加判断写错，导致不能正常判断反向记数    
    2. 问题2：计算误差函数的过零点处理判断被编译器优化掉    
  2. 和视觉协商新的数据通信方案    
