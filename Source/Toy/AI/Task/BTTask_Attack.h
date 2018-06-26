// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Attack.generated.h"


/**
 * 
 */
UCLASS()
class TOY_API UBTTask_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


  UBTTask_Attack(const FObjectInitializer& _objectInitializer);
  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _owner_component,
      uint8* _node_memoery);
	
	
};
