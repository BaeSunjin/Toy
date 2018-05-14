// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Squad.generated.h"

class ADefaultUnit;

//struct로 현재 이 squad manager에 생성되어야할 정보를 저장해둔다.
USTRUCT(BlueprintType)
struct FSquadInfo {

  GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int32 org_unit_count_;

  UPROPERTY(VisibleAnywhere)
	int32 max_unit_count_;

  UPROPERTY(VisibleAnywhere)
	int32 min_unit_count_;

};


UCLASS()
class TOY_API ASquad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASquad();

public:

  TArray<TWeakObjectPtr<ADefaultUnit>> units_;
  FSquadInfo squad_info_;
  
  int currnet_unit_count_;

  int array_vertical_;
  int array_horizental_;

  float unit_vertical_interval_;
  float unit_hoizental_interval_;

private:

  FVector2D squad_forward_;
  FVector2D squad_right_;

  FVector2D squad_pos_;

  UPROPERTY(VisibleDefaultsOnly)
  USceneComponent* root_component_;

public:

  void Init(const FVector& _pos,const FVector2D& _forward);
  void InitUnitTransform();

  // 2D로 인자를 받는 이유는 스쿼드의 움직임처리는 xy 좌표로만 동작하기 때문
  void MoveSquad(const FVector& _pos);
  void SetHighLight(bool _light_on);

private:

  void UnitsMove(const TArray<FVector>& _goals);
  FVector GetUnitGoalPos(const FVector& _move_pos,
                         const float& _vertical_interval, 
                         const float& _horziontal_interval_,
                         const int& _soldier_num);

  bool MakeGoals(const FVector& _move_pos, TArray<FVector>& _out_goals);

  bool CheckNeedRearrange(const FVector2D& _move_dir);

  //대열 초기화
  void RearrangeSquad(const TArray<FVector>& goals);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
