// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePlayer.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnvironmentPlayer.generated.h"

UCLASS()
class NEUROEVOLUTIONAGENTS_API AEnvironmentPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnvironmentPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetLocation();

	static AEnvironmentPlayer* GetInstance();

	virtual void Reset();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* playerMesh;

	static AEnvironmentPlayer* instance;

	FVector spawnLocation;

	void SpawnPlayer();
};
