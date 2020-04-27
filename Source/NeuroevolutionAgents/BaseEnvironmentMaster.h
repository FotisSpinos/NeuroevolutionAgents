// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnvironmentMaster.h"
#include "EnemyAgent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEnvironmentMaster.generated.h"

UCLASS()
class NEUROEVOLUTIONAGENTS_API ABaseEnvironmentMaster : public AActor, public IEnvironmentMaster
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnvironmentMaster();
	~ABaseEnvironmentMaster();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	// Returns the input using the index parameter
	double GetInput(int agentIndex, int index) override;

	// Add the observations resieved
	virtual void AddObservation(int agentIndex, std::vector<float>& observations) override;
	virtual void AddObservation(int agentIndex, float observation) override;

	// Clears observations
	virtual void ClearObservations() override;

	virtual void SetFitness(int agentIndex, float fitness) override;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	class ABaseEnvironmentState* environmentState;

	// Returns a static instance of the object
	static ABaseEnvironmentMaster* GetInstance();

	static ABaseEnvironmentMaster* instance;

	virtual bool GetResetRequired() override;

	void ResetRequired();

	virtual void Reset() override;

	int agentCount;

	int GetAgentNumber() {
		return agentCount;
	}

	class FileWriter* GetPopAnalytics() {
		return popAnalytics;
	}

private:
	std::vector<AEnemyAgent*> enemyAgents;

	class FileWriter* popAnalytics;
};
