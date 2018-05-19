// Fill out your copyright notice in the Description page of Project Settings.

#include "Squad.h"
#include "Common/PS_Utils.h"
#include <limits>
#include "Object/Unit/Character/DefaultUnit.h"
#include "Runtime/Engine/Classes/Engine/World.h"

#define REARRAGNE_DEGREE 0.7071f

// Sets default values
ASquad::ASquad()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  root_component_ = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
  root_component_->SetRelativeLocation(FVector(0, 0, 0));
  SetRootComponent(root_component_);

  squad_forward_ = FVector2D(1, 0);
  squad_right_ = FVector2D(0, 1);

}

void ASquad::Init(const FVector& _pos, const FVector2D& _forward)
{
  SetActorLocation(_pos);

  squad_pos_ = SJ_VectorUtills::ToVector2D(_pos);
  squad_forward_ = _forward;
  squad_right_ = SJ_RotateUtills::ToRight(_forward);

  InitUnitTransform();


}

void ASquad::InitUnitTransform() {


  SJ_ASSERT(units_.Num());

  if (units_[0].IsValid()) {
    unit_vertical_interval_ = units_[0].Get()->GetVerticalInterval();
    unit_hoizental_interval_ = units_[0].Get()->GetHirzontalInterval();
  }

  //TODO ������ ��� �Ұ����ҋ� ������ ������ ���� ���Ѵ�. ó�� ����ʿ�
  SJ_ASSERT(unit_vertical_interval_);
  SJ_ASSERT(unit_hoizental_interval_);

  for (int soldier_num = 0; soldier_num < units_.Num(); soldier_num++) {

    FVector init_pos = GetUnitGoalPos(GetActorLocation(), unit_vertical_interval_,
                                      unit_vertical_interval_, soldier_num);

    SJ_ASSERT(units_[soldier_num].IsValid());

    // position init
    auto unit_ref = units_[soldier_num].Get();
    unit_ref->SetActorLocation(init_pos);

    // rotation init
    FVector forward = SJ_VectorUtills::ToVector3D(squad_forward_);
    unit_ref->SetActorRotation(forward.Rotation());
      
  }

}

void ASquad::MoveSquad(const FVector& _pos)
{

  SJ_ASSERT(units_.Num());

  FVector2D move_pos_2d = SJ_VectorUtills::ToVector2D(_pos);
  FVector2D move_dir = move_pos_2d - squad_pos_;
  SJ_ASSERT(SJ_VectorUtills::Normalize(move_dir));

  //������ squad ����� ���Ͽ� ��迭 ���� �������´�.
  bool do_rearange = CheckNeedRearrange(move_dir);

  //������ ������ vector�� ���
  //������ ���ġ ���õǾ� ���Ǿ�����.
  squad_forward_ = move_pos_2d - squad_pos_;
  SJ_ASSERT(SJ_VectorUtills::Normalize(squad_forward_));

  squad_right_ = SJ_RotateUtills::ToRight(squad_forward_);
  SJ_ASSERT(SJ_VectorUtills::Normalize(squad_right_));

  //TODO 2D ���ͷ� �����ؾ��ϳ� ����ʿ�.
  TArray<FVector> goals;
  SJ_ASSERT(MakeGoals(_pos, goals));

  if (do_rearange) {
    RearrangeSquad(goals);
  }

  UnitsMove(goals);
  SetActorLocation(_pos);

}

void ASquad::SetHighLight(bool _light_on)
{
  SJ_ASSERT(units_.Num())

    for (auto& unit : units_) {
      if (unit.IsValid()) {
        unit->SetHighLight(_light_on);
      }
    }
}

//���� �ϳ��ϳ����� �̵��� ��ġ�� �˷��ش�.
void ASquad::UnitsMove(const TArray<FVector>& _goals)
{
 
  SJ_ASSERT(_goals.Num());
  SJ_ASSERT(units_.Num());
  SJ_ASSERT(_goals.Num() == units_.Num());

  FVector forward = SJ_VectorUtills::ToVector3D(squad_forward_);
  SJ_ASSERT(forward.Normalize());

  int num = _goals.Num();
  for (int i = 0; i < num; i++) {

    SJ_ASSERT(units_[i].IsValid());
    auto unit = units_[i].Get();
    unit->MoveTo(_goals[i], forward);
  }
}


FVector ASquad::GetUnitGoalPos(const FVector& _move_pos,
                               const float& _vertical_interval,
                               const float& _horziontal_interval_,
                               const int& _soldier_num)
{

  SJ_ASSERT(array_vertical_);
  float harf_vertical = array_vertical_ / 2.0f;

  FVector goal_pos = _move_pos;

  //�� ������ ���� ������ ��� �� �̵� ���
  SJ_ASSERT(units_.Num());
  auto unit = units_[_soldier_num];

  SJ_ASSERT(unit.IsValid());

  auto unit_ref = unit.Get();
  unit_ref->soldier_num_ = _soldier_num;

  //������ ���������� ��ġ �ε��� ���
  float vertical_pos_idx = (_soldier_num % array_vertical_) - harf_vertical;
  float horizontal_pos_idx = (_soldier_num / array_vertical_);

  // vertial position correction from sqaud
  FVector added_right = SJ_VectorUtills::ToVector3D(squad_right_);
  added_right *= vertical_pos_idx * _vertical_interval;
  goal_pos += added_right;

  //horizontal position correction from squad
  FVector added_forward = SJ_VectorUtills::ToVector3D(squad_forward_);
  added_forward *= -horizontal_pos_idx * _horziontal_interval_;
  goal_pos += added_forward;

  return goal_pos;

}

bool ASquad::MakeGoals(const FVector& _move_pos, TArray<FVector>& _out_goals)
{

  int unit_num = units_.Num();
  SJ_ASSERT(unit_num);

  for (int soldier_num = 0; soldier_num < unit_num; soldier_num++) {
    
    FVector GoalPos = GetUnitGoalPos(_move_pos, unit_vertical_interval_,
                                     unit_hoizental_interval_, soldier_num);
    
    _out_goals.Add(GoalPos);
  }

  if (_out_goals.Num() != unit_num) { return false; }
  return true;
  
}

bool ASquad::CheckNeedRearrange(const FVector2D& _move_dir) {

  SJ_ASSERT(SJ_VectorUtills::IsNormalized(_move_dir));

  float dot_scala = FVector2D::DotProduct(squad_forward_, _move_dir);

  //REARRAGNE_DEGREE = 45 dgree
  return dot_scala < REARRAGNE_DEGREE;

}

void ASquad::RearrangeSquad(const TArray<FVector>& goals)
{
  int goal_num = goals.Num();
  int unit_num = units_.Num();
  SJ_ASSERT(goal_num);
  SJ_ASSERT(unit_num);
  SJ_ASSERT(goal_num == unit_num);

  // �ǵ��ϴ� ������ goals�� �� ����� idx��
  // units_������ ���� ª�� �Ÿ��� ���� ������ idx��
  // ���� ����� �ش�.


  int array_num = goal_num;
  for (int goal_idx = 0 ; goal_idx < (goal_num - 1) ; goal_idx++) {

    int near_unit_idx = goal_idx;
    float length = std::numeric_limits<float>::max();

    for (int unit_idx = goal_idx; unit_idx < array_num; unit_idx++) {

      SJ_ASSERT(units_[unit_idx].IsValid());

      auto unit = units_[unit_idx].Get();
      FVector unit_pos = unit->GetActorLocation();
      float new_length = (goals[goal_idx] - unit_pos).SizeSquared();
      
      if (new_length < length) {
        length = new_length;
        near_unit_idx = unit_idx;
      }
    }


    // ������ ��ġ ���� �ʿ� x
    if (near_unit_idx == goal_idx) { continue; }

    // ������ ��ġ�� �����Ѵ�.    
    units_.Swap(goal_idx, near_unit_idx);

  }



  //���ֿ� ���ںο�
  //���Ŀ� ��밡�ɼ� ����..
  int count = 0;
  for (auto unit : units_) {
    SJ_ASSERT(unit.IsValid());
    unit.Get()->soldier_num_ = count;
    count++;
  }

}

// Called when the game starts or when spawned
void ASquad::BeginPlay()
{
  Super::BeginPlay();

}

// Called every frame
void ASquad::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

}


