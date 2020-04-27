// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Agent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAgent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NEUROEVOLUTIONAGENTS_API IAgent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:

	virtual double GetInput(int index);
	virtual void AddObservation(std::vector<float>& observations);
	virtual void AddObservation(float observation);
	virtual void SetFitness(float fitness) = 0;

	virtual void Reset() = 0;
	virtual void UpdateAgent() = 0;
	virtual void BeginAgent() = 0;

	int agentIndex;
	static int totalAgents;

	double fitness;
};
