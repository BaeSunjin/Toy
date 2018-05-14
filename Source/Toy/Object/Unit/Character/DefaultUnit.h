// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Object/Tool/SquadMaker.h"
#include "DefaultUnit.generated.h"

class ASquad;
class UCapsuleComponent;


UCLASS()
class TOY_API ADefaultUnit : public ACharacter
{
  GENERATED_BODY()

public:
  // Sets default values for this character's properties
  ADefaultUnit();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  

protected:

  UPROPERTY(EditAnywhere, Category = HighLight)
  UStaticMeshComponent* high_light_component_;


  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  USkeletalMeshComponent* unit_mesh_;

  // 임시 디버깅 심볼
  mutable FVector log_pos_;

public:

  TWeakObjectPtr<ASquad> squad_;

protected:

  UPROPERTY(VisibleDefaultsOnly, Category = Test)
  UClass* test_billbard_generateed_class_;

  UPROPERTY(EditAnywhere, Category = Test)
  class AActor* test_billboard_;

  // soldier 
  float vertical_interval_;
  float horizontal_interval_;

public:

  //squad soldier idx number
  UPROPERTY(BlueprintReadOnly, Category = Test)
  int soldier_num_;

private:

  //SquadMaker에서만 사용을 한다.
  friend class SquadMaker;
  void SetSquad(const TWeakObjectPtr<ASquad>& data);

public:

  const TWeakObjectPtr<ASquad>& GetSquad();


  void SetHighLight(bool _light_on);
  float GetHirzontalInterval();
  float GetVerticalInterval();

  virtual void UnitMeshInit();
  virtual void UnitIntervalInit();

  virtual void MoveTo(const FVector& _move_pos,
                      const FVector& _normal_look_at) const;

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
