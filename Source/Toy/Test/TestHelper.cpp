// Fill out your copyright notice in the Description page of Project Settings.

#include "TestHelper.h"
#include "Object/Squad/Squad.h"
#include "Common/PS_Utils.h"
#include "Object/Unit/Character/DefaultUnit.h"


// Sets default values
ATestHelper::ATestHelper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}


void ATestHelper::MakeDefaultUnit(FString pos_name) {

}
// Called when the game starts or when spawned
void ATestHelper::BeginPlay()
{
	Super::BeginPlay();

	MakeDefaultUnit(FString("SpawnPosition"));
	/*TArray<int> test_array;
	test_array.Add(9);
	test_array.Add(8);
	test_array.Add(7);
	test_array.Add(6);
	test_array.Add(5);

	test_array.HeapSort([](const int& A, const int& B) {
	  return A < B;
	});

	for (auto value : test_array) {
	  UE_LOG(LogTemp, Error, TEXT("Value is %d"), value);
	}*/

}

// Called every frame
void ATestHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

