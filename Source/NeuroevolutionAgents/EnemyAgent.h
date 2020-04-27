// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseAgent.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyAgent.generated.h"

UCLASS()
class NEUROEVOLUTIONAGENTS_API AEnemyAgent : public ABaseAgent
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyAgent();

	// Called when the game starts or when spawned
	void BeginAgent() override;

	// The angle sendor observing the environment
	class UAngleSensor* angleSensor;

	// Moves pawn towards it's forward direction
	virtual void MoveForward(float speed);

	// Rotates the pawn around the Z axis
	virtual void TurnRight(float rotDelta);

	// Called every frame
	void UpdateAgent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float moveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float rotSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	class UStaticMeshComponent* agentMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	class UMaterialInterface* idleMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	class UMaterialInterface* attackMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float attackRecharge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle Sensor Attributes")
	float visionRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle Sensor Attributes")
	float visionDetail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle Sensor Attributes")
	float visionDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	class USphereComponent* collisionSphere;

	int GetAttackCount();

	virtual void Reset() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void Attack();

	float attackRechargeStore;
	bool canAttack;

	FVector initLocation;
	FRotator initRotation;

	int attackCount;
	int rayHitWallCount;
	int rayHitPlayerCount;

	float timeToReachThePlayer;
};