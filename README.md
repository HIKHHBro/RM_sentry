# RM_freetrtos_templates_HAL
## 文件层次
- Drivers(HAL库驱动层)
- Middlewares（freertos层）
- MDK-ARM （keil工程文件和编译文件）
- User（用户层）
  * bsp（用户底层配置层）
  * drivers（用户模块设备层）
  * control（控制层）
  * inc（应用层头文件）
  * src（应用层源文件）
- Inc（主函数和中断层头文件）
- Src（主函数和中断层源文件）
## 库说明
STM32CubeF4 Firmware Package V1.21.0 / 23-February-2018
## 开发板支持
RM新版和旧版（f427IIHx）
## 代码命名规则说明
 函数名   nbsp                       *大驼峰 如MyName*<br>
 普通变量     &nbsp                   *全小写 如my_name*<br>
 结构体声明                      *小驼峰 如myNameStruct*<br>
 结构体定义                      *小驼峰_t 如 myName_t*<br>
 (枚举联合体规则类似结构体)<br>
 指针类型                        *相应类型前加p 如普通变量指针类型pmy_name，结构体指针类型pmyName_t，其他类型类似*<br>
