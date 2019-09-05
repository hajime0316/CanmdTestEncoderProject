/*
 * Module : stm32f3_encoder
 * File   : stm32f3_encoder.cpp
 * Version: 01.00
 * Author : hajime
 * 
 * Created on 2019/03/23, 17:29
 */

#include "stm32f3_encoder.hpp"
#include "main.h"

#ifndef MAX_ENCODER_COUNT
    #error MAX_ENCODER_COUNTが定義されていません．\
           CubeMXの[User Constants]でこのマクロ定\
           数を定義してください．
#endif

Stm32f3Encoder *Stm32f3Encoder::last_instance_p = nullptr;

Stm32f3Encoder::Stm32f3Encoder(TIM_HandleTypeDef *htim) {
    this->htim = htim;
    this->overflow_cnt = 0;

    previous_instance_p = last_instance_p; 
    last_instance_p = this;

    // ハードウェアスタート
    HAL_TIM_Base_Start_IT(htim);    // これがないと割り込みが有効にならない
    HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
}

Stm32f3Encoder::~Stm32f3Encoder(void) {
    if(last_instance_p == nullptr) return;

    if(last_instance_p == this) {
        last_instance_p = this->previous_instance_p;
        return;
    }

    Stm32f3Encoder *i_p = last_instance_p;
    while(true) {
        if(i_p->previous_instance_p == this) {
            i_p->previous_instance_p = this->previous_instance_p;
            return;
        }
        else {
            i_p = i_p->previous_instance_p;
            continue;
        }
    }
}

int Stm32f3Encoder::get_pulse_cnt(void) {
    return htim->Instance->CNT;
}

int Stm32f3Encoder::get_overflow_cnt(void) {
    return overflow_cnt;
}

void Stm32f3Encoder::interrupt_handler(TIM_HandleTypeDef *htim_generating_interrupt) {
    if(last_instance_p == nullptr) return;

    last_instance_p->interrupt_routine(htim_generating_interrupt);

    return;
}

void Stm32f3Encoder::interrupt_routine(TIM_HandleTypeDef *htim_generating_interrupt) {
    if(this->htim->Instance == htim_generating_interrupt->Instance) {
        if(htim_generating_interrupt->Instance->CNT < (MAX_ENCODER_COUNT / 2)){
            // MAX_PULSE_COUNT から 0 の方向にオーバーフロー
            overflow_cnt++;
        }
        else {
            // 0 から MAX_PULSE_COUNT の方向にオーバーフロー
            overflow_cnt--;
        }
    }

    if(previous_instance_p == NULL) return;

    previous_instance_p->interrupt_routine(htim_generating_interrupt);

    return;
}