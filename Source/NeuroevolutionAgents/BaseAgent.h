// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Agent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAgent.generated.h"

UCLASS()
class NEUROEVOLUTIONAGENTS_API ABaseAgent : public AActor, public IAgent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseAgent();

	// Called every frame
	void Tick(float DeltaTime) override;

protected:

	// Called when the game starts or when spawned
	void BeginPlay() override;

	double GetInput(int index) override;

	void AddObservation(std::vector<float>& observations) override;

	void AddObservation(float observation) override;

	virtual void SetFitness(float fitness) override;

	virtual void Reset() override;

	virtual void UpdateAgent() override;

	virtual void BeginAgent() override;
};
