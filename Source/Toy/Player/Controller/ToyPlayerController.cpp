// Fill out your copyright notice in the Description page of Project Settings.

#include "ToyPlayerController.h"



AToyPlayerController::AToyPlayerController(const FObjectInitializer& _objectInitializer) {

  UE_LOG(LogTemp, Error, TEXT("PlayerControlelr Initializer"));

  bShowMouseCursor = true;
  bEnableClickEvents = true;
  bEnableTouchEvents = true;

}


