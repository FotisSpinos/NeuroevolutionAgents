// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"

/**
 * 
 */
class NEUROEVOLUTIONAGENTS_API FileWriter
{
public:

	FileWriter(const FString& directory);
	~FileWriter();

	// records next generation
	void RecordNextGeneration();

	// Saves recorded data to file
	void SaveDataToFile();

	// Record Contents
	void RecordContents(FString description, FString value);
	void RecordContents(FString description, float value);
	void RecordContents(FString description, double value);
	void RecordContents(FString description, int value);
	void RecordContents(FString text);

private:
	FString directory;
	FString fileData;
};
