/* ***************************************************************************************** *
 *	公司名称	:	福大海矽微电子有限公司（FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD）
 *	创建人		：	袁文斌
 *	文件名		：	FD628_DRIVE.C
 *	功能描述	：	FD628驱动的头文件，需要移植修改和调用的文件
 *	其他说明	：	串行数据的传输从低位开始，FD628在串行通信的时钟上升沿读取数据，下降沿输出数据
 *	软件版本	：	V1B3（2012-10-17）
****************************************************************************************** */
#ifndef __FD628_Drive_H__
#define __FD628_Drive_H__

#define FD628_Drive_EXT

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
#define		NEGA_LED_NONE 0X00
#define		NEGA_LED_0 0X3F
#define		NEGA_LED_1 0x06
#define		NEGA_LED_2 0x5B
#define		NEGA_LED_3 0x4F
#define		NEGA_LED_4 0x66
#define		NEGA_LED_5 0X6d
#define		NEGA_LED_6 0x7D
#define		NEGA_LED_7 0x07
#define		NEGA_LED_8 0x7f
#define		NEGA_LED_9 0x6F

#define		NEGA_LED_A 0X77
#define		NEGA_LED_b 0x7c
#define		NEGA_LED_C 0X39
#define		NEGA_LED_c 0X58
#define		NEGA_LED_d 0x5E
#define		NEGA_LED_E 0X79
#define		NEGA_LED_e 0X7b
#define		NEGA_LED_F 0x71

#define		NEGA_LED_I 0X60
#define		NEGA_LED_L 0X38
#define		NEGA_LED_r 0X72
#define		NEGA_LED_n 0X54
#define		NEGA_LED_N 0X37
#define		NEGA_LED_O 0X3F
#define		NEGA_LED_P 0XF3
#define		NEGA_LED_S 0X6d
#define		NEGA_LED_y 0X6e
#define		NEGA_LED__ 0x08

/* **************************************API*********************************************** */

/* *************************用户需要修改部分************************** */
typedef unsigned char           BOOLEAN;       /* 布尔变量布尔变量*/
typedef unsigned char  			INT8U;         /* 无符号8位数*/
typedef unsigned int  			INT32U;        /* 无符号32位数*/

/* **************按键名和对应码值定义********************** */
#define FD628_KEY10 	0x00000200
#define FD628_KEY9 		0x00000100
#define FD628_KEY8 		0x00000080
#define FD628_KEY7 		0x00000040
#define FD628_KEY6  	0x00000020
#define FD628_KEY5 		0x00000010
#define FD628_KEY4  	0x00000008
#define FD628_KEY3  	0x00000004
#define FD628_KEY2  	0x00000002
#define FD628_KEY1  	0x00000001
#define FD628_KEY_NONE_CODE 0x00

#define FD628_DELAY_KEY_SCAN             //延时10ms
#define FD628_DISP_NORMAL	 (FD628_DISP_ON|FD628_Brightness_8 )


/* *************************用户不需要修改部分************************** */
/* ************** 控制FD628的宏 ********************** */
#define FD628_4DIG_MODE 					FD628_Command(FD628_4DIG_CMD)						/*设置FD628工作在4位模式*/
#define FD628_5DIG_MODE 					FD628_Command(FD628_5DIG_CMD)						/*设置FD628工作在5位模式*/
#define FD628_6DIG_MODE 					FD628_Command(FD628_6DIG_CMD)						/*设置FD628工作在6位模式*/
#define FD628_7DIG_MODE 					FD628_Command(FD628_7DIG_CMD)						/*设置FD628工作在7位模式*/
#define FD628_Disp_Brightness_SET(Status)	FD628_Command(FD628_DISP_STATUE_WRCMD |(Status&0x0f))   	/*设置FD628的显示方式（亮度和显示开关）*/


/* *************************************************************************************************************************************** *
*	Status说明	| bit7	| bit6	| bit5	| bit4	| bit3			| bit2	| bit1	| bit0	| 		 Display_EN：显示使能位，1：打开显示；0：关闭显示
*				| 0		| 0		| 0		| 0		| Display_EN	|	brightness[3:0]		|		 brightness：显示亮度控制位，000～111 分别代表着1（min）～8（max）级亮度
* ************************************************************************************************************************************* */
/* ************** Status可以使用下面的宏 （之间用或的关系） ************ */
#define FD628_DISP_ON        					0x08		/*打开FD628显示*/
#define FD628_DISP_OFF        				0x00		/*关闭FD628显示*/

#define FD628_Brightness_1        				0x00		/*设置FD628显示亮度等级为1*/
#define FD628_Brightness_2        				0x01		/*设置FD628显示亮度等级为2*/
#define FD628_Brightness_3        				0x02		/*设置FD628显示亮度等级为3*/
#define FD628_Brightness_4        				0x03		/*设置FD628显示亮度等级为4*/
#define FD628_Brightness_5        				0x04		/*设置FD628显示亮度等级为5*/
#define FD628_Brightness_6        				0x05		/*设置FD628显示亮度等级为6*/
#define FD628_Brightness_7        				0x06		/*设置FD628显示亮度等级为7*/
#define FD628_Brightness_8        				0x07		/*设置FD628显示亮度等级为8*/

#define	FD628_WAIT_KEY_FREE		 		while(FD628_GetKey()!=FD628_KEY_NONE_CODE);		//等待按键释放
#define	FD628_WAIT_KEY_PRESS			while(FD628_GetKey()==FD628_KEY_NONE_CODE);		//等待按键按下													 									//按键扫描时间 20ms
/* ****************** 函数 ************************** */
/****************************************************************
 *	函数的名称:					    FD628_Command
 *	描述:							发送控制命令
 *	参数:		             		INT8U 控制命令
 *	返回值:				    	    void
****************************************************************/
//FD628_Drive_EXT		void FD628_Command(INT8U);
/***************************************************************
 *	函数的名称:					    FD628_GetKey
 *	描述:										读按键码值
 *	参数:			             	void
 *	返回值:					        INT8U 返回按键值
 **************************************************************************************************************************************
返回的按键值编码
			| 0			| 0			| 0			| 0			| 0			| 0			| KS10	| KS9		| KS8		| KS7		| KS6		| KS5		| KS4		| KS3		| KS2		| KS1		|
KEY1 	| bit15	| bit14	| bit13	| bit12	| bit11	| bit10	| bit9	| bit8	| bit7	| bit6	| bit5	| bit4	| bit3	| bit2	| bit1	| bit0	|
KEY2 	| bit31	| bit30	| bit29	| bit28	| bit27	| bit26	| bit25	| bit24	| bit23	| bit22	| bit21	| bit20	| bit19	| bit18	| bit17	| bit16	|
***************************************************************************************************************************************/
//FD628_Drive_EXT		INT32U FD628_GetKey();
/****************************************************************
 *	函数的名称:					    FD628_WrDisp_AddrINC
 *	描述:										以地址递增模式发送显示内容
 *	参数:		         				INT8U Addr发送显示内容的起始地址；具体地址和显示对应的表格见datasheet
 *													INT8U DataLen 发送显示内容的位数
 *	返回值:				        	BOOLEAN；如果地址超出将返回1；如果执行成功返回0。
 *  使用方法：						先将数据写入FD628_DispData[]的相应位置，再调用FD628_WrDisp_AddrINC（）函数。
****************************************************************/
//FD628_Drive_EXT		BOOLEAN FD628_WrDisp_AddrINC(INT8U,INT8U)	;
/****************************************************************
 *	函数的名称:				FD628_WrDisp_AddrStatic
 *	描述:							以地址固定模式发送显示内容 ;地址表看datasheet
 *	参数:		          INT8U Addr发送显示内容的地址；
 *										INT8U DIGData 写入显示内容
 *	返回值:				    BOOLEAN；如果地址超出将返回1；如果执行成功返回0。
****************************************************************/
//FD628_Drive_EXT		BOOLEAN FD628_WrDisp_AddrStatic(INT8U,INT8U );
/****************************************************************
 *	函数的名称:				FD628_Init
 *	描述:							FD628初始化，用户可以根据需要修改显示
 *	参数:		          void
 *	返回值:				    void
****************************************************************/
//FD628_Drive_EXT 	void FD628_Init(void);
//FD628_Drive_EXT		INT8U	FD628_DispData[14]; /* 显示数据寄存器,调用FD628_WrDisp_AddrINC（）前，先将数据写入FD628_DispData[]的相应位置。*/
//FD628_Drive_EXT		code  INT8U NEGA_Table[0x10];	/* 共阴数码管码值编码数组，依次对应的显示：0,1,2,3,4,5,6,7,8,9,A,b,C,d,E,F  */
/* ****************************************************************************************************** */




/* ************************************* *Drive 模块* ********************************************** */
#ifdef   FD628_Drive_GLOBALS //内部变量和函数
#


/* ************************************ *用户需要修改部分* *************************************** */
/* 定义通信接口的IO连接,与实际电路有关 */
//  	  FD628_STB				;	  //FD628通信选通
//  	  FD628_CLK				;	  //FD628通信时钟
//  	  FD628_DIO				;	 	//FD628通信数据
/* 通信接口的IO操作，与平台IO操作有关 */
#if 0
#define		FD628_STB_SET			            					   	/* 将STB设置为高电平 */
#define		FD628_STB_CLR			            					  /* 将STB设置为低电平 */
#define		FD628_STB_D_OUT						 			    	    /* 设置STB为输出方向 */
#define		FD628_CLK_SET				             					  /* 将CLK设置为高电平 */
#define		FD628_CLK_CLR				               						/* 将CLK设置为低电平 */
#define		FD628_CLK_D_OUT					  				  	   		/* 设置CLK为输出方向 */
#define		FD628_DIO_SET					               						/* 将DIO设置为高电平 */
#define		FD628_DIO_CLR				             						/* 将DIO设置为低电平 */
#define		FD628_DIO_IN				            							/* 当DIO设为输入方向时，读取的电平高低 */
#define		FD628_DIO_D_OUT  										        /* 设置DIO为输出方向 */
#define		FD628_DIO_D_IN   										        /* 设置DIO为输入方向 */
#define 	FD628_DELAY_1us											    	/* 延时时间 >1us*/
#endif

/* **************************************用户不需要修改*********************************************** */
/* **************写入FD628延时部分（具体含义看Datasheet）********************** */
#define 	FD628_DELAY_LOW		     	FD628_DELAY_1us                     		        /* 时钟低电平时间 >500ns*/
#define		FD628_DELAY_HIGH     	 	FD628_DELAY_1us 	   										 				/* 时钟高电平时间 >500ns*/
#define  	FD628_DELAY_BUF		 		 	FD628_DELAY_1us	                     				  	/* 通信结束到下一次通信开始的间隔 >1us*/
#define  	FD628_DELAY_STB					FD628_DELAY_1us
#endif
/* ***********************写入FD628操作命令***************************** */
#define FD628_KEY_RDCMD        					0x42                //按键读取命令
#define FD628_4DIG_CMD        				0x00		/*设置FD628工作在4位模式的命令*/
#define FD628_5DIG_CMD        				0x01		/*设置FD628工作在5位模式的命令*/
#define FD628_6DIG_CMD         				0x02	 	/*设置FD628工作在6位模式的命令*/
#define FD628_7DIG_CMD         				0x03	 	/*设置FD628工作在7位模式的命令*/
#define FD628_DIGADDR_WRCMD  						0xC0								//显示地址写入命令
#define FD628_ADDR_INC_DIGWR_CMD       	0x40								//地址递增方式显示数据写入
#define FD628_ADDR_STATIC_DIGWR_CMD    	0x44								//地址不递增方式显示数据写入
#define FD628_DISP_STATUE_WRCMD        	0x80								//显示亮度写入命令
/* **************************************************************************************************************************** */



#endif
