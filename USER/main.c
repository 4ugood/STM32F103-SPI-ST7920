#include "u_sys.h"
#include "u_delay.h"
#include "u_timer.h"
#define x1    0x80
#define x2    0x88
#define y     0x80
#define comm  0
#define dat   1
u8 const num_lcd[]={"0123456789 :.-C%��"};
/**
  * @brief  ������ʼ��
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
      LCD_SID = 1;        //ȡ�����λ
    }
    else
    {
      LCD_SID = 0;
    }
    LCD_CLK = 1;
    t = 0x10;
    while(t--);     //��ʱ lcd��ȡ����
    LCD_CLK = 0;
    bbyte <<= 1;    //����
  } 
}
void Write_Char(u8 start, u8 ddata)
{
  u8 start_data,Hdata,Ldata;
  if(start==0)
  {
    start_data=0xf8;  //дָ��
  }
  else
  {
    start_data=0xfa;  //д����
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
//������ʾ����
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
  Write_Char(0,0x30);  //8 λ���棬����ָ�
  Write_Char(0,0x0C);  //��ʾ�򿪣����أ����׹�
  Write_Char(0,0x01);  //��������DDRAM�ĵ�ַ����������
}
/**
 * ����: ʱ��ѭ������
 * λ��: ��ѭ��
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
    LCD_Write_String(1, 0, "ʱ�����");
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
  delay_init();	    	                            //��ʱ������ʼ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  LCDInit();
  TIM3_Int_Init(9, 7199);                         //10Khz�ļ���Ƶ�ʣ�������10Ϊ1ms
  initParameter();
  while(1)
  {
    runTimeTask();
  }
}
