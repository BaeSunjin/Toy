// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultUnitController.h"
#include "Navigation/CrowdFollowingComponent.h"


ADefaultUnitController::ADefaultUnitController(const FObjectInitializer& ObjectInitializer) 
  : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) 
{

}