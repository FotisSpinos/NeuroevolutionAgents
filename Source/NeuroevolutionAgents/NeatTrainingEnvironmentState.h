// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NEAT/neat.h"
#include "NEAT/network.h"
#include "NEAT/population.h"
#include "NEAT/organism.h"
#include "NEAT/genome.h"
#include "NEAT/species.h"

#include "CoreMinimal.h"
#include "BaseEnvironmentState.h"
#include "NeatTrainingEnvironmentState.generated.h"

/**
 * 
 */
UCLASS()
class NEUROEVOLUTIONAGENTS_API ANeatTrainingEnvironmentState : public ABaseEnvironmentState
{
	GENERATED_BODY()

public:
	// Default constructor
	ANeatTrainingEnvironmentState();

	~ANeatTrainingEnvironmentState();

	// state update 
	virtual void UpdateState() override;

	// state init
	virtual void InitState() override;

	class FileWriter* popAnalytics;

protected:
	// The current generation
	int generation;

	// The best recorded fitness
	double bestFitness;

	// The avairage population fitness recorded in the current frame
	double avairagePopulationFitness;

	// the current geenration progress
	int generationProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float generationTimer;

	float generationTimerStore;

	// The population
	class NEAT::Population* population;

	// The genome used to generate the population 
	class NEAT::Genome* genome;
};
