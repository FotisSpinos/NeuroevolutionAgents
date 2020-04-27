// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnvironmentState.h"
#include "Agent.h"
#include "BaseAgent.h"
#include <string>
#include "BaseEnvironmentMaster.h"

ABaseEnvironmentState::ABaseEnvironmentState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseEnvironmentState::AddObservation(int agentIndex, std::vector<float>& observations)
{	
	for (int i = 0; i < observations.size(); i++)
	{
		if (observationsIndeces[agentIndex] > observationsSize)
			return;

		this->observations[agentIndex][observationsIndeces[agentIndex]] = observations[i];
		observationsIndeces[agentIndex]++;
	}
}

void ABaseEnvironmentState::AddObservation(int agentIndex, float observation)
{
	if (observationsIndeces[agentIndex] > observationsSize)
		return;
	
	this->observations[agentIndex][observationsIndeces[agentIndex]] = observation;
	observationsIndeces[agentIndex]++;
}

double ABaseEnvironmentState::GetInput(int agentIndex, int index)
{
	if (index > inputs.size() || index < 0)
		return 0.0f;

	return	inputs[agentIndex][index];
}

void ABaseEnvironmentState::SetFitness(int agentIndex, float fitness)
{
	fitnesses[agentIndex] = fitness;
}

void ABaseEnvironmentState::UpdateState()
{
}

void ABaseEnvironmentState::InitState()
{
}

void ABaseEnvironmentState::ClearObservations()
{
	//	//	PRINT INPUT BEFORE CLEARING DATA
	/*
	for (int i = 0; i < this->observations[0].size(); i++)
	{
		GEngine->AddOnScreenDebugMessage(i, 5.0f, FColor::Yellow,
			FString("Observation: ") + FString::FromInt(i) + FString(" ") + FString(std::to_string(observations[0][i]).c_str())
		);
	}
	*/

	std::fill(observationsIndeces.begin(), observationsIndeces.end(), 0);
}

bool ABaseEnvironmentState::IsEditorInputValid()
{
	// Initialize observations and input vectors
	if (observationsSize <= 0 || inputsSize <= 0)
		return false;
	return true;
}

void ABaseEnvironmentState::BeginPlay()
{
	observationsIndeces = std::vector<int>(ABaseEnvironmentMaster::GetInstance()->GetAgentNumber());

	// Initialize observations and input vectors
	if (observationsSize < 0 || inputsSize < 0)
	{
		observationsSize = 10;
		inputsSize = 3;

		GEngine->AddOnScreenDebugMessage(-1,
			5.f,
			FColor::Red,
			FString::Printf(TEXT("The attributes set to the state objects are invalid")));

		return;
	}
	else
	{
		// Init Observations
		this->observations.resize(ABaseEnvironmentMaster::GetInstance()->GetAgentNumber(), std::vector<float>(observationsSize));

		// Init inputs
		this->inputs.resize(ABaseEnvironmentMaster::GetInstance()->GetAgentNumber(), std::vector<float>(inputsSize));

		// Init fitnesses
		fitnesses = std::vector<double>(ABaseEnvironmentMaster::GetInstance()->GetAgentNumber());
	}
}
