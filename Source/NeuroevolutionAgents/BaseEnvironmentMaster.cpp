// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnvironmentMaster.h"
#include "BaseEnvironmentState.h"

#include "EngineUtils.h"

#include "FileWriter.h"

#include "Engine/World.h"
#include <EnvironmentPlayer.h>

ABaseEnvironmentMaster* ABaseEnvironmentMaster::instance;

// Sets default values
ABaseEnvironmentMaster::ABaseEnvironmentMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	instance = this;

	agentCount = 1;

	popAnalytics = new FileWriter(FPaths::GetProjectFilePath() + "/../Analytics/pop_analytics.txt");
}

void ABaseEnvironmentMaster::AddObservation(int agentIndex, std::vector<float>& observations)
{
	environmentState->AddObservation(agentIndex, observations);
}

void ABaseEnvironmentMaster::AddObservation(int agentIndex, float observation)
{
	environmentState->AddObservation(agentIndex, observation);
}

bool ABaseEnvironmentMaster::GetResetRequired()
{
	return resetRequired;
}

void ABaseEnvironmentMaster::ResetRequired()
{
	resetRequired = true;
}

void ABaseEnvironmentMaster::ClearObservations()
{
	environmentState->ClearObservations();
}

void ABaseEnvironmentMaster::SetFitness(int agentIndex, float fitness)
{
	environmentState->SetFitness(agentIndex, fitness);
}

void ABaseEnvironmentMaster::Reset()
{
	int attackSum = 0;

	// reset all agents
	for (int i = 0; i < enemyAgents.size(); i++)
	{
		attackSum += enemyAgents[i]->GetAttackCount();
		enemyAgents[i]->Reset();
	}

	// add avairage 
	popAnalytics->RecordContents(FString("avairage attacks"), (float) (attackSum / enemyAgents.size()));
	popAnalytics->SaveDataToFile();

	//AEnvironmentPlayer::GetInstance()->Reset();
}

ABaseEnvironmentMaster* ABaseEnvironmentMaster::GetInstance()
{
	return instance;
}

double ABaseEnvironmentMaster::GetInput(int agentIndex, int index)
{
	if (environmentState == nullptr)
		return 0.0f;

	return environmentState->GetInput(agentIndex, index);
}

ABaseEnvironmentMaster::~ABaseEnvironmentMaster()
{
	delete popAnalytics;
}

// Called every frame
void ABaseEnvironmentMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Clear observations
	environmentState->ClearObservations();

	// Update all agents
	for (int i = 0; i < enemyAgents.size(); i++)
	{
		enemyAgents.at(i)->UpdateAgent();
	}

	// Update State
	environmentState->UpdateState();
}

void ABaseEnvironmentMaster::BeginPlay()
{
	Super::BeginPlay();

	// Set instance
	instance = this;

	//Find all agents
	for (TActorIterator<AEnemyAgent> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		enemyAgents.push_back(*ActorItr);
		ActorItr->BeginAgent();
		agentCount++;
	}

	// Init state
	environmentState->InitState();


	popAnalytics->RecordContents(FString("Title"), FString("This file contains population data resulting from training"));
	popAnalytics->RecordContents(FString("\n"));

	popAnalytics->RecordContents(FString("Population size"), agentCount);
}