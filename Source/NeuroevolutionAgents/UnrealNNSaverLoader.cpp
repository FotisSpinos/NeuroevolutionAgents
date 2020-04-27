// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealNNSaverLoader.h"
#include "FileWriter.h"
#include "EngineUtils.h"
#include <Runtime\Core\Public\HAL\PlatformFilemanager.h>
#include <Runtime\Core\Public\Misc\FileHelper.h>

UnrealNNSaverLoader::UnrealNNSaverLoader()
{
	
}

UnrealNNSaverLoader::~UnrealNNSaverLoader()
{
}

void UnrealNNSaverLoader::StoreToFile(NEAT::Network* network, const FString path)
{
	FileWriter fileWriter(path);

	// record max weight
	fileWriter.RecordContents(FString("max_weight"), network->maxweight);

	// store inputs
	StoreNodes(network->inputs, fileWriter, FString("input"));
	fileWriter.RecordContents("\n");

	// store outputs
	StoreNodes(network->outputs, fileWriter, FString("output"));
	fileWriter.RecordContents("\n");

	// store all nodes
	StoreNodes(network->all_nodes, fileWriter, FString("hidden"));
	fileWriter.RecordContents("\n");

	// store genotype - is it required?
	//StoreGenotype(network->genotype, fileWriter);

	fileWriter.SaveDataToFile();
}

void UnrealNNSaverLoader::StoreNodes(std::vector<NEAT::NNode*> nodes, FileWriter& fileWriter, FString layerInfo)
{
	fileWriter.RecordContents("Layer data ->");

	// add input nodes
	for (int i = 0; i < nodes.size(); i++)
	{
		fileWriter.RecordContents("Node data ->");

		// add layer info
		fileWriter.RecordContents(FString("layer info"), layerInfo);

		// store active_flag
		fileWriter.RecordContents(FString("active_flag"), nodes[i]->active_flag);

		// store activesum
		fileWriter.RecordContents(FString("activesum"), nodes[i]->activesum);

		// store activation 
		fileWriter.RecordContents(FString("activation"), nodes[i]->activation);

		// output = nnode.output;
		fileWriter.RecordContents(FString("output"), nodes[i]->output);

		//type = nnode.type; //NEURON or SENSOR type
		fileWriter.RecordContents(FString("nn type"), nodes[i]->type);

		//node_id = nnode.node_id;
		fileWriter.RecordContents(FString("id"), nodes[i]->node_id);

		//ftype = nnode.ftype;
		fileWriter.RecordContents(FString("ftype"), nodes[i]->ftype);

		//gen_node_label = nnode.gen_node_label;
		fileWriter.RecordContents(FString("generation node label"), nodes[i]->gen_node_label);

		//frozen = nnode.frozen;
		fileWriter.RecordContents(FString("frozen"), nodes[i]->frozen);

		// record incomming links
		StoreNodeLinks(nodes[i]->incoming, fileWriter, FString("incomming links data ->"));

		StoreNodeLinks(nodes[i]->outgoing, fileWriter, FString("outgoing links data ->"));

		fileWriter.RecordContents("\n");
	}
}

void UnrealNNSaverLoader::StoreNodeLinks(std::vector<NEAT::Link*> links, FileWriter& fileWriter, FString infoLayer)
{
	fileWriter.RecordContents(infoLayer);

	for (int i = 0; i < links.size(); i++)
	{
		fileWriter.RecordContents(FString("added_weight"), links[i]->added_weight);

		fileWriter.RecordContents(FString("in_node_id"), links[i]->in_node->node_id);

		fileWriter.RecordContents(FString("is_recurrent"), links[i]->is_recurrent);

		fileWriter.RecordContents(FString("out_node_id"), links[i]->out_node->node_id);

		fileWriter.RecordContents(FString("time_delay"), links[i]->time_delay);

		fileWriter.RecordContents(FString("trait_id"), links[i]->trait_id);

		fileWriter.RecordContents(FString("weight"), links[i]->weight);

		// fileWriter.RecordContents(FString("linktrait"), links[i]->linktrait);
		StoreTrait(links[i]->linktrait, fileWriter, infoLayer + FString("link_trait"));

		StoreNetworkParams(links[i]->params, NEAT::num_trait_params, fileWriter, infoLayer + FString("links_params"));
	}
}

void UnrealNNSaverLoader::StoreTrait(NEAT::Trait* trait, FileWriter& fileWriter, FString infoLayer)
{
	fileWriter.RecordContents("Trait data ->");

	if (trait == nullptr)
		return;

	StoreNetworkParams((trait->params), NEAT::num_trait_params, fileWriter, infoLayer + "trait_params");

	fileWriter.RecordContents("trait_id", trait->trait_id);
}

void UnrealNNSaverLoader::StoreNetworkParams(double* paramsArray, int paramsNum, FileWriter& fileWriter, FString paramsDesc)
{
	int counter = 0;
	while (paramsArray != nullptr && counter < paramsNum)
	{
		fileWriter.RecordContents(paramsDesc, *paramsArray);
		paramsArray++;

		counter++;
	}
}

void UnrealNNSaverLoader::StoreGenotype(NEAT::Genome* genotype, FileWriter& fileWriter)
{
	// store nodes
	StoreNodes(genotype->nodes, fileWriter, FString("Genotype nodes-->"));
	
	// store traits
	StoreTraits(genotype->traits, fileWriter);
	
	// store network
	//genotype->phenotype

	// store genes
	//genotype->genes

	
}

void UnrealNNSaverLoader::StoreTraits(std::vector<NEAT::Trait*> traits, FileWriter& fileWriter)
{
	for (int i = 0; i < traits.size(); i++)
	{
		StoreTrait(traits[i], fileWriter, FString("Genotype trait"));
	}
}

NEAT::NNode* UnrealNNSaverLoader::GetNodeFromID(std::vector<std::vector<NEAT::NNode*>>& allLayerNodes, int nodeID)
{
	for (int i = 0; i < allLayerNodes.size(); i++)
	{
		for (int y = 0; y < allLayerNodes[i].size(); y++)
		{
			if (allLayerNodes[i][y]->node_id == nodeID)
			{
				return allLayerNodes[i][y];
			}
		}
	}

	return nullptr;
}

// TODO? https://wiki.unrealengine.com/File_Management,_Create_Folders,_Delete_Files,_and_More#Create_Text_Files

NEAT::Network* UnrealNNSaverLoader::LoadFromFile(FString path)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*path))
	{
		UE_LOG(LogTemp, Warning, TEXT("File could not be found"));
		return nullptr;
	}
 
	const int64 FileSize = FPlatformFileManager::Get().GetPlatformFile().FileSize(*path);

	FString txtData = "";
	FFileHelper::LoadFileToString(txtData, *path);

	FString tag = "";
	FString value = "";
	
	int32 previousElementIndex = 0;

	std::vector<NEAT::NNode*> inputNodes;
	std::vector<NEAT::NNode*> outputNodes;
	std::vector<NEAT::NNode*> allNodes;

	std::vector<NEAT::Link*> outgoingLinks;
	std::vector<NEAT::Link*> incommingLinks;
	NEAT::Link* currentLink = new NEAT::Link(0.0);

	int traitParamsCounter = 0;

	NEAT::Network* output = new NEAT::Network(inputNodes, outputNodes, allNodes, 1);

	// define current node
	NEAT::NNode* currentNode = nullptr;

	for (int32 i = 0; i < txtData.Len(); i++)
	{
		if (txtData[i] == '\n')
		{
			// define the value
			value = txtData.Mid(previousElementIndex, i - previousElementIndex);

			// code to check the value and store network elements
			if (tag == "max_weight")
			{
				output->maxweight = FCString::Atof(*value);
			}
			else if (tag == "active_flag")
			{
				currentNode->active_flag = (bool) FCString::Atoi(*value);
			}
			else if (tag == "activesum")
			{
				currentNode->activesum = FCString::Atof(*value);
			}
			else if (tag == "activation")
			{
				currentNode->activation = FCString::Atof(*value);
			}
			else if (tag == "output")
			{
				currentNode->output = FCString::Atof(*value);
			}
			else if (tag == "nn type")
			{
				currentNode->type = (NEAT::nodetype) FCString::Atoi(*value);
			}
			else if (tag == "id")
			{
				currentNode->node_id = FCString::Atoi(*value);
			}
			else if (tag == "ftype")
			{
				currentNode->ftype = (NEAT::functype) FCString::Atoi(*value);
			}
			else if (tag == "generation node label")
			{
				currentNode->gen_node_label = (NEAT::nodeplace) FCString::Atoi(*value);
			}
			else if (tag == "frozen")
			{
				currentNode->frozen = (bool) FCString::Atoi(*value);
			}
			else if (tag == "added_weight")
			{
				currentLink->added_weight = FCString::Atoi(*value);
			}
			else if (tag == "in_node_id")
			{
				std::vector<std::vector<NEAT::NNode*>> allLayerNodes;
				allLayerNodes.resize(3);
				allLayerNodes[0] = inputNodes;
				allLayerNodes[1] = outputNodes;
				allLayerNodes[2] = allNodes;

				currentLink->in_node = GetNodeFromID(allLayerNodes, (int) FCString::Atoi(*value));
			}
			else if (tag == "is_recurrent")
			{
				currentLink->is_recurrent = (bool)FCString::Atoi(*value);
			}
			else if (tag == "out_node_id")
			{
				std::vector<std::vector<NEAT::NNode*>> allLayerNodes;
				allLayerNodes.resize(3);
				allLayerNodes[0] = inputNodes;
				allLayerNodes[1] = outputNodes;
				allLayerNodes[2] = allNodes;

				currentLink->in_node = GetNodeFromID(allLayerNodes, (int)FCString::Atoi(*value));
			}
			else if (tag == "time_delay")
			{
				currentLink->time_delay = (bool)FCString::Atoi(*value);
			}
			else if (tag == "trait_id")
			{
				currentLink->trait_id = FCString::Atoi(*value);
			}
			else if (tag == "weight")
			{
				currentLink->weight = FCString::Atoi(*value);
			}
			else if (tag == "links_params")
			{
				currentLink->params[traitParamsCounter] = FCString::Atof(*value);
				traitParamsCounter++;
			}
			else if (tag == "layer info")
			{
				currentNode = new NEAT::NNode(NEAT::nodetype::SENSOR, 0);
				if (value == "hidden")
					allNodes.push_back(currentNode);
				else if (value == "input")
					inputNodes.push_back(currentNode);
				else if (value == "output")
					outputNodes.push_back(currentNode);
				else
					UE_LOG(LogTemp, Warning, TEXT("The layer info is unrecognised"));

				traitParamsCounter = 0;
			}

			tag = "";

			if(value == "")
				previousElementIndex = i + 1;
			else previousElementIndex = i + 1;
		}
		else if (txtData[i] == ':')
		{
			tag = txtData.Mid(previousElementIndex, i - previousElementIndex);
			previousElementIndex = i + 2;
		}
		else if (txtData[i] == '-')
		{

 }
	}

	output->inputs = inputNodes;
	output->all_nodes = allNodes;
	output->outputs = outputNodes;

	return output;
}

void UnrealNNSaverLoader::Clear()
{

}
