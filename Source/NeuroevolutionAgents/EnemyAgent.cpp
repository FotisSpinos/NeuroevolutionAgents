// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAgent.h"
#include "BaseEnvironmentMaster.h"
#include "Engine/World.h"
#include "AngleSensor.h"
#include "Engine/StaticMesh.h"

#include "EnvironmentPlayer.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "FileWriter.h"
#include <string>

// Sets default values
AEnemyAgent::AEnemyAgent()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create angle sensor
	angleSensor = CreateDefaultSubobject<UAngleSensor>(TEXT("Angle Sensor"));

	// Make sure that collisions are enabled
	SetActorEnableCollision(true);

	// the agent can attack
	canAttack = true;

	// set fitness to 0
	fitness = 0.0;

	// attack counter 
	attackCount = 0;

	timeToReachThePlayer = 0.0;
}

// Called when the game starts or when spawned
void AEnemyAgent::BeginAgent()
{
	agentIndex = ABaseEnvironmentMaster::GetInstance()->GetAgentNumber();

	// Set angle sensor attributes
	angleSensor->SetSensorAttributes(visionDetail, visionRange, visionDistance);

	// Set idle mesh
	agentMesh->SetMaterial(0, idleMaterial);

	// Set up default values
	attackRechargeStore = attackRecharge;

	ABaseEnvironmentMaster::GetInstance()->ResetRequired();

	initLocation = GetActorLocation();
	initRotation = GetActorRotation();

	rayHitWallCount = 0;
	rayHitPlayerCount = 0;

	agentMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAgent::OnOverlapBegin);
}

void AEnemyAgent::MoveForward(float amount)
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * amount * moveSpeed * GetWorld()->DeltaTimeSeconds);
}

void AEnemyAgent::TurnRight(float amount)
{
	AddActorLocalRotation(FRotator(0.0f, amount * rotSpeed * GetWorld()->DeltaTimeSeconds, 0.0f));
}

// Called every frame
void AEnemyAgent::UpdateAgent()
{
	// Set angle sensor attributes
	angleSensor->SetSensorAttributes(visionDetail, visionRange, visionDistance);
	angleSensor->EmitSensorRays();

	// Add distance fitness
	FVector playerPosXY = FVector(AEnvironmentPlayer::GetInstance()->GetLocation().X, AEnvironmentPlayer::GetInstance()->GetLocation().Y, 0);
	FVector agentPosXY = FVector(GetActorLocation().X, GetActorLocation().Y, 0);

	FVector playerToAgentVec = playerPosXY - agentPosXY;

	if (playerToAgentVec.Size() > 300.0f)
	{
		timeToReachThePlayer += GetWorld()->GetDeltaSeconds();
	}

	// Apply input
	MoveForward(GetInput(0));
	TurnRight(GetInput(1));


	if (canAttack && GetInput(2) >= 0.5)
	{
		Attack();
		canAttack = false;

	}
	else if(!canAttack)
	{
		if (attackRecharge <= 1)
		{
			// reset the used material
			agentMesh->SetMaterial(0, idleMaterial);
		}
		if (attackRecharge <= 0)
		{
			canAttack = true;
		}
	}

	attackRecharge -= GetWorld()->DeltaTimeSeconds;

	// Add observations
	AddObservation(angleSensor->GetRecordedDistances());

	for (int i = 0; i < angleSensor->GetRecordedDistances().size(); i++)
	{
		FString current = (angleSensor->GetRecordedTags())[i];
		double obs = 0.3;

		if (FString("Wall").Equals(current))
		{
			obs = 0.6;

			if (angleSensor->GetRecordedDistances().at(i) < 0.25f)
			{
				rayHitWallCount++;
			}
		}
		else if (FString("Player").Equals(current))
		{
			if (angleSensor->GetRecordedDistances().at(i) < 0.45f)
			{
				rayHitPlayerCount++;
			}
			obs = 0.9;
		}

		AddObservation(obs);
	}

	//add agent's position
	double agentPosX = agentPosXY.X / 10000;
	double agentPosZ= agentPosXY.Y / 10000;
	AddObservation(agentPosX);
	AddObservation(agentPosZ);

	//add agent's rotation
	double rotation = ((double)GetActorRotation().Euler().Z + 180) / 360 ;
	AddObservation(rotation);


	// add environmet player pos
	double playerX = (double)playerPosXY.X / 10000;
	double playerZ = (double)playerPosXY.Y / 10000;
	AddObservation(playerX);
	AddObservation(playerZ);

	// culculate fitness
	fitness = (1 / playerToAgentVec.Size()) * 300;
	fitness += attackCount * 0.1f;
	fitness += rayHitPlayerCount * 0.05f;
	fitness -= rayHitWallCount * 0.05f;
	//fitness -= timeToReachThePlayer * 0.007;

	// exponetial fitness?
	//fitness = fitness * fitness * fitness * fitness * fitness;

	SetFitness(fitness);
}

void AEnemyAgent::Attack()
{
	attackRecharge = attackRechargeStore;

	//if the player is near the agent
	if (AEnvironmentPlayer::GetInstance() == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-10, 0.3f, FColor::Yellow, FString("Cannot get a player instance"));
		return;
	}

	//set idle mesh
	agentMesh->SetMaterial(0, attackMaterial);

	FVector playerPosXZ = FVector(AEnvironmentPlayer::GetInstance()->GetLocation().X, AEnvironmentPlayer::GetInstance()->GetLocation().Y, 0);
	FVector agentPosXZ = FVector(GetActorLocation().X, GetActorLocation().Y, 0);

	// culculate the player to angle vector
	FVector playerToAgentVec = playerPosXZ - agentPosXZ;

	// culculate the player distance
	float playerDistance = playerToAgentVec.Size();

	// noralize player to angle vector
	playerToAgentVec.Normalize();

	// Culc player angle
	float playerAnge = acos(FVector::DotProduct(playerToAgentVec, GetActorForwardVector()));

	// if the attack is successful 
	if (playerDistance < 300.0f && 
		playerAnge < 0.5f)
	{
		attackCount++;
	}
}

int AEnemyAgent::GetAttackCount()
{
	return attackCount;
}

void AEnemyAgent::Reset()
{
	// reset fitness
	fitness = 0.0;

	// reset actor's position and rotation
	SetActorRotation(initRotation);
	SetActorLocation(initLocation);

	// reset count variables
	attackCount = 0;
	rayHitPlayerCount = 0;
	rayHitWallCount = 0;

	// reset time to reach to player
	timeToReachThePlayer = 0.0f;
}


void AEnemyAgent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
	}
}