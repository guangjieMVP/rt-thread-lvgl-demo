#include "bsp_general_tim.h"

/**
  * @brief  ͨ�ö�ʱ�� TIMx,x[2,3,4,5]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    // �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQn;

    // ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

    // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 * TIM_Period / Auto Reload Register(ARR) = 1000-1   TIM_Prescaler=((SystemCoreClock/2)/1000000)-1 
 *  *	
 * ��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
 *				PCLK1 = HCLK / 4 
 *				=> TIMxCLK = HCLK / 2 = SystemCoreClock /2
 * ��ʱ��Ƶ��Ϊ = TIMxCLK/(TIM_Prescaler+1) = (SystemCoreClock /2)/((SystemCoreClock/2)/1000000) = 1000000 = 1MHz
 * �ж�����Ϊ = 1/(1MHz) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8,15,16,17]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    // ����TIMx_CLK,x[2,3,4,5]
    RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE);

    /* �ۼ� TIM_Period�������һ�����»����ж�*/
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1; //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����

    //��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1
    //				PCLK1 = HCLK / 4
    //				=> TIMxCLK = HCLK / 2 = SystemCoreClock /2
    // ��ʱ��Ƶ��Ϊ = TIMxCLK/(TIM_Prescaler+1) = (SystemCoreClock /2)/((SystemCoreClock/2)/1000000) = 1000000 = 1MHz
    TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock / 2) / 1000000) - 1;

    //  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ

    // ��ʼ����ʱ��TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

    // ����������жϱ�־λ
    TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);

    // �����������ж�
    TIM_ITConfig(GENERAL_TIM, TIM_IT_Update, ENABLE);

    // ʹ�ܼ�����
    TIM_Cmd(GENERAL_TIM, ENABLE);
}

/**
  * @brief  ��ʼ��ͨ�ö�ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{

    TIM_Mode_Config();

    TIMx_NVIC_Configuration();
}

