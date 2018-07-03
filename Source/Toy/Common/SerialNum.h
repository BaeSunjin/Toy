// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Math/NumericLimits.h"

/* 
 * �� ��ü�� �ø��� ��ȣ�� ǥ���Ѵ�.
 * �ø��� ��ȣ�� ������ ����Ǵ� ���� ��ĥ���� ���� ���̴�.
 * ǥ�� ����
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