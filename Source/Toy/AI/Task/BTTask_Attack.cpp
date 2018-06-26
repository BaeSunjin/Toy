// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "Object/Unit/Character/DefaultUnit.h"
#include "Object/Squad/Squad.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"

//공격을 하는 방법에 관하여
//가장 가까운 적에가 다가가 공격한다.
UBTTask_Attack::UBTTask_Attack(const FObjectInitializer& _objectInitializer)
  : Super(_objectInitializer) {

  NodeName = "Attack";

}


EBTNodeResult::Type UBTTask_Attack::ExecuteTask(
  UBehaviorTreeComponent& _owner_component,
  uint8* _node_memoery) {

  auto target_ptr =  _owner_component.GetBlackboardComponent()->
    GetValueAsObject("AttackTarget");

  if (target_ptr == nullptr) { return EBTNodeResult::Failed; }
  ASquad* squad = Cast<ASquad>(target_ptr);

  //가장 가까운 적을 찻고 그 그근처로 이동한후에 공격
  //이렇게 하면안될거  같다.

  //그냥 적군스쿼드 이동방향으로 이동하고 캡슐 하나를 더씌워서 적군을 공격하자.

  return EBTNodeResult::Succeeded;

}