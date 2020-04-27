// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"


AEnvironmentPlayer* AEnvironmentPlayer::instance;

// Sets default values
AEnvironmentPlayer::AEnvironmentPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	playerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	playerMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnvironmentPlayer::BeginPlay()
{
	Super::BeginPlay();

	instance = this;

	spawnLocation = GetActorLocation();
	
	//SpawnPlayer();
}

void AEnvironmentPlayer::SpawnPlayer()
{
	FVector offset(
		(FMath::FRand() * 2 - 1) * 8000,
		(FMath::FRand() * 2 - 1) * 8000,
		0);

	SetActorLocation(spawnLocation + offset);
}

// Called every frame
void AEnvironmentPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AEnvironmentPlayer::GetLocation()
{
	return GetActorLocation();
}

AEnvironmentPlayer* AEnvironmentPlayer::GetInstance()
{
	return instance;
}

void AEnvironmentPlayer::Reset()
{
	SpawnPlayer();
}

