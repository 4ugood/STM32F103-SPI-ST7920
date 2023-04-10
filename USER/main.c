#include "u_sys.h"
#include "u_delay.h"
#include "u_timer.h"
#define x1    0x80
#define x2    0x88
#define y     0x80
#define comm  0
#define dat   1
u8 const num_lcd[]={"0123456789 :.-C%°"};
/**
  * @brief  参数初始化
  */
void initParameter(void)
{
  PSB = 0;
}
void Send_Byte(u8 bbyte)
{
  u8 i,t;
  for(i=0;i<8;i++)
  {
    if((bbyte)&0x80)
    {
      LCD_SID = 1;        //取出最高位
    }
    else
    {
      LCD_SID = 0;
    }
    LCD_CLK = 1;
    t = 0x10;
    while(t--);     //延时 lcd读取数据
    LCD_CLK = 0;
    bbyte <<= 1;    //左移
  } 
}
void Write_Char(u8 start, u8 ddata)
{
  u8 start_data,Hdata,Ldata;
  if(start==0)
  {
    start_data=0xf8;  //写指令
  }
  else
  {
    start_data=0xfa;  //写数据
  }
  Hdata=ddata&0xf0;
  Ldata=(ddata<<4)&0xf0;
  Send_Byte(start_data);
  delay_us(2);
  Send_Byte(Hdata);
  delay_us(1);
  Send_Byte(Ldata);
  delay_us(1);
}
void LCD_Set_XY(u8 X, u8 Y)
{ 
  u8 address;
  switch(X)
  {
    case 0:
      address = 0x80 + Y;
      break;   
    case 1:
      address = 0x80 + Y;
      break;
    case 2:
      address = 0x90 + Y;
      break;
    case 3:
      address = 0x88 + Y;
      break;
    case 4:
      address = 0x98 + Y;
      break;
    default:
      address = 0x80 + Y;
      break;
  }
  Write_Char(0, address);
}
void LCD_Write_String(u8 X, u8 Y, uc8 *s)
{
  LCD_Set_XY(X, Y);
  while (*s)
  {
    Write_Char(1, *s);
    s ++;
    delay_us(1);//200ns
  }
}
//数字显示函数
void LCD_Write_Number(u8 s)
{
  Write_Char(1,num_lcd[s]);
  delay_us(1);//200ns
}
void LCDInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  delay_us(10);
  LCD_CS = 1;
  Write_Char(0,0x30);  //8 位介面，基本指令集
  Write_Char(0,0x0C);  //显示打开，光标关，反白关
  Write_Char(0,0x01);  //清屏，将DDRAM的地址计数器归零
}
/**
 * 描述: 时间循环函数
 * 位置: 主循环
*/
void runTimeTask(void)
{
  //1ms
  if (1 == Bflg1msDelaytime)
  {
    Bflg1msDelaytime = 0;
    Guc10msDelaytime ++;
  }
  //10ms
  if (Guc10msDelaytime >= 10)
  {
    Guc10msDelaytime = 0;
    Guc100msDelaytime ++;
  }
  //100ms
  if (Guc100msDelaytime >= 10)
  {
    Guc100msDelaytime = 0;
    Guc1000msDelaytime ++;

  }
  //1s
  if (Guc1000msDelaytime >= 10)
  {
    Guc1000msDelaytime = 0;
    Guc1minDelaytime ++;
    LCD_Write_String(1, 0, "时间嘀嗒");
    LCD_Set_XY(1, 4);
    Write_Char(1, num_lcd[Guc1minDelaytime/10]);
    Write_Char(1, num_lcd[Guc1minDelaytime%10]);
  }
  //1min
  if (Guc1minDelaytime >= 60)
  {
    Guc1minDelaytime = 0;

  }
}
int main(void)
{
  delay_init();	    	                            //延时函数初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  LCDInit();
  TIM3_Int_Init(9, 7199);                         //10Khz的计数频率，计数到10为1ms
  initParameter();
  while(1)
  {
    runTimeTask();
  }
}
