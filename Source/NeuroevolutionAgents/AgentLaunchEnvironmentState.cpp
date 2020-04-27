// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentLaunchEnvironmentState.h"

#include "NEAT/network.h"
#include "Directories.h"
#include "UnrealNNSaverLoader.h"

void AAgentLaunchEnvironmentState::InitState()
{
	nn = saverLoader->LoadFromFile(TRAINED_AGENTS + FString("BestAgent.txt"));

	nn == nullptr ? validNN = false : validNN = true;
}

void AAgentLaunchEnvironmentState::UpdateState()
{
	//dont allow the state to update if the neural network is invalid
	if (!validNN)
		return;

	// add inputs to the neural network
	nn->load_sensors(observations[0]);

	// activate nn
	nn->activate();

	// get nn outputs
	for (int i = 0; i < nn->outputs.size(); i++)
	{
		inputs[0][i] = nn->outputs[i]->activation - 0.5;
	}

	// flush nn
	nn->flush();
}