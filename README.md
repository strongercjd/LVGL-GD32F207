关于工程的说明

# 仓库说明

github的仓管地址：[github](https://github.com/strongercjd/LVGL-GD32F207)

gitee的仓库地址：[gitee](https://gitee.com/strongercjd/LVGL-GD32F207)

本仓库主要是基于GD32F207平台的LVGL学习

IAR版本：7.20

Keil版本：V5.25.2.0

QT版本：Qt Creator 4.12.2   Qt 5.14.2

# 关于example说明

example都是测试用例，比如将example\00-基础对象文件夹中的lvgl_app.c复制到\LVGL-APP文件夹中，覆盖员原来的文件，就可以在单片机和QT中看到运行结果。

如果**example中某个例子中有多个文件**，不仅仅有lvgl_app.c和lvgl_app.h，那么多出来的文件，就需要在IAR，Keil或QT的工程文件中添加这些文件。

# 文件夹说明

| 文件夹      | 说明                                                      |
| ----------- | --------------------------------------------------------- |
| GD32F207    | GD207的工程，包括Keil和IAR                                |
| Libraries   | GD207的标准外设库                                         |
| LVGL-APP    | lvgl的应用代码，主要供GD207和QT使用，两个平台共用一个代码 |
| LVGL-QT     | QT的模拟环境                                              |
| Third_Party | 第三方库存放地址，放lvgl源码                              |

