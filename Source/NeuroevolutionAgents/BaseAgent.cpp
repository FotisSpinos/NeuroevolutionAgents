// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAgent.h"
#include "BaseEnvironmentMaster.h"

int IAgent::totalAgents = 0;

// Sets default values
ABaseAgent::ABaseAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseAgent::BeginPlay()
{
	Super::BeginPlay();
}

double ABaseAgent::GetInput(int index)
{
	if (ABaseEnvironmentMaster::GetInstance() == nullptr)
		return 0.0f;

	return ABaseEnvironmentMaster::GetInstance()->GetInput(agentIndex - 1, index);
}

void ABaseAgent::AddObservation(std::vector<float>& observations)
{
	if (ABaseEnvironmentMaster::GetInstance() == nullptr)
		return;

	ABaseEnvironmentMaster::GetInstance()->AddObservation((agentIndex - 1), observations);
}

void ABaseAgent::AddObservation(float observation)
{
	if (ABaseEnvironmentMaster::GetInstance() == nullptr)
		return;

	ABaseEnvironmentMaster::GetInstance()->AddObservation(agentIndex - 1, observation);
}

void ABaseAgent::SetFitness(float fitness)
{
	if (ABaseEnvironmentMaster::GetInstance() == nullptr)
		return;

	ABaseEnvironmentMaster::GetInstance()->SetFitness(agentIndex - 1, fitness);
}

void ABaseAgent::Reset()
{
}

void ABaseAgent::UpdateAgent()
{
}

void ABaseAgent::BeginAgent()
{
}

// Called every frame
void ABaseAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
