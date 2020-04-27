// Fill out your copyright notice in the Description page of Project Settings.
// https://github.com/wxFormBuilder/ticpp

#include "NeatTrainingEnvironmentState.h"
#include "Engine/World.h"

#include "BaseEnvironmentMaster.h"

#include "EnvironmentPlayer.h"

#include "FileWriter.h"

#include "Misc/Paths.h"

#include "Engine/World.h"

#include "GameFramework/WorldSettings.h"

#include "UnrealNNSaverLoader.h"

#include "Directories.h"

ANeatTrainingEnvironmentState::ANeatTrainingEnvironmentState()
{
	bestFitness = -INFINITY;
	avairagePopulationFitness = 0;

	generationTimer = 0;
}

ANeatTrainingEnvironmentState::~ANeatTrainingEnvironmentState()
{
	delete popAnalytics;
	delete population;
	delete genome;
}

void ANeatTrainingEnvironmentState::UpdateState()
{
	GEngine->AddOnScreenDebugMessage(3, 30.3f, FColor::Yellow, FString("Generation Avairage Fitness") + FString(std::to_string(avairagePopulationFitness).c_str()));
	GEngine->AddOnScreenDebugMessage(2, 0.3f, FColor::Yellow, FString("Number of organisms: ") + FString(std::to_string(population->organisms.size()).c_str()));
	GEngine->AddOnScreenDebugMessage(1, 0.3f, FColor::Yellow, FString("Currrent Generation: ") + FString::FromInt(generation));

	// set inputs for each organism	
	for (int orgIndx = 0; orgIndx < population->organisms.size(); orgIndx++)
	{
		population->organisms[orgIndx]->net->load_sensors(observations[orgIndx]);
	}

	// get the outputs for each organism
	for (int orgIndx = 0; orgIndx < population->organisms.size(); orgIndx++)
	{
		population->organisms[orgIndx]->net->activate();

		for (int i = 0; i < population->organisms[orgIndx]->net->outputs.size(); i++)
		{
			inputs[orgIndx][i] = population->organisms[orgIndx]->net->outputs[i]->activation - 0.5;//* 2 - 1;
		}
		population->organisms[orgIndx]->net->flush();
	}
	
	generationTimer -= GetWorld()->DeltaTimeSeconds;

	//generate new population
	if (generationTimer <= 0)
	{
		generationTimer = generationTimerStore;

		avairagePopulationFitness = 0.0;
		bestFitness = population->organisms[0]->fitness;
		NEAT::Organism* bestOrganism = population->organisms[0];

		//Set fitness and record avairage fitness
		for (int i = 0; i < population->organisms.size(); i++)
		{
			population->organisms[i]->fitness = fitnesses[i];

			if (bestFitness < population->organisms[i]->fitness)
			{
				// record the best fitness of the current geenration
				bestFitness = population->organisms[i]->fitness;

				//record the best organism of the current generation
				bestOrganism = population->organisms[i];
			}
				
			avairagePopulationFitness += population->organisms[i]->fitness;
		}


		avairagePopulationFitness /= population->organisms.size();

		ABaseEnvironmentMaster::GetInstance()->GetPopAnalytics()->RecordContents(FString("generation"), generation);
		ABaseEnvironmentMaster::GetInstance()->GetPopAnalytics()->RecordContents(FString("population fitness"), (float)avairagePopulationFitness);
		ABaseEnvironmentMaster::GetInstance()->GetPopAnalytics()->RecordContents(FString("best generation fitness"), (float)bestFitness);
		ABaseEnvironmentMaster::GetInstance()->GetPopAnalytics()->SaveDataToFile();

		ABaseEnvironmentMaster::GetInstance()->GetPopAnalytics()->RecordContents(FString("\n"));

		UnrealNNSaverLoader saverLoader;
		saverLoader.StoreToFile(bestOrganism->net, FString(""));
		NEAT::Network* network = saverLoader.LoadFromFile(ANALYTICS);

		//DEBUG CODE
		if (network == bestOrganism->net)
		{
			UE_LOG(LogTemp, Warning, TEXT("UnrealNNSaverLoader works properly"));
		}

		generation++;

		// generate new population
		population->epoch(generation);

		//reset generation
		generationProgress = 0.0f;

		ABaseEnvironmentMaster::GetInstance()->Reset();
	}

	// update generation progress
	generationProgress++;
}

void ANeatTrainingEnvironmentState::InitState()
{
	// Initialize variables
	generation = 0;
	generationProgress = 0.0f;

	if (generationTimer == 0)
		generationTimer = 200.0f;

	generationTimerStore = generationTimer;

	// Create Gnome
	genome = new NEAT::Genome(1, observationsSize, inputsSize, 5, 15, false, 0.2);

	// Generate population from single Gnome
	population = new NEAT::Population(genome, ABaseEnvironmentMaster::GetInstance()->GetAgentNumber() - 1);

	// accelerate environment
	GetWorldSettings()->SetTimeDilation(1000.0f);

	using namespace NEAT;
	// NEAT variables initialization for feuture work load these data from a file
	NEAT::trait_param_mut_prob = 0.5;
	NEAT::trait_mutation_power = 1.5;
	NEAT::linktrait_mut_sig = 1.0;
	NEAT::nodetrait_mut_sig = 0.5;
	NEAT::weight_mut_power = 3.5;
	NEAT::recur_prob = 0.1;
	NEAT::disjoint_coeff = 1.0;
	NEAT::excess_coeff = 1.0;
	NEAT::mutdiff_coeff = 0.4;
	NEAT::compat_threshold = 3.0;
	NEAT::age_significance = 1.0;
	NEAT::survival_thresh = 0.15;
	NEAT::mutate_only_prob = 0.2;
	NEAT::mutate_random_trait_prob = 0.6;
	NEAT::mutate_link_trait_prob = 0.1;
	NEAT::mutate_node_trait_prob = 0.1;
	NEAT::mutate_link_weights_prob = 0.1;
	NEAT::mutate_toggle_enable_prob = 0.25;
	NEAT::mutate_gene_reenable_prob = 0.25;
	NEAT::mutate_add_node_prob = 0.33;
	NEAT::mutate_add_link_prob = 0.35;
	NEAT::interspecies_mate_rate = 0.6;
	NEAT::mate_multipoint_prob = 0.4;
	NEAT::mate_multipoint_avg_prob = 0.4;
	NEAT::mate_singlepoint_prob = 0.3;
	NEAT::mate_only_prob = 0.1;
	NEAT::recur_only_prob = 0.2;
	NEAT::pop_size = ABaseEnvironmentMaster::GetInstance()->GetAgentNumber() - 1;
	NEAT::dropoff_age = 10;
	NEAT::newlink_tries = 20;
	NEAT::print_every = 30;
	NEAT::babies_stolen = 1;
	NEAT::num_runs = 1;
}

/*

	// NEAT variables initialization for feuture work load these data from a file
	NEAT::trait_param_mut_prob = 0.5;
	NEAT::trait_mutation_power = 1.0;
	NEAT::linktrait_mut_sig = 1.0;
	NEAT::nodetrait_mut_sig = 0.5;
	NEAT::weight_mut_power = 2.5;
	NEAT::recur_prob = 0.1;
	NEAT::disjoint_coeff = 1.0;
	NEAT::excess_coeff = 1.0;
	NEAT::mutdiff_coeff = 0.4;
	NEAT::compat_threshold = 3.0;
	NEAT::age_significance = 1.0;
	NEAT::survival_thresh = 0.25;
	NEAT::mutate_only_prob = 0.25;
	NEAT::mutate_random_trait_prob = 0.2;
	NEAT::mutate_link_trait_prob = 0.2;
	NEAT::mutate_node_trait_prob = 0.2;
	NEAT::mutate_link_weights_prob = 0.3;
	NEAT::mutate_toggle_enable_prob = 0.15;
	NEAT::mutate_gene_reenable_prob = 0.15;
	NEAT::mutate_add_node_prob = 0.23;
	NEAT::mutate_add_link_prob = 0.25;
	NEAT::interspecies_mate_rate = 0.01;
	NEAT::mate_multipoint_prob = 0.5;
	NEAT::mate_multipoint_avg_prob = 0.3;
	NEAT::mate_singlepoint_prob = 0.2;
	NEAT::mate_only_prob = 0.2;
	NEAT::recur_only_prob = 0.0;
	NEAT::pop_size = ABaseEnvironmentMaster::GetInstance()->GetAgentNumber() - 1;
	NEAT::dropoff_age = 20;
	NEAT::newlink_tries = 20;
	NEAT::print_every = 30;
	NEAT::babies_stolen = 0;
	NEAT::num_runs = 1;
*/

/*
BEST CONFIG SO FAR

	NEAT::trait_param_mut_prob = 0.5;
	NEAT::trait_mutation_power = 1.0;
	NEAT::linktrait_mut_sig = 1.0;
	NEAT::nodetrait_mut_sig = 0.5;
	NEAT::weight_mut_power = 2.5;
	NEAT::recur_prob = 0.00;
	NEAT::disjoint_coeff = 1.0;
	NEAT::excess_coeff = 1.0;
	NEAT::mutdiff_coeff = 0.4;
	NEAT::compat_threshold = 2.0;
	NEAT::age_significance = 1.0;
	NEAT::survival_thresh = 0.20;
	NEAT::mutate_only_prob = 0.25;
	NEAT::mutate_random_trait_prob = 0.1;
	NEAT::mutate_link_trait_prob = 0.1;
	NEAT::mutate_node_trait_prob = 0.1;
	NEAT::mutate_link_weights_prob = 0.3;
	NEAT::mutate_toggle_enable_prob = 0.00;
	NEAT::mutate_gene_reenable_prob = 0.000;
	NEAT::mutate_add_node_prob = 0.03;
	NEAT::mutate_add_link_prob = 0.05;
	NEAT::interspecies_mate_rate = 0.001;
	NEAT::mate_multipoint_prob = 0.3;
	NEAT::mate_multipoint_avg_prob = 0.4;
	NEAT::mate_singlepoint_prob = 0.0;
	NEAT::mate_only_prob = 0.4;
	NEAT::recur_only_prob = 0.0;
	NEAT::pop_size = ABaseEnvironmentMaster::GetInstance()->GetAgentNumber() - 1; //150;
	NEAT::dropoff_age = 15; //15;
	NEAT::newlink_tries = 20;
	NEAT::print_every = 30;
	NEAT::babies_stolen = 0;
	NEAT::num_runs = 0;
*/