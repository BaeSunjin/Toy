// Fill out your copyright notice in the Description page of Project Settings.

#include "Human_Archer.h"
#include "Common/PS_Utils.h"
#include "Object/Unit/Controller/DefaultUnitController.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"


AHuman_Archer::AHuman_Archer(const FObjectInitializer & _initializer) :
  Super(_initializer) {

  
  // init data setting
  static_mesh_file_path_ = "SkeletalMesh'/Game/Characters/HumanBowMan/Mesh/erika_archer_bow_arrow.erika_archer_bow_arrow'";
  animation_blueprint_file_path_ = "AnimBlueprint'/Game/Characters/HumanBowMan/Animation/Human_Bow_Man_Anim_BP.Human_Bow_Man_Anim_BP'";
  behavior_tree_file_path_ = "BehaviorTree'/Game/Characters/AI/DefulatUnit_BT.DefulatUnit_BT'";
  weapon_socket_name_ = "LeftHandSocket";
  AIControllerClass = ADefaultUnitController::StaticClass();
  SetAttackRange(125.0f);

  //公扁客 AttackComponent 积己 鞘夸

  //Init();
}

void AHuman_Archer::Tick(float _delta_time) {
  Super::Tick(_delta_time);

}