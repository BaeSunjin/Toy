// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "DefaultWeaponComponent.generated.h"


class ADefaultUnit;
/**
 *
 */
UCLASS()
class TOY_API UDefaultWeaponComponent : public USkeletalMeshComponent
{
  GENERATED_BODY()

public:

  UDefaultWeaponComponent(const FObjectInitializer & _initializer);
  void Attach(USkeletalMeshComponent* _mesh, FName _socket_name);

  void AttackTarget(TWeakObjectPtr<ADefaultUnit> _target);

protected:
  virtual void InitMesh();


private:

  float demage_;

};
