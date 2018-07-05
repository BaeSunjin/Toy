// Fill out your copyright notice in the Description page of Project Settings.

#include "RangeUnitAttackRangeComponent.h"
#include "Object/Squad/Squad.h"

// Sets default values for this component's properties
URangeUnitAttackRangeComponent::URangeUnitAttackRangeComponent(
  const FObjectInitializer & _initializer) {

  PrimaryComponentTick.bCanEverTick = false;

  OnComponentBeginOverlap.AddDynamic(this,
    &URangeUnitAttackRangeComponent::BeginOverlapCallBack);
  OnComponentEndOverlap.AddDynamic(this,
    &URangeUnitAttackRangeComponent::EndOverlapCallBack);

  SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  SetCollisionObjectType(ECollisionChannel::ECC_Trigger);
  bGenerateOverlapEvents = true;
  SetVisibility(true);

  SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  team_flag_ = TeamFlag::kNone;

}

void URangeUnitAttackRangeComponent::Attach(USceneComponent* _parent) {

  SJ_ASSERT(_parent);
  parent_ = _parent;
  AttachToComponent(_parent, FAttachmentTransformRules::KeepRelativeTransform);
  SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

}

ADefaultUnit* URangeUnitAttackRangeComponent::GetAttackTarget() {

  SJ_ASSERT((team_flag_ != TeamFlag::kNone));

  //가장 가까운 적을 가지고 온다.
  FVector owner_locatoin = parent_->GetComponentLocation();
  FCloseTarget<ASquad> target;

  bool find = overlap_object_container_.GetCloseTarget(owner_locatoin, target);
  if (!find) { return nullptr; }

  if (target.distance < attack_range_squared_) {
    if (target.object_ptr.IsValid()) {
      return target.object_ptr.Get()->units_[0].Get();
    }
  }

  return nullptr;

}


void URangeUnitAttackRangeComponent::SetTeamFlag(const TeamFlag& _team_flag) {
  team_flag_ = _team_flag;
}

void URangeUnitAttackRangeComponent::Work() {

  SetCollisionResponseToChannel(ECollisionChannel::ECC_Squad,
    ECollisionResponse::ECR_Overlap);

}

void URangeUnitAttackRangeComponent::BeginOverlapCallBack(
  UPrimitiveComponent* _overlap_component,
  AActor* _other_actor,
  UPrimitiveComponent* _other_component,
  int32 _other_body_idx,
  bool _from_sweep,
  const FHitResult &_sweep_result) {

  if (team_flag_ == TeamFlag::kNone) {

    UE_LOG(LogTemp, Error,
      TEXT("BoxAttackRange Not inited but other actor overlap"));

    return;
  }

  auto other_squad = Cast<ASquad>(_other_actor);
  if (other_squad == nullptr) { return; }

  if (other_squad->GetTeamFlag() == TeamFlag::kNone) {
    UE_LOG(LogTemp, Error,
      TEXT("BoxAttackRange overlaped actor Not inited"));

    return;
  }

  //같은 팀 확인
  if (other_squad->GetTeamFlag() == team_flag_) { return; }

  overlap_object_container_.Add(other_squad);

}


void URangeUnitAttackRangeComponent::EndOverlapCallBack(
  UPrimitiveComponent* _overlap_component,
  AActor* _other_actor,
  UPrimitiveComponent* _other_component,
  int32 _other_body_idx) {

  auto other_squad = Cast<ASquad>(_other_actor);
  if (other_squad == nullptr) { return; }

  //같은 팀 확인
  if (other_squad->GetTeamFlag() == team_flag_) { return; }

  overlap_object_container_.Remove(other_squad);


}

void URangeUnitAttackRangeComponent::SetAttackRange(const float& _attack_range) {

  attack_range_ = _attack_range;
  attack_range_squared_ = attack_range_ * attack_range_;

  SetBoxExtent(FVector(attack_range_* 2.0f, attack_range_* 2.0f, 100.0f));
  //SetBoxExtent(FVector(attack_range_* 2.0f, attack_range_* 2.0f, 1000.0f));
  //SetBoxExtent(FVector(attack_range_squared_, attack_range_squared_, 1000.0f));

}
