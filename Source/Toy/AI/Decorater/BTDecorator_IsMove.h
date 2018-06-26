// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsMove.generated.h"



struct FBTIsMoveDecoraterMemory
{
  FVector move_to_;
};

/**
 * 
 */
UCLASS()
class TOY_API UBTDecorator_IsMove : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
  UBTDecorator_IsMove(const FObjectInitializer& _object_initializer);
  virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& ,
                                          uint8* ) const override;
	

  virtual void OnBecomeRelevant(UBehaviorTreeComponent& _owner_component,
    uint8* _node_memory) override;

  virtual uint16 GetInstanceMemorySize() const override;
	
};
