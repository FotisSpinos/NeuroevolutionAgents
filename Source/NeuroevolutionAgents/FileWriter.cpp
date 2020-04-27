// Fill out your copyright notice in the Description page of Project Settings.

#include "FileWriter.h"
#include "PlatformFileManager.h"
#include "FileHelper.h"

#include "BaseEnvironmentState.h"
#include "BaseEnvironmentMaster.h"

#include <Engine.h>



FileWriter::FileWriter(const FString& directory) :
	directory(directory), fileData(FString(""))
{
	// make sure to clear the file
	FFileHelper::SaveStringToFile(fileData, *directory);
}

FileWriter::~FileWriter()
{
}

void FileWriter::RecordContents(FString description, FString value)
{
	fileData = fileData + "\n" + description + ": " + value;
}

void FileWriter::RecordContents(FString description, float value)
{
	fileData = fileData + "\n" + description + ": " + FString::SanitizeFloat(value);
}

void FileWriter::RecordContents(FString description, double value)
{
	fileData = fileData + "\n" + description + ": " + FString(std::to_string(value).c_str());
}

void FileWriter::RecordContents(FString description, int value)
{
	fileData = fileData + "\n" + description + ": " + FString::FromInt(value);
}

void FileWriter::RecordContents(FString text)
{
	fileData = fileData + "\n" + text;
}

void FileWriter::RecordNextGeneration()
{
}

void FileWriter::SaveDataToFile()
{
	FString loadedContents;
	FFileHelper::LoadFileToString(loadedContents, *directory);

	fileData = loadedContents + fileData;

	// save recorded data to file
	FFileHelper::SaveStringToFile(fileData, *directory);

	// clear file data
	fileData = FString("");
}