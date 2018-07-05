/// Fill out your copyright notice in the Description page of Project Settings.

#include "Human_TwoHandedAx.h"
#include "Object/Unit/Component/AttackRange/BoxAttackRangeComponent.h"
#include "Object/Unit/Component/Weapon/TwoHandedAxComponent.h"



AHuman_TwoHandedAx::AHuman_TwoHandedAx(const FObjectInitializer & _initializer)
  : Super(_initializer) {

  // init data setting
  static_mesh_file_path_ = "SkeletalMesh'/Game/Characters/Animations/TwoHandedSword_AnimsetPro/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'";
  animation_blueprint_file_path_ = "AnimBlueprint'/Game/Characters/Animations/TwoHandedSword_AnimsetPro/Animations/AnimBlueprint/2hand_sward_warrior.2hand_sward_warrior'";
  behavior_tree_file_path_ = "BehaviorTree'/Game/Characters/AI/DefulatUnit_BT.DefulatUnit_BT'";
  weapon_socket_name_ = "LeftHandSocket";
  AIControllerClass = ADefaultUnitController::StaticClass();
  SetAttackRange(125.0f);

  auto weapon_instnace =
    CreateDefaultSubobject<UTwoHandedAxComponent>(TEXT("Weapon_Component"));

  auto attack_range = CreateDefaultSubobject<UBoxAttackRangeComponent>(
    TEXT("AttackRangeComponent"));

  Init(weapon_instnace, attack_range);


}


// Called every frame
void AHuman_TwoHandedAx::Tick(float _delta_time)
{
  Super::Tick(_delta_time);
 
}

void AHuman_TwoHandedAx::OnDamage(int _damega) {

  SJ_ASSERT(_damega);

  //TODO... ??? const;;;
  auto hp_data = GetState().hp_;
  int hp = hp_data.GetValue();
  hp -= _damega;
  if (hp <= 0) {
    //Destroy
  }

  hp_data.SetData(hp);

}
