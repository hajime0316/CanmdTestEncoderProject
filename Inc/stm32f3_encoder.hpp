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
    
    // エンコーダモジュールの割り込みハンドラ
    // TIM割り込み関数の中でコール
    static void interrupt_handler(TIM_HandleTypeDef *htim_generating_interrupt);

private:
    // 全インスタンスにアクセスするためのポインタ．
    // 割り込みルーチンの中で使う．
    Stm32f3Encoder        *previous_instance_p; // 自身の前に生成されたインスタンスへのポインタ
                                                // 前のインスタンスがない場合はnull
    static Stm32f3Encoder *last_instance_p;     // 最後に生成したインスタンスへのポインタ

    // interrupt_handler関数から呼び出される関数
    void interrupt_routine(TIM_HandleTypeDef *htim);
};

#endif  /* VELOCITY_HPP */