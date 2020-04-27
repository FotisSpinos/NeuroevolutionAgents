// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnvironmentState.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnvironmentState : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NEUROEVOLUTIONAGENTS_API IEnvironmentState
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	// the vector of observations
	std::vector<std::vector<float>> observations;

	// the vector holding the inputs applied to the actor agent
	std::vector<std::vector<float>> inputs;

	std::vector<int> observationsIndeces;

	// the vector holding the recorded fitness values recorded on each frame
	std::vector<double> fitnesses;

	// Functionality to add observations to the vector
	virtual void AddObservation(int agentIndex, std::vector<float>& observations) = 0;
	virtual void AddObservation(int agentIndex, float observation) = 0;

	virtual bool IsEditorInputValid() = 0;

	// Retrieve recorded inputs
	virtual double GetInput(int agentIndex, int index) = 0;

	// Provides functionality to clear observations
	virtual void ClearObservations() = 0;

	// Set the fitness
	virtual void SetFitness(int agentIndex, float fitness) = 0;
};
