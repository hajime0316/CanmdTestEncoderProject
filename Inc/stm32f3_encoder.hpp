/*
 * Module : stm32f3_Stm32f3Encoder
 * File   : stm32f3_Stm32f3Encoder.hpp
 * Version: 01.00
 * Author : hajime
 * 
 * Created on 2019/03/23, 17:32
 */

#ifndef STM32F3_ENDODER_HPP_
#define STM32F3_ENDODER_HPP_

#include "stm32f3xx_hal.h"

class Stm32f3Encoder {
private:
    TIM_HandleTypeDef *htim;
    int overflow_cnt;

public:
    Stm32f3Encoder(TIM_HandleTypeDef *htim);
    ~Stm32f3Encoder();

    int get_pulse_cnt(void);
    int get_overflow_cnt(void);
    
    // �G���R�[�_���W���[���̊��荞�݃n���h��
    // TIM���荞�݊֐��̒��ŃR�[��
    static void interrupt_handler(TIM_HandleTypeDef *htim_generating_interrupt);

private:
    // �S�C���X�^���X�ɃA�N�Z�X���邽�߂̃|�C���^�D
    // ���荞�݃��[�`���̒��Ŏg���D
    Stm32f3Encoder        *previous_instance_p; // ���g�̑O�ɐ������ꂽ�C���X�^���X�ւ̃|�C���^
                                                // �O�̃C���X�^���X���Ȃ��ꍇ��null
    static Stm32f3Encoder *last_instance_p;     // �Ō�ɐ��������C���X�^���X�ւ̃|�C���^

    // interrupt_handler�֐�����Ăяo�����֐�
    void interrupt_routine(TIM_HandleTypeDef *htim);
};

#endif  /* VELOCITY_HPP */