// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsRunAway.generated.h"

/**
 * 
 */
UCLASS()
class TOY_API UBTDecorator_IsRunAway : public UBTDecorator_BlackboardBase
{
  GENERATED_BODY()


  UBTDecorator_IsRunAway(const FObjectInitializer& ObjectInitializer);
  virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                          uint8* NodeMemory) const override;
	
	
};
