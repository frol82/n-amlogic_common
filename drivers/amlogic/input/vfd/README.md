
### GPIO函数位置

```
    $: yixiaoyang@[/devel/git/osc/mlogic905/common] % grep -anR 'aml_clr_reg32_mask' ./
    ./include/linux/amlogic/gpio-amlogic.h:102:static inline void aml_clr_reg32_mask(void __iomem *_reg,
    static inline void aml_clr_reg32_mask(void __iomem *_reg,
            const uint32_t _mask)
    {
        writel_relaxed((readl_relaxed(_reg) & (~(_mask))), _reg);
    }


    yixiaoyang@[/devel/git/osc/mlogic905/common] % grep -anR 'P_AO_GPIO_O_EN_N' ./           
    ./drivers/amlogic/dvb_tv/c_stb_regs_define.h:6361:#define P_AO_GPIO_O_EN_N              AOBUS_REG_ADDR(AO_GPIO_O_EN_N)

    #define CBUS_REG_ADDR(reg)  ((IO_CBUS_BASE << OFFSET) + (reg << 2))

```

```
./drivers/amlogic/hdmi/hdmi_tx_14/ts/mach_reg.h:23:#define CBUS_REG_ADDR(reg)  ((IO_CBUS_BASE << OFFSET) + (reg << 2))
./drivers/amlogic/hdmi/hdmi_tx_14/ts/mach_reg.h:24:#define VCBUS_REG_ADDR(reg) ((IO_APB_BUS_BASE << OFFSET) + (0x100000+(reg<<2)))
22 #define OFFSET  24
23 #define CBUS_REG_ADDR(reg)  ((IO_CBUS_BASE << OFFSET) + (reg << 2))
24 #define VCBUS_REG_ADDR(reg) ((IO_APB_BUS_BASE << OFFSET) + (0x100000+(reg<<2)))
25 #define AOBUS_REG_ADDR(reg) ((IO_AOBUS_BASE << OFFSET) + reg)
26 #define APB_REG_ADDR(reg)   ((IO_APB_BUS_BASE << OFFSET) + reg)
```


```
./include/linux/amlogic/iomap.h
22 enum{
23     IO_CBUS_BASE = 0,
24     IO_APB_BUS_BASE,
25     IO_AOBUS_BASE,
26     IO_HIUBUS_BASE,
27     IO_BUS_MAX,
28 };
```

```
/****************************************************************
*	函数的名称:			    set_stb_gpio
*	描述:					设置STB口为输入或者输出
*	参数：					void
*	返回值:					void
****************************************************************/
static INT8U set_stb_gpio(INT8U value)
{
    if(value>0)
    aml_clr_reg32_mask(P_AO_GPIO_O_EN_N,1<<6);
    return 1;
}

6360 #define AO_GPIO_O_EN_N ((0x00 << 10) | (0x09 << 2))
6361 #define P_AO_GPIO_O_EN_N        AOBUS_REG_ADDR(AO_GPIO_O_EN_N)

即将P_AO_GPIO_O_EN_N 0xc8100 0000 + 0x09*4地址置位，即GPIOAO_OEN[13:0]+OUT[29:16]置位


原设置为GPIOX_1,GPIOX_2,GPIOX_11， GPIOAO_4,GPIOAO_5,GPIOAO_6（STB接口）
//$$ MATCH "vfd_pin_0_match" = "&vfd_pins"
//$$ L2 PROP_U32 2 = "amlogic,setmask"
//$$ L2 PROP_STR = "amlogic,pins"
                    vfd_pins:vfd_pin{
                    amlogic,clrmask = <8 0x30
                                        5 0x6000
                                        2 0x8
                                        3 0x100000
                                        7 0x40000000>;
                    amlogic,pins="GPIOX_1,GPIOX_2,GPIOX_11";
            };
//$$ MATCH "vfd_pin_0_match" = "&vfd_pins"
//$$ L2 PROP_U32 2 = "amlogic,setmask"
//$$ L2 PROP_STR = "amlogic,pins"
//                       vfd_pins:vfd_pin{
//                       amlogic,clrmask = <1 0x2000
//                                          10 0x1852066>;
//                       amlogic,pins="GPIOAO_4,GPIOAO_5,GPIOAO_6";
//                 };

```


所以
```
22 enum{
23     IO_CBUS_BASE = 0,
24     IO_APB_BUS_BASE,
25     IO_AOBUS_BASE,
26     IO_HIUBUS_BASE,
27     IO_BUS_MAX,
28 };

22 #define OFFSET  24
23 #define CBUS_REG_ADDR(reg)  ((IO_CBUS_BASE << OFFSET) + (reg << 2))
24 #define VCBUS_REG_ADDR(reg) ((IO_APB_BUS_BASE << OFFSET) + (0x100000+(reg<<2)))
25 #define AOBUS_REG_ADDR(reg) ((IO_AOBUS_BASE << OFFSET) + reg)
26 #define APB_REG_ADDR(reg)   ((IO_APB_BUS_BASE << OFFSET) + reg)


42 #define PREG_PAD_GPIO6_EN_N 0x2008 => 8020
43 #define P_PREG_PAD_GPIO6_EN_N   CBUS_REG_ADDR(PREG_PAD_GPIO6_EN_N)
44 #define PREG_PAD_GPIO6_O 0x2009 => 8024
45 #define P_PREG_PAD_GPIO6_O      CBUS_REG_ADDR(PREG_PAD_GPIO6_O)
46 #define PREG_PAD_GPIO6_I 0x200a => 8028
47 #define P_PREG_PAD_GPIO6_I      CBUS_REG_ADDR(PREG_PAD_GPIO6_I)
```

实际上地址为 (1 << 24) + (reg << 2) => (1 << 24) + (0x2008<<2) => 0x10 00008020




```
Combining NOTICE files into HTML
Combining NOTICE files into text
make -C common O=../out/target/product/p201/obj/KERNEL_OBJ ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- meson64_defconfig
make[1]: Entering directory `/devel/android/mlogic905/common'
  GEN     /devel/android/mlogic905/out/target/product/p201/obj/KERNEL_OBJ/Makefile
#
# configuration written to .config
#
make[1]: Leaving directory `/devel/android/mlogic905/common'
make Image
make -C common O=../out/target/product/p201/obj/KERNEL_OBJ ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-
make[1]: Entering directory `/devel/android/mlogic905/common'
  GEN     /devel/android/mlogic905/out/target/product/p201/obj/KERNEL_OBJ/Makefile
scripts/kconfig/conf --silentoldconfig Kconfig
  Using /devel/android/mlogic905/common as source for kernel
  GEN     /devel/android/mlogic905/out/target/product/p201/obj/KERNEL_OBJ/Makefile
  CHK     include/generated/uapi/linux/version.h
  CHK     include/generated/utsrelease.h
  CALL    /devel/android/mlogic905/common/scripts/checksyscalls.sh
<stdin>:1232:2: warning: #warning syscall renameat2 not implemented [-Wcpp]
  CHK     include/generated/compile.h
  GZIP    kernel/config_data.gz
  CHK     kernel/config_data.h
  UPD     kernel/config_data.h
  CC      kernel/configs.o
  LD      kernel/built-in.o
  CC      drivers/amlogic/input/vfd/vfd.o
/devel/android/mlogic905/common/drivers/amlogic/input/vfd/vfd.c:42:26: fatal error: mach/am_regs.h: No such file or directory
 #include <mach/am_regs.h>
                          ^
compilation terminated.
make[6]: *** [drivers/amlogic/input/vfd/vfd.o] Error 1
make[5]: *** [drivers/amlogic/input/vfd] Error 2
make[4]: *** [drivers/amlogic/input] Error 2
make[3]: *** [drivers/amlogic] Error 2
make[2]: *** [drivers] Error 2
make[1]: *** [sub-make] Error 2
make[1]: Leaving directory `/devel/android/mlogic905/common'
make: *** [out/target/product/p201/obj/KERNEL_OBJ/arch/arm64/boot/Image.lzo] Error 2

#### make failed to build some targets (10:37 (mm:ss)) ####
```
