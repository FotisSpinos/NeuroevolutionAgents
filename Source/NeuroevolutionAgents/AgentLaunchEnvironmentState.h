// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnvironmentState.h"
#include "AgentLaunchEnvironmentState.generated.h"

/**
 * 
 */
namespace NEAT 
{
	class Network;
}

UCLASS()
class NEUROEVOLUTIONAGENTS_API AAgentLaunchEnvironmentState : public ABaseEnvironmentState
{
	GENERATED_BODY()
	
public:
	// state update 
	virtual void UpdateState() override;

	// state init
	virtual void InitState() override;

private:
	class UnrealNNSaverLoader* saverLoader;

	class NEAT::Network* nn;

	bool validNN;
};
