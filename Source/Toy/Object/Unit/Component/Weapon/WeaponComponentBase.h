// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Object/Unit/Character/UnitBase.h"
#include "WeaponComponentBase.generated.h"



/**
 * 
 */
UCLASS()
class TOY_API UWeaponComponentBase : public USkeletalMeshComponent
{
  GENERATED_BODY()

 public:

   UWeaponComponentBase(const FObjectInitializer & _initializer);

   void Init();

   void AttachTo(USkeletalMeshComponent* _mesh, FName _socket_name);
   virtual void Attack(TWeakObjectPtr<AUnitBase> _target);

   float GetDemage();
   void SetDemage(float _demage);

 private:

   void InitMesh();
   float demage_;

public:

  FString static_mesh_file_path_;
	
};
