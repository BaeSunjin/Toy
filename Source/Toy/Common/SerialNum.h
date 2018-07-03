// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Math/NumericLimits.h"

/* 
 * 각 객체의 시리얼 번호를 표현한다.
 * 시리얼 번호는 게임이 진행되는 동안 겹칠일이 없을 것이다.
 * 표현 범위
 * 1 ~ int::max
*/

template<typename T>
class TOY_API SerialNum
{
public:
  SerialNum() { 
    generated_num_++;
    if (generated_num_ == TNumericLimits< int32 >::Max()) {
      generated_num_ = 0;
    }
    serial_num_ = generated_num_;
  }
  ~SerialNum() {}

  int GetSerialNum() {
    return serial_num_;
  }

private:

  int serial_num_;
  static int generated_num_;
};

template<typename T>
int SerialNum<T>::generated_num_ = 0;