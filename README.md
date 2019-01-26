# sentry（哨兵代码）   v2.0
## 文件层次
+ Drivers(HAL库驱动层)
+ Middlewares（freertos层）
+ MDK-ARM （keil工程文件和编译文件）
+ User（用户层）
  - bsp（用户底层配置层）
  - drivers（用户模块设备层）
    * counters(控制器等算法)
    * data_struture(用户自定义数据结构)
    * modules（模块和外设）
  - control（控制层）
  - inc（应用层头文件）
  - src（应用层源文件）
+ Inc（主函数和中断层头文件）
+ Src（主函数和中断层源文件）
## 库说明
STM32CubeF4 Firmware Package V1.21.0 / 23-February-2018
## 系统支持
FreeTROS
## 开发板支持
RM新板RM,旧板,彬哥第一代板（f427IIHx）
## 代码命名规则说明
 函数名 &ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp; *大驼峰 如MyName*<br>
 普通变量  &ensp;  &ensp;  &ensp;  &ensp;&ensp;&ensp; *全小写 如my_name*<br>
 结构体声明  &ensp;  &ensp;  &ensp;  &ensp;&ensp;&ensp; *小驼峰 如myNameStruct*<br>
 结构体定义 &ensp;  &ensp;  &ensp;  &ensp; &ensp;&ensp;*小驼峰_t 如 myName_t*<br>
 (枚举联合体规则类似结构体)<br>
 指针类型 &ensp;  &ensp;  &ensp;  &ensp;&ensp;&ensp; *相应类型前加p 如普通变量指针类型pmy_name，结构体指针类型pmyName_t，其他类型类似*<br>
## 底层外设支持（bsp）
 串口空闲中断+DMA不定长度接收<br>
 can中断接收
##	模块设备支持（modules）
模块支持|模块名称
-|-
电机|6623  3508  2006
遥控|大疆遥控dbus
陀螺仪|GY555



数据结构和算法支持|算法名称
-|-
pid控制器|普通pid，模糊pid, 专家pid
数据结构|循环队列
滤波器|加权滑动平均滤波


