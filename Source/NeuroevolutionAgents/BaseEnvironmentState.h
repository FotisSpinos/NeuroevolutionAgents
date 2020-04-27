// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "EnvironmentState.h"
#include <vector>
#include <string>
#include "EnvironmentState.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseEnvironmentState.generated.h"

UCLASS()
class NEUROEVOLUTIONAGENTS_API ABaseEnvironmentState : public APawn, public IEnvironmentState
{
	GENERATED_BODY()
	
public:	
	ABaseEnvironmentState();

	virtual void AddObservation(int agentIndex, std::vector<float>& observations) override;
	virtual void AddObservation(int agentIndex, float observation) override;

	virtual double GetInput(int agentIndex, int index) override;

	virtual void SetFitness(int agentIndex, float fitness) override;

	virtual void UpdateState();
	virtual void InitState();

	virtual void ClearObservations();

	virtual bool IsEditorInputValid();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int observationsSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int inputsSize;
};
