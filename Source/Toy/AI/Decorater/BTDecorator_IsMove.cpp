// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsMove.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsMove::UBTDecorator_IsMove(
  const FObjectInitializer& _object_initializer)
  : Super(_object_initializer) {

  NodeName = "IsMove";
  bNotifyTick = false;

}

void UBTDecorator_IsMove::OnBecomeRelevant(
  UBehaviorTreeComponent& _owner_component,
  uint8* _node_memory) {

  FBTIsMoveDecoraterMemory* memory = (FBTIsMoveDecoraterMemory*)_node_memory;
  memory->move_to_ = _owner_component.GetBlackboardComponent()->
    GetValueAsVector("MoveTo");

}

//지금 설계가 잘못되어있다.
bool UBTDecorator_IsMove::CalculateRawConditionValue(
  UBehaviorTreeComponent& _owner_component,
  uint8* _node_memory) const {

  FVector new_move_to = _owner_component.GetBlackboardComponent()->
    GetValueAsVector("MoveTo");

  FBTIsMoveDecoraterMemory* memory = (FBTIsMoveDecoraterMemory*)_node_memory;
  if (memory->move_to_ == new_move_to) {

  }


  return true;

}

uint16 UBTDecorator_IsMove::GetInstanceMemorySize() const
{
  return sizeof(FBTIsMoveDecoraterMemory);
}
