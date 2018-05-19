// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultUnit.h"
#include "Common/PS_Utils.h"
#include "Object/Squad/Squad.h"
#include "Object/Unit/Controller/DefaultUnitController.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

// Sets default values
ADefaultUnit::ADefaultUnit()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  // ------------colision init --------------

  FCollisionResponseContainer collision_response;
  collision_response.SetResponse(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
  collision_response.SetResponse(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
  collision_response.SetResponse(ECollisionChannel::ECC_MouseRay, ECollisionResponse::ECR_Block);
  collision_response.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


  GetCapsuleComponent()->SetCollisionResponseToChannels(collision_response);
  //RootComponent = GetCapsuleComponent();


// -------- mesh init ----------------
  high_light_component_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighLight_Mesh"));
  high_light_component_->SetVisibility(false);
  high_light_component_->SetupAttachment(RootComponent);
  high_light_component_->RelativeLocation = FVector(0, 0, -85.0f);
  high_light_component_->SetCollisionEnabled(ECollisionEnabled::NoCollision);


  static ConstructorHelpers::FObjectFinder<UStaticMesh> HL_Mesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
  SJ_ASSERT(HL_Mesh.Object) {
    high_light_component_->SetStaticMesh(HL_Mesh.Object);
  }

  static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Characters/Materials/Unit_HighLight.Unit_HighLight'"));

  if (Material.Succeeded())
  {
    //auto instnace_mat = UMaterialInstanceDynamic::Create(Material.Object, this);
    high_light_component_->SetMaterial(0, Material.Object);
  }


  // override function
  UnitMeshInit();


  //----- Setting Controller-----------------


  AIControllerClass = ADefaultUnitController::StaticClass();
  AutoPossessAI = EAutoPossessAI::Spawned;


  //---------- suquad  ------------------

  UnitIntervalInit();

  //--------- default value ------------

  vertical_interval_ = 200.0f;
  horizontal_interval_ = 200.0f;

  //---- Helper UI

  static ConstructorHelpers::FObjectFinder<UBlueprint> Test_Billboard(TEXT("Blueprint'/Game/UI/WUI/Solider_Idx_.Solider_Idx_'"));
  if (Test_Billboard.Succeeded())
  {

    test_billbard_generateed_class_ = Test_Billboard.Object->GeneratedClass;

  }

}

// Called when the game starts or when spawned
void ADefaultUnit::BeginPlay()
{
  Super::BeginPlay();

  // setting test billbard
  test_billboard_ = GetWorld()->SpawnActor(test_billbard_generateed_class_);
  test_billboard_->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
  test_billboard_->SetActorRelativeLocation(FVector(0.0f, 0.0f, 120.0f));



}

const TWeakObjectPtr<ASquad>& ADefaultUnit::GetSquad() {

  return squad_;

}

void ADefaultUnit::SetSquad(const TWeakObjectPtr<ASquad>& data) {
  squad_ = data;
}


void ADefaultUnit::SetHighLight(bool _light_on) {

  //UE_LOG(LogTemp, Error, TEXT("On Unit HighLight"));
  high_light_component_->SetVisibility(_light_on);


}

float ADefaultUnit::GetHirzontalInterval() {
  return horizontal_interval_;
}

float ADefaultUnit::GetVerticalInterval() {
  return vertical_interval_;
}

void ADefaultUnit::UnitMeshInit() {

  static ConstructorHelpers::FObjectFinder<USkeletalMesh> new_unit_mesh(TEXT("SkeletalMesh'/Game/Characters/HeroTPP/HeroTPP.HeroTPP'"));
  SJ_ASSERT(new_unit_mesh.Object);

  unit_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Unit_Mesh"));
  unit_mesh_->SetupAttachment(RootComponent);
  unit_mesh_->RelativeLocation = FVector(0.0f, 0.0f, -90.0f);
  unit_mesh_->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);

  unit_mesh_->SetSkeletalMesh(new_unit_mesh.Object);
  unit_mesh_->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ADefaultUnit::UnitIntervalInit() {

  vertical_interval_ = 100.0f;
  horizontal_interval_ = 100.0f;

}



void ADefaultUnit::MoveTo(const FVector& _move_pos,
                          const FVector& _normal_look_at) const
{

  SJ_ASSERT(_normal_look_at.IsNormalized());
  auto ai_controller = Cast<ADefaultUnitController>(GetController());

  ai_controller->MoveToLocation(_move_pos);

  log_pos_ = _move_pos;
}


// Called every frame
void ADefaultUnit::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  DrawDebugLine(GetWorld(), log_pos_, log_pos_ + FVector(0, 0, 100.f), FColor::Red);

}

// Called to bind functionality to input
void ADefaultUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

}

