// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "Object/Unit/Character/DefaultUnit.h"
#include "Object/Squad/Squad.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"

//������ �ϴ� ����� ���Ͽ�
//���� ����� ������ �ٰ��� �����Ѵ�.
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

  //���� ����� ���� ���� �� �ױ�ó�� �̵����Ŀ� ����
  //�̷��� �ϸ�ȵɰ�  ����.

  //�׳� ���������� �̵��������� �̵��ϰ� ĸ�� �ϳ��� �������� ������ ��������.

  return EBTNodeResult::Succeeded;

}