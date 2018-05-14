// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Squad.generated.h"

class ADefaultUnit;

//struct�� ���� �� squad manager�� �����Ǿ���� ������ �����صд�.
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

private:

  FVector2D squad_forward_;
  FVector2D squad_right_;

  FVector2D squad_pos_;

public:

  void Init(const FVector& _pos,const FVector2D& _forward);
  void InitUnitPos();

  // 2D�� ���ڸ� �޴� ������ �������� ������ó���� xy ��ǥ�θ� �����ϱ� ����
  void MoveSquad(const FVector& _pos);
  void SetHighLight(bool _light_on);

  FVector2D GetSquadCenter();

private:

  void UnitsMove(const FVector& _pos);
  FVector GetUnitGoalPos(const FVector& _move_pos,
                         const float& _vertical_interval, 
                         const float& _horziontal_interval_,
                         const int& _soldier_num);

  bool CheckNeedRearrange(const FVector2D& _move_dir);

  //�뿭 �ʱ�ȭ
  void RearrangeSquad(const FVector2D& _pos);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
