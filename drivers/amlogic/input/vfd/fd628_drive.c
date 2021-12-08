/* ***************************************************************************************** *
 *	公司名称	:		福大海矽微电子有限公司（FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD）
 *	创建人		：	袁文斌
 *	文件名		：	FD628_DRIVE.C
 *	功能描述	：	实现FD628的驱动
 *	其他说明	：	串行数据的传输从低位开始，FD628在串行通信的时钟上升沿读取数据，下降沿输出数据
 *	软件版本	：	V1B3（2012-10-17）
****************************************************************************************** */
#define FD628_Drive_GLOBALS

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/dma-mapping.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>
#include <linux/device.h>
#include <linux/spi/flash.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/input/vfd.h>
#include <linux/delay.h>
#include <linux/kernel.h>

#include	"fd628_drive.h"


/**
 * by leon@2016.04.18
 * GPIOY_13 => CLK
 * GPIOY_14 => DAT
 * GPIOY_15 => STB
 // 				 	OEN 		OUT 			IN
 // GPIOY_13~0  	bit[16:0]	bit[16:0]		bit[16:0]
 */
#define FD628_AML_GPIO_CLK 			(13)
#define FD628_AML_GPIO_DAT			(14)
#define FD628_AML_GPIO_STB			(15)
#define FD628_AML_GPIO_OEN 			P_PREG_PAD_GPIO1_EN_N
#define FD628_AML_GPIO_I 			P_PREG_PAD_GPIO1_I
#define FD628_AML_GPIO_O 			P_PREG_PAD_GPIO1_O


#ifdef CONFIG_VFD_FD628SW
INT8U FD628_DispData[14]={0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
/* 显示数据寄存器,调用FD628_WrDisp_AddrINC（）前，先将数据写入FD628_DispData[]的相应位置。*/
INT8U NEGA_Table[0x10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7c,0x39,0x5E,0x79,0x71};
									    /*  0    1    2    3    4    5    6    7    8    9    A    b    C    d    E		F */

static void FD628_Start( void );
static void FD628_Stop( void );
static void FD628_WrByte( INT8U dat );
//static INT8U  FD628_RdByte( void );
static void FD628_Command(INT8U CMD);
//static INT32U FD628_GetKey(void);
static BOOLEAN  FD628_WrDisp_AddrINC(INT8U Addr,INT8U DataLen );
//static BOOLEAN FD628_WrDisp_AddrStatic(INT8U Addr,INT8U DIGData );

#if 0
/****************************************************************
 *	函数的名称:			    set_stb_gpio
 *	描述:					设置STB口为输入或者输出
 *	参数：					void
 *	返回值:					void
****************************************************************/
static INT8U set_stb_gpio(INT8U value)
{
	if(value>0){
		// set OEN=0, output
		// static inline void aml_set_reg32_bits(void __iomem *_reg,const uint32_t _value,const uint32_t _start, const uint32_t _len)
		aml_set_reg32_bits(FD628_AML_GPIO_OEN, 0, FD628_AML_GPIO_STB, 1);
		//aml_clr_reg32_mask(P_AO_GPIO_O_EN_N,1<<6);
	}else{
		aml_set_reg32_bits(FD628_AML_GPIO_OEN, 1, FD628_AML_GPIO_STB, 1);
	}
	return 1;
}

/****************************************************************
 *	函数的名称:			    set_clk_gpio
 *	描述:					设置CLK口为输入或者输出
 *	参数：					void
 *	返回值:					void
****************************************************************/
static INT8U set_dio_gpio(INT8U value)
{
	if(value>0){
		aml_set_reg32_bits(FD628_AML_GPIO_OEN, 0, FD628_AML_GPIO_DAT, 1);
		//aml_clr_reg32_mask(P_AO_GPIO_O_EN_N,1<<5);
	}else{
		aml_set_reg32_bits(FD628_AML_GPIO_OEN, 1, FD628_AML_GPIO_DAT, 1);
		//aml_set_reg32_mask(P_AO_GPIO_O_EN_N,1<<5);
	}
	return 1;
}
#endif

void MDrv_FrontPnl_Init(void);

int (*vfd_set_stb_pin_value)(int value);
int (*vfd_set_clock_pin_value)(int value);
int (*vfd_set_do_pin_value)(int value);
int (*vfd_get_di_pin_value)(void);
/* 1为输出， 0为输入 */
int (*vfd_set_stb_gpio)(int value);
int (*vfd_set_dio_gpio)(int value);
int (*vfd_set_clk_gpio)(int value);

/* TODO: 初始化FD628，逐一设置使用的GPIO */
static int fd628_init(struct vfd_platform_data *pvfd_platform_data)
{
		vfd_set_stb_pin_value = pvfd_platform_data->set_stb_pin_value;
		vfd_set_clock_pin_value = pvfd_platform_data->set_clock_pin_value;
		vfd_set_do_pin_value = pvfd_platform_data->set_do_pin_value;
		vfd_get_di_pin_value = pvfd_platform_data->get_di_pin_value;
		vfd_set_stb_gpio = pvfd_platform_data->set_stb_gpio;
		vfd_set_dio_gpio = pvfd_platform_data->set_dio_gpio;
		vfd_set_clk_gpio = pvfd_platform_data->set_clk_gpio;
		MDrv_FrontPnl_Init();

		return 0;
}

static int set_FD628_led_value(char *display_code)
{
    int i = 0;
    char data;
    int dot = 0;

    for(i = 0; i < 4; i++)
    {
        data = display_code[i];
        if(data >= '0' && data <= '9')
            FD628_DispData[2*i] = NEGA_Table[data-'0'];
        else if(data >= 'a' && data <= 'f')
            FD628_DispData[2*i] = NEGA_Table[data-'a'+10];
        else if(data >= 'A' && data <= 'F')
            FD628_DispData[2*i] = NEGA_Table[data-'A'+10];
        else if(data == 'l' || data == 'L')
            FD628_DispData[2*i] = 0x38;
        else
            FD628_DispData[2*i] = NEGA_LED_NONE;
    }

    FD628_DispData[8] = 0;
    for(i=4 ;i< 11; i++)
    {
        data = display_code[i];
        if(data == '1')
            dot = 0X01<<(i-4);
        else
            dot = 0;

        FD628_DispData[8] |= dot;
    }

    printk("spink test ****set_FD628_led_value*******************\n");
    FD628_WrDisp_AddrINC(0x00,14);
    return 0;
}

#if 1 //def CONFIG_VFD_SM1628
int hardware_init(struct vfd_platform_data *pdev)
{
		int ret;
		ret = fd628_init(pdev);
		return ret;
}

int get_vfd_key_value(void)
{
		int key_value;
		key_value = 0;//get_sm1628_key_value();
		return key_value;
}

int set_vfd_led_value(char *display_code)
{
		int ret;
		ret = set_FD628_led_value(display_code);//FD628_WrDisp_AddrINC(0x00,14);//set_sm1628_led_value(display_code);
		return ret;
}
#endif

/* 通信接口的IO操作，与平台IO操作有关 */
#define FD628_STB_SET                                   vfd_set_stb_pin_value(1)          	/* 将STB设置为高电平 */
#define FD628_STB_CLR                                   vfd_set_stb_pin_value(0)          	/* 将STB设置为低电平 */
#define FD628_STB_D_OUT                                 vfd_set_stb_gpio(1)         		/* 设置STB为输出方向 */
#define FD628_CLK_SET                                   vfd_set_clock_pin_value(1)          /* 将CLK设置为高电平 */
#define FD628_CLK_CLR                                   vfd_set_clock_pin_value(0)          /* 将CLK设置为低电平 */
/* FIXME: 此处的clk设置 */
#define FD628_CLK_D_OUT                                 vfd_set_clk_gpio(1)         		/* 设置CLK为输出方向 */
#define FD628_DIO_SET                                   vfd_set_do_pin_value(1)          	/* 将DIO设置为高电平 */
#define FD628_DIO_CLR                                   vfd_set_do_pin_value(0)          	/* 将DIO设置为低电平 */
#define FD628_DIO_IN                                    vfd_get_di_pin_value()            	/* 当DIO设为输入方向时，读取的电平高低 */
#define FD628_DIO_D_OUT                                 vfd_set_dio_gpio(1)  				/* 设置DIO为输出方向 */
#define FD628_DIO_D_IN                                  vfd_set_dio_gpio(0)         		/* 设置DIO为输入方向 */
#define FD628_DELAY_1us                                 udelay(10)               			/* 延时时间 >1us*/

/****************************************************************
 *	函数的名称:			FD628_Start
 *	描述:						FD628通信的起始准备
 *	参数：					void
 *	返回值:					void
****************************************************************/
static void FD628_Start( void )
{
	FD628_STB_CLR;  				  /* 设置STB为低电平 */
	FD628_STB_D_OUT;				  /* 设置STB为输出方向 */
	FD628_CLK_D_OUT;				  /* 设置CLK为输出方向 */
	FD628_DELAY_STB;
}
/****************************************************************
 *	函数的名称:			FD628_Stop
 *	描述:						FD628通信的结束准备
 *	参数：					void
 *	返回值:					void
****************************************************************/
static void FD628_Stop( void )
{
	FD628_CLK_SET;						  /* 设置CLK为高电平 */
	FD628_DELAY_STB;
	FD628_STB_SET;  					  /* 设置STB为高电平 */
	FD628_DIO_SET;						  /* 设置DIO为高电平 */
	FD628_DIO_D_IN;						  /* 设置DIO为输入方向 */
	FD628_DELAY_BUF;					  /* 通信结束到下一次通信开始的间隔 */
}
/****************************************************************
 *	函数的名称:			FD628_WrByte
 *	描述:						向FD628写入一个字节的数据
 *	参数：					INT8U  发送的数据
 *	返回值:					void
 *	注意:						数据从低位到高位传输
****************************************************************/
static void FD628_WrByte( INT8U dat )
{
	INT8U i;				        			/* 移位控制变量 */
	FD628_DIO_D_OUT;		        	/* 设置DIO为输出方向 */
	for( i = 0; i != 8; i++ )	    /* 输出8 bit的数据 */
	{
		FD628_CLK_CLR;					  	/* 设置CLK为低电平 */
		if( dat & 0x01 ) 						/* 数据从低位到高位输出 */
		{
		    FD628_DIO_SET;		    	/* 设置DIO为高电平 */
		}
		else
		{
		    FD628_DIO_CLR;					/* 设置DIO为低电平 */
		}
  	FD628_DELAY_LOW;						/* 时钟低电平时间 */
		FD628_CLK_SET;							/* 设置SCL为高电平 上升沿写入*/
		dat >>= 1;									/* 输出数据右移一位，数据从低到高的输出 */
		FD628_DELAY_HIGH;          	/* 时钟高电平时间 */
	}
}
#if 0
/****************************************************************
 *	函数的名称:			FD628_RdByte
 *	描述:						从FD628读一个字节的数据
 *	参数：					void
 *	返回值:					INT8U 读到的数据
 *	注意:						数据从低位到高位传输
****************************************************************/
static INT8U  FD628_RdByte( void )
{
	INT8U i	,dat = 0;				 			/* 移位控制变量i;读取数据暂存变量dat */
	FD628_DIO_SET;		            /* 设置DIO为高电平 */
	FD628_DIO_D_IN;		       		  /* 设置DIO为输出方向 */
	for( i = 0; i != 8; i++ )	    /* 输出8 bit的数据 */
	{
		FD628_CLK_CLR;					  	/* 设置CLK为低电平 */
 	  FD628_DELAY_LOW;						/* 时钟低电平时间 */
		dat >>= 1;					 				/* 读入数据右移一位，数据从低到高的读入 */
		if( FD628_DIO_IN ) dat|=0X80;		/* 读入1 bit值 */
		FD628_CLK_SET;							/* 设置CLK为高电平 */
		FD628_DELAY_HIGH;          	/* 时钟高电平时间 */
	}
	return dat;						 				/* 返回接收到的数据 */
}
#endif
/****************************************FD628操作函数*********************************************/
/****************************************************************
 *	函数的名称:					    FD628_Command
 *	描述:										发送控制命令
 *	参数:		             		INT8U 控制命令
 *	返回值:				    	    void
****************************************************************/
static void FD628_Command(INT8U CMD)
{
	FD628_Start();
	FD628_WrByte(CMD);
	FD628_Stop();
}
/****************************************************************
 *	函数的名称:					    FD628_GetKey
 *	描述:										读按键码值
 *	参数:			             	void
 *	返回值:					        INT32U 返回按键值
 **************************************************************************************************************************************
返回的按键值编码
				| 0			| 0			| 0			| 0			| 0			| 0			| KS10	| KS9		| KS8		| KS7		| KS6		| KS5		| KS4		| KS3		| KS2		| KS1		|
KEYI1 	| bit15	| bit14	| bit13	| bit12	| bit11	| bit10	| bit9	| bit8	| bit7	| bit6	| bit5	| bit4	| bit3	| bit2	| bit1	| bit0	|
KEYI2 	| bit31	| bit30	| bit29	| bit28	| bit27	| bit26	| bit25	| bit24	| bit23	| bit22	| bit21	| bit20	| bit19	| bit18	| bit17	| bit16	|
***************************************************************************************************************************************/
#if 0
static INT32U FD628_GetKey(void)
{
	INT8U i,KeyDataTemp;
	INT32U FD628_KeyData=0;
	FD628_Start();
	FD628_WrByte(FD628_KEY_RDCMD);
	for(i=0;i!=5;i++)
	{
		KeyDataTemp=FD628_RdByte();					   /*将5字节的按键码值转化成2字节的码值*/
		if(KeyDataTemp&0x01)	 FD628_KeyData|=(0x00000001<<i*2);
		if(KeyDataTemp&0x02)	 FD628_KeyData|=(0x00010000<<i*2);
		if(KeyDataTemp&0x08)	 FD628_KeyData|=(0x00000002<<i*2);
		if(KeyDataTemp&0x10)	 FD628_KeyData|=(0x00020000<<i*2);
	}
	FD628_Stop();
	return(FD628_KeyData);
}
#endif
/****************************************************************
 *	函数的名称:					    FD628_WrDisp_AddrINC
 *	描述:										以地址递增模式发送显示内容
 *	参数:		         				INT8U Addr发送显示内容的起始地址；具体地址和显示对应的表格见datasheet
 *													INT8U DataLen 发送显示内容的位数
 *	返回值:				        	BOOLEAN；如果地址超出将返回1；如果执行成功返回0。
 *  使用方法：						先将数据写入FD628_DispData[]的相应位置，再调用FD628_WrDisp_AddrINC（）函数。
****************************************************************/
// 位顺序
INT8U realDGInum[14]=
{
    4,1,2,3,6,5,0,7,8,9,10,11,12,13
};
// 段顺序
INT8U realDGInum_bit[7]=
{
    0x10,0x08,0X04,0X02,0X01,0x40,0X20
};

INT8U gongyang_FD628_DispData[14];
/* *************************************************************************************************************************************
 *            a
 *         -------
 *        |       |
 *      f |       | b
 *         ---g---
 *        |       |	c
 *      e |       |
 *         ---d---   dp
 * *************************************************************************************************************************************** *
 *码字		| 0		| 1		| 2		| 3		| 4		| 5		| 6		| 7		| 8		| 9		| A		| b		| C 	| d		| E		| F		|
 *编码		|0x3F	|0x06	|0x5B	|0x4F	|0x66	|0x6D	|0x7D	|0x07	|0x7F	|0x6F	|0x77	|0x7c	|0x39	|0x5E	|0x79	|0x71	|
 ************************************************************************************************************************************* */

static void Get_gongyang_date(void)
{
    INT8U i;
    for(i=0;i<14;i++)
    {
        gongyang_FD628_DispData[i]= 0;
    }

    for(i =0;i< 5;i++)
    {
        gongyang_FD628_DispData[0]  |= ((FD628_DispData[realDGInum[i*2]] & realDGInum_bit[0])? 0x01:0);
        gongyang_FD628_DispData[2]  |= ((FD628_DispData[realDGInum[i*2]] & realDGInum_bit[1])? 0x01:0);
        gongyang_FD628_DispData[4]  |= ((FD628_DispData[realDGInum[i*2]] & realDGInum_bit[2])? 0x01:0);
        gongyang_FD628_DispData[6]  |= ((FD628_DispData[realDGInum[i*2]] & realDGInum_bit[3])? 0x01:0);
        gongyang_FD628_DispData[8]  |= ((FD628_DispData[realDGInum[i*2]] & realDGInum_bit[4])? 0x01:0);
        gongyang_FD628_DispData[10] |= ((FD628_DispData[realDGInum[i*2]] & realDGInum_bit[5])? 0x01:0);
        gongyang_FD628_DispData[12] |= ((FD628_DispData[realDGInum[i*2]] & realDGInum_bit[6])? 0x01:0);

        if(i<4)
        {
            gongyang_FD628_DispData[0]  <<= 0x01;
            gongyang_FD628_DispData[2]  <<= 0x01;
            gongyang_FD628_DispData[4]  <<= 0x01;
            gongyang_FD628_DispData[6]  <<= 0x01;
            gongyang_FD628_DispData[8]  <<= 0x01;
            gongyang_FD628_DispData[10] <<= 0x01;
            gongyang_FD628_DispData[12] <<= 0x01;
        }
    }

    printk("spink test *************Get_gongyang_date*******************\n");
}



static BOOLEAN  FD628_WrDisp_AddrINC(INT8U Addr,INT8U DataLen )
{
	INT8U i;
	if(DataLen+Addr>14) return(1);

    Get_gongyang_date();

	FD628_Command(FD628_ADDR_INC_DIGWR_CMD);
	FD628_Start();
	FD628_WrByte(FD628_DIGADDR_WRCMD|Addr);
	for(i=Addr;i!=(Addr+DataLen);i++)
	{
		//FD628_WrByte(FD628_DispData[i]);
		FD628_WrByte(gongyang_FD628_DispData[i]);
	}
    printk("spink test *************FD628_WrDisp_AddrINC*******************\n");
	FD628_Stop();
	return(0);
}
#if 0
/****************************************************************
 *	函数的名称:				FD628_WrDisp_AddrStatic
 *	描述:							以地址固定模式发送显示内容 ;地址表看datasheet
 *	参数:		          INT8U Addr发送显示内容的地址；
 *										INT8U DIGData 写入显示内容
 *	返回值:				    BOOLEAN；如果地址超出将返回1；如果执行成功返回0。
****************************************************************/
static BOOLEAN FD628_WrDisp_AddrStatic(INT8U Addr,INT8U DIGData )
{
	if(Addr>=14) return(1);
	FD628_Command(FD628_ADDR_STATIC_DIGWR_CMD);
	FD628_Start();
	FD628_WrByte(FD628_DIGADDR_WRCMD|Addr);
	FD628_WrByte(DIGData);
	FD628_Stop();
	return(0);
}
#endif


void Led_Show_lockflg(bool SingalStatus)
{
    if (SingalStatus)
    {
//        FP_Singal_LED_OnOff(1);
    }
    else
    {
//        FP_Singal_LED_OnOff(0);
    }
}






/****************************************************************
 *	函数的名称:				FD628_Init
 *	描述:							FD628初始化，用户可以根据需要修改显示
 *	参数:		          void
 *	返回值:				    void
****************************************************************/

static void FD628_Init(void )
{

    //init_gpio();

	FD628_CLK_SET;						  /* 设置CLK为高电平 */
	FD628_STB_SET;  					  /* 设置STB为高电平 */
	FD628_DIO_SET;						  /* 设置DIO为高电平 */
	FD628_STB_D_OUT;				  /* 设置STB为输出方向 */
	FD628_CLK_D_OUT;				  /* 设置CLK为输出方向 */
	FD628_DIO_D_OUT;						  /* 设置DIO为输入方向 */
	FD628_DELAY_BUF;					  /* 通信结束到下一次通信开始的间隔 */
	FD628_7DIG_MODE;
	FD628_Disp_Brightness_SET(FD628_DISP_NORMAL);

    FD628_DispData[0]=NEGA_LED_NONE;  //BIT4
	FD628_DispData[2]=NEGA_LED_NONE;   //BIT3
	FD628_DispData[4]=NEGA_LED_NONE;   //BIT2
	FD628_DispData[6]=NEGA_LED_NONE;   //BIT1
	FD628_DispData[8]=NEGA_LED_NONE;   //BIT0

	FD628_WrDisp_AddrINC(0x00,14);

    printk("spink test ****************FD628_Init *******************\n");
}

// Frontpanel API
void MDrv_FrontPnl_Init(void)
{
    printk("jack *******************   MDrv_FrontPnl_Init *******************\n");
    FD628_Init();
	//MDrv_FrontPnl_Update((char *)"8888", 0);
}

#endif
