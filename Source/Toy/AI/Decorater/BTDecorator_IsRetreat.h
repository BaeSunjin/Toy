// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsRetreat.generated.h"

/**
 * 
 */
UCLASS()
class TOY_API UBTDecorator_IsRetreat : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
  UBTDecorator_IsRetreat(const FObjectInitializer& ObjectInitializer);
  virtual bool CalculateRawConditionValue(UBehaviorTreeComponent&,
    uint8*) const override;
	
};
