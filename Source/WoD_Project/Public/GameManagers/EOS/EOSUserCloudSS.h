// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EOSUserCloudSS.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLoadData, bool, bWasSuccess,bool, bInicializeData);

UCLASS()
class WOD_PROJECT_API UEOSUserCloudSS : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	void SavePlayerData(TArray<uint8>& FileData, const FString FileName);
	void LoadPlayerData();
	FOnLoadData OnLoadData;
	bool GetLoadedData(TArray<uint8>** OutLoadedData);

private:
	void OnSaveDataCompleted(bool bWasSuccessful, const FUniqueNetId& UserId, const FString& FileName);
	FDelegateHandle OnSaveDataHandle;

	void OnEnumDataCompleted(bool bWasSuccessfull, const FUniqueNetId& NetId);
	FDelegateHandle OnEnumDataHandle;

	void OnReadDataCompleted(bool bWasSuccessfull, const FUniqueNetId& UserId, const FString& FileName);
	FDelegateHandle OnReadDataHandle;

	TArray<uint8> LoadedData;
};
