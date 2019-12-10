## CSP_HD Project  Review records

##### 2019-12-05  

yuxin  *yuxin@headingdata.com*;  gonglanjing  *gonglanjing@headingdata.com*;

yangle  *yangle@headingdata.com*;  zhangyichi  *zhangyichi@headingdata.com*;

duzhi  *duzhi@headingdata.com*;  zhouguangyue  *zhouguangyue@headingdata.com*;

weixun  *weixun@headingdata.com*;

- 单例模式destroy时，加入对instance是否为空的判断；
- 软件中所有使用NULL的地方替换成nullptr；
- csp timer模块中加入定时调用某个特定接口的机制。

------



##### 2019-12-09 

ligao  *ligao@headingdata.com*;

weixun  *weixun@headingdata.com*;

- 将嵌入式操作系统主要设定为Linux、QNX、Android、RTOS这四类；
- 操作系统和板级支持包的逻辑顺序进行调换，操作系统(OS)调整往上，板级支持包(BSP)调整往下；
- 硬件平台要可以适配不同的开发嵌入式开发板，所以各个模块要进行分类，哪些是必须项，哪些是可选项；
- 软件平台的网络客户端放在逻辑最上层，应用接口层放在逻辑最底层；
- 板级支持包uboot放在排序最前面，然后依次是kernel、rootfs；
- 硬件平台针对必须项和可选项，用不同的颜色进行标注；
- 添加软件平台模块间逻辑操作图。

