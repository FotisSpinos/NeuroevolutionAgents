// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NEAT/network.h"
#include "string"
#include "NEAT/genome.h"
#include "NEAT/gene.h"

/**
 * 
 */
class NEUROEVOLUTIONAGENTS_API UnrealNNSaverLoader
{
public:
	UnrealNNSaverLoader();
	~UnrealNNSaverLoader();

	void StoreToFile(NEAT::Network* network, const FString filename);
	NEAT::Network* LoadFromFile(FString path);

	void Clear();
private:

	void StoreNodes(std::vector<NEAT::NNode*> nodes, class FileWriter& fileWriter, FString layerInfo);
	void StoreNodeLinks(std::vector<NEAT::Link*> links, FileWriter& fileWriter, FString infoLayer);

	void StoreTrait(NEAT::Trait* trait, FileWriter& fileWriter, FString infoLayer);
	void StoreNetworkParams(double* paramsArray, int paramsNum, FileWriter& fileWriter, FString paramsDesc);

	void StoreGenotype(NEAT::Genome* genotype, FileWriter& fileWriter);

	void StoreTraits(std::vector<NEAT::Trait*> traits, FileWriter& fileWriter);

	NEAT::NNode* GetNodeFromID(std::vector<std::vector<NEAT::NNode*>>& allLayerNodes, int nodeID);
};
