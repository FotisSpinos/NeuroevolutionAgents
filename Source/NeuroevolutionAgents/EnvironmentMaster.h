// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnvironmentMaster.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnvironmentMaster : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NEUROEVOLUTIONAGENTS_API IEnvironmentMaster
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	IEnvironmentMaster() {};

	// Sets the observations resieved
	virtual void AddObservation(int agentIndex, std::vector<float>& observations) = 0;

	// Sets the observations resieved
	virtual void AddObservation(int agentIndex, float observation) = 0;

	// Returns the input using the index parameter
	virtual double GetInput(int agentIndex, int index) = 0;

	virtual void SetFitness(int agentIndex, float fitness) = 0;

	// clears observations
	virtual void ClearObservations() = 0;

	// resets data
	virtual void Reset() = 0;

	// 
	virtual bool GetResetRequired() = 0;

	//
	bool resetRequired;
};
