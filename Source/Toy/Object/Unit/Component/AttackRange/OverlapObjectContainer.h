// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/PS_Utils.h"
#include "Runtime/Core/Public/Math/NumericLimits.h"
#include "Runtime/Core/Public/Templates/Tuple.h"
#include "Runtime/Core/Public/UObject/WeakObjectPtrTemplates.h"
#include "Rule/Rules.h"


//데이터 전달용 구조체
template<typename T>
struct FCloseTarget {

public:

  float distance;
  TWeakObjectPtr<T> object_ptr;
};

template<typename T>
class TOY_API OverlapObjectContainer
{

 public:

   explicit OverlapObjectContainer();
   ~OverlapObjectContainer();

  
   bool GetCloseTarget(const FVector& _locatoin, FCloseTarget<T>& _out);

   void Add(TWeakObjectPtr<T> _actor);
   void Remove(TWeakObjectPtr<T> _actor);
  
private:

  OverlapObjectContainer(const OverlapObjectContainer &t) = delete;
  OverlapObjectContainer& operator=(const OverlapObjectContainer &t) = delete;


  bool HasAlreadyAdded(T* _actor);

  UPROPERTY(VisibleAnywhere, Category = AttackRange)
  TArray<TWeakObjectPtr<T>> targets_;


};




template <typename T>
OverlapObjectContainer<T>::OverlapObjectContainer() {}

template <typename T>
OverlapObjectContainer<T>::~OverlapObjectContainer() {}


template <typename T>
bool OverlapObjectContainer<T>::GetCloseTarget(const FVector& _locatoin,
                                            FCloseTarget<T>& _out) {

  float short_distance = TNumericLimits< float >::Max();
  ADefaultUnit* attack_target = nullptr;
  //std::lock_guard<std::mutex> guard(mutex_);  
  for (auto target : targets_) {
    if (target.IsValid()) {
      float distance = FVector::DistSquared(
        _locatoin, target.Get()->GetActorLocation());

      if (distance < short_distance) {
        attack_target = target.Get();
        short_distance = distance;
      }
    }
  }

  if (!attack_target) { return false; }

  _out.distance = short_distance;
  _out.object_ptr = attack_target;

  return true;

}

template <typename T>
void OverlapObjectContainer<T>::Add(TWeakObjectPtr<T> _actor) {

  if (_actor.IsValid()) {
    if (!HasAlreadyAdded(_actor.Get())) {
      targets_.Add(_actor);
    }
  }

}

template <typename T>
void OverlapObjectContainer<T>::Remove(TWeakObjectPtr<T> _actor) {

  bool find = false;
  int count = targets_.Num();
  int array_idx;
  for (array_idx = 0; array_idx < count; ++array_idx) {
    if (targets_[array_idx].IsValid()) {
      auto target = targets_[array_idx].Get();
      if (target->GetSerialNum() == _actor->GetSerialNum()) {
        find = true;
        break;
      }
    }
  }

  if (find) {
    targets_.RemoveAt(array_idx);
  }

}

template <typename T>
bool OverlapObjectContainer<T>::HasAlreadyAdded(T* _actor) {

  SJ_ASSERT(_actor);

  //아무것도 존재 하지 않을떄 : 찻기 실패 
  if (!targets_.Num()) { return false; }

  for (auto& target : targets_) {
    if (target.IsValid()) {
      if (target.Get()->GetSerialNum() == _actor->GetSerialNum()) {
        UE_LOG(LogTemp, Error,
          TEXT("%s Overlap again"), *_actor->GetName());
        return true;
      }
    }
  }
  return false;

}