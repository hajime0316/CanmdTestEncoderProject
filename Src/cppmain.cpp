#include "cppmain.hpp"
#include "stm32f3_encoder.hpp"
#include "stm32f3_velocity.hpp"

#include "main.h"
#include "can.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

// �G���R�[�_���W���[��
Stm32f3Encoder *encoder_0;
Stm32f3Encoder *encoder_1;
int encoder_pulse_count[2];
int encoder_overflow_count[2];
int htim6_counter = 0;
int htim7_counter = 0;

// ���x���W���[��
Stm32f3Velocity *velocity_0;
Stm32f3Velocity *velocity_1;
int velocity_value[2] = {0};

void setup(void) {
    // �\�t�g�E�F�A���W���[��������
    
    // �n�[�h�E�F�A���W���[���X�^�[�g
    //// �G���R�[�_���W���[��������
    encoder_0 = new Stm32f3Encoder(&htim2);
    encoder_1 = new Stm32f3Encoder(&htim3);
    //// ���x���W���[��������
    velocity_0 = new Stm32f3Velocity(&htim2);
    velocity_1 = new Stm32f3Velocity(&htim3);
    //// �^�C�~���O���\�[�X
    HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_Base_Start_IT(&htim7);
}

void loop(void) {
    //HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOA, LED_B_Pin);

    if(encoder_0 != nullptr) {
        encoder_pulse_count[0] = encoder_0->get_pulse_cnt();
        encoder_overflow_count[0] = encoder_0->get_overflow_cnt();

        if(encoder_overflow_count[0] == 10) {
            delete encoder_0;
            encoder_0 = nullptr;
        }
    }

    if(encoder_1 != nullptr) {
        encoder_pulse_count[1] = encoder_1->get_pulse_cnt();
        encoder_overflow_count[1] = encoder_1->get_overflow_cnt();

        if(encoder_overflow_count[1] == 10) {
            delete encoder_1;
            encoder_1 = nullptr;
        }
    }
}

//**************************
//    �^�C�}���荞�݊֐�
//**************************
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    Stm32f3Encoder::interrupt_handler(htim);

    // 5msec�^�C�}
    if(htim->Instance == htim6.Instance) {
        htim6_counter++;
        velocity_value[0] = velocity_0->periodic_calculate_velocity();
        velocity_value[1] = velocity_1->periodic_calculate_velocity();
    }

	// 100msec�^�C�}
	if(htim->Instance == htim7.Instance) {
        htim7_counter++;
    }
}