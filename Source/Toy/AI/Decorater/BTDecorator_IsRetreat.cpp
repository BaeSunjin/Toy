// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsRetreat.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsRetreat::UBTDecorator_IsRetreat(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer) {

  NodeName = "IsRetreat";

  bNotifyTick = false;
}




bool UBTDecorator_IsRetreat::CalculateRawConditionValue(
  UBehaviorTreeComponent& _owner_componenet,
  uint8* _node_memory) const {

  return _owner_componenet.GetBlackboardComponent()->
    GetValueAsBool("Retreat");

}


