#include "u_sys.h" 
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
  MSR MSP, r0 			//set Main Stack value
  BX r14
}
u8 Bflg1msDelaytime;       //1ms
u8 Guc10msDelaytime;
u8 Guc100msDelaytime;
u8 Guc1000msDelaytime;
u8 Guc1minDelaytime;
