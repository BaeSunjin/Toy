// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsRunAway.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsRunAway::UBTDecorator_IsRunAway(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer) {

  NodeName = "IsRunAway";

}


bool UBTDecorator_IsRunAway::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {

  return OwnerComp.GetBlackboardComponent()->GetValueAsBool("RunAway");

}


