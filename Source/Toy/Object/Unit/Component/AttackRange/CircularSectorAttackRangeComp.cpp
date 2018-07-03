// Fill out your copyright notice in the Description page of Project Settings.

#include "CircularSectorAttackRangeComp.h"
#include "Common/PS_Utils.h"
#include "Runtime/Core/Public/Math/NumericLimits.h"

// Sets default values for this component's properties
UCircularSectorAttackRangeComp::UCircularSectorAttackRangeComp()
{

  PrimaryComponentTick.bCanEverTick = false;

  OnComponentBeginOverlap.AddDynamic(this,
    &UCircularSectorAttackRangeComp::BeginOverlapCallBack);
  OnComponentEndOverlap.AddDynamic(this,
    &UCircularSectorAttackRangeComp::EndOverlapCallBack);

  bGenerateOverlapEvents = true;
  SetVisibility(true);

  SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  team_flag_ = TeamFlag::kNone;

}

void UCircularSectorAttackRangeComp::Attach(USceneComponent* _parent) {

  SJ_ASSERT(_parent);
  parent_ = _parent;
  AttachToComponent(_parent, FAttachmentTransformRules::KeepRelativeTransform);
  SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

}


ADefaultUnit* UCircularSectorAttackRangeComp::GetAttackTarget() {

  SJ_ASSERT((team_flag_ != TeamFlag::kNone));

  //Attack Range로 한번 거르고
  //가장 가까운 적을 찻아서 공격한다.
  FVector owner_locatoin = parent_->GetComponentLocation();

  float short_distance = TNumericLimits< float >::Max();

  ADefaultUnit* attack_target = nullptr;
  //std::lock_guard<std::mutex> guard(mutex_);  
  for (auto unit : targets_) {
    if (unit.IsValid()) {
      float distance = FVector::DistSquared(
        owner_locatoin, unit.Get()->GetActorLocation());

      if (distance < short_distance) {
        if (distance < attack_range_squared_) {
          attack_target = unit.Get();
        }
      }
    }
  }

  return attack_target;

}


void UCircularSectorAttackRangeComp::SetTeamFlag(const TeamFlag& _team_flag) {
  team_flag_ = _team_flag;
}

void UCircularSectorAttackRangeComp::Work() {

  SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
    ECollisionResponse::ECR_Overlap);

}


void UCircularSectorAttackRangeComp::BeginOverlapCallBack(
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

  auto other_actor = Cast<ADefaultUnit>(_other_actor);
  if (other_actor == nullptr) { return; }

  if (other_actor->GetTeamFlag() == TeamFlag::kNone) {
    UE_LOG(LogTemp, Error,
      TEXT("BoxAttackRange overlaped actor Not inited"));

    return;
  }

  //같은 팀 확인
  if (other_actor->GetTeamFlag() == team_flag_) { return; }

  // std::lock_guard<std::mutex> guard(mutex_);
  if (!HasAlreadyAdded(other_actor)) {
    targets_.Add(other_actor);
  }

}


void UCircularSectorAttackRangeComp::EndOverlapCallBack(
  UPrimitiveComponent* _overlap_component,
  AActor* _other_actor,
  UPrimitiveComponent* _other_component,
  int32 _other_body_idx) {

  auto other_actor = Cast<ADefaultUnit>(_other_actor);
  if (other_actor == nullptr) { return; }

  //같은 팀 확인
  if (other_actor->GetTeamFlag() == team_flag_) { return; }

  bool find = false;
  int count = targets_.Num();
  int array_idx;
  for (array_idx = 0; array_idx < count; ++array_idx) {
    if (targets_[array_idx].IsValid()) {
      auto target = targets_[array_idx].Get();
      if (target->GetSerialNum() == other_actor->GetSerialNum()) {
        find = true;
        break;
      }
    }
  }

  if (find) {
    targets_.RemoveAt(array_idx);
  }
}

void UCircularSectorAttackRangeComp::SetAttackRange(const float& _attack_range) {

  attack_range_ = _attack_range;
  attack_range_squared_ = attack_range_;

  SetBoxExtent(FVector(attack_range_, attack_range_, 100.0f));
  SetRelativeLocation(FVector(attack_range_, 0.0f, 0.0f));

}

bool UCircularSectorAttackRangeComp::HasAlreadyAdded(ADefaultUnit* _actor) {

  SJ_ASSERT(_actor);

  //아무것도 존재 하지 않을떄 : 찻기 실패 
  if (!targets_.Num()) { return false; }

  for (auto& target : targets_) {
    if (target.IsValid()) {
      if (target.Get()->GetSerialNum() == _actor->GetSerialNum()) {
        UE_LOG(LogTemp, Error,
          TEXT("%s Overlap again"), *_actor->GetName());
        return true;
      }
    }
  }
  return false;

}



