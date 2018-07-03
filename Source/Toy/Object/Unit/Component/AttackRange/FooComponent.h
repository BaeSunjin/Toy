// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Object/Unit/Component/AttackRange/AttackRangeInterface.h"
#include "Object/Unit/Component/AttackRange/OverlapObjectContainer.h"
#include "FooComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOY_API UFooComponent : public UBoxComponent,
                              public IAttackRangeInterface
{
  GENERATED_BODY()


    UFooComponent();

  virtual void Attach(USceneComponent* _parent) override;
  virtual void Work() override;

  virtual void SetTeamFlag(const TeamFlag& _team_flag) override;
  virtual void SetAttackRange(const float& _attack_range) override;

  virtual ADefaultUnit* GetAttackTarget() override;


  //이부분은 확실하게 재정의 되어야 한다.
  UFUNCTION()
    void BeginOverlapCallBack(UPrimitiveComponent* _overlap_component,
      AActor* _other_actor,
      UPrimitiveComponent* _other_component,
      int32 _other_body_idx,
      bool _from_sweep,
      const FHitResult &_sweep_result);

  UFUNCTION()
    void EndOverlapCallBack(UPrimitiveComponent* _overlap_component,
      AActor* _other_actor,
      UPrimitiveComponent* _other_component,
      int32 _other_body_idx);

private:

  //===========================================================

  OverlapObjectContainer<ADefaultUnit> overlap_object_container_;

  bool HasAlreadyAdded(ADefaultUnit* _actor);

  UPROPERTY(VisibleAnywhere, Category = AttackRange)
    TArray<TWeakObjectPtr<ADefaultUnit>> targets_;

  USceneComponent* parent_;
  TeamFlag team_flag_;
  float attack_range_;
  float attack_range_squared_;
	
	
};
