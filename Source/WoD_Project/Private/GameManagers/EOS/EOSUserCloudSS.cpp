// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/EOS/EOSUserCloudSS.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineUserCloudInterface.h"

void UEOSUserCloudSS::SavePlayerData(TArray<uint8>& FileData, const FString FileName)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
    IOnlineUserCloudPtr UserCloud = Subsystem->GetUserCloudInterface();

    OnSaveDataHandle =
        UserCloud->AddOnWriteUserFileCompleteDelegate_Handle(
            FOnWriteUserFileCompleteDelegate::CreateUObject(
                this,
                &ThisClass::OnSaveDataCompleted));

    FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(0);
    if (NetId == nullptr) return;

    if (!UserCloud->WriteUserFile(*NetId, FileName, FileData))
    {
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Error writing player data..."));
        UserCloud->ClearOnWriteUserFileCompleteDelegate_Handle(OnSaveDataHandle);
        OnSaveDataHandle.Reset();
    }
}

void UEOSUserCloudSS::OnSaveDataCompleted(bool bWasSuccessful, const FUniqueNetId& UserId, const FString& FileName)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    IOnlineUserCloudPtr UserCloud = Subsystem->GetUserCloudInterface();

    if (bWasSuccessful)
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Player data saved successfully..."));
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Error saving player data (callback)..."));
    }

    UserCloud->ClearOnWriteUserFileCompleteDelegate_Handle(OnSaveDataHandle);
    OnSaveDataHandle.Reset();
}

void UEOSUserCloudSS::LoadPlayerData()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
    IOnlineUserCloudPtr PlayerData = Subsystem->GetUserCloudInterface();

    LoadedData.Empty();

    FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(0);
    if (NetId == nullptr) return;

    OnEnumDataHandle = PlayerData->AddOnEnumerateUserFilesCompleteDelegate_Handle(FOnEnumerateUserFilesCompleteDelegate::CreateUObject(
        this,
        &ThisClass::OnEnumDataCompleted
    ));


    PlayerData->EnumerateUserFiles(*NetId);
}

void UEOSUserCloudSS::OnEnumDataCompleted(bool bWasSuccessfull, const FUniqueNetId& NetId)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    IOnlineUserCloudPtr PlayerData = Subsystem->GetUserCloudInterface();

    if (bWasSuccessfull)
    {
        TArray<FCloudFileHeader> PlayerFiles;
        PlayerData->GetUserFileList(NetId, PlayerFiles);

        if (PlayerFiles.Num() == 0) //if has no files should save the default starting data for the player?
        {
            OnLoadData.Broadcast(true,true);
            GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("No files found..."));
            return;
        }

        TArray<FString> PlayerFileNames;

        for (const auto& File : PlayerFiles)
        {
            PlayerFileNames.Add(File.FileName);
        }

        OnReadDataHandle = PlayerData->AddOnReadUserFileCompleteDelegate_Handle(FOnReadUserFileCompleteDelegate::CreateUObject(
            this,
            &ThisClass::OnReadDataCompleted
        ));

        if (!PlayerData->ReadUserFile(NetId, PlayerFileNames[0]))
        {
            GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Failed to read file (%s)"), *PlayerFileNames[0]));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Error enumerating files (callback)..."));
    }

    PlayerData->ClearOnEnumerateUserFilesCompleteDelegate_Handle(OnEnumDataHandle);
    OnEnumDataHandle.Reset();
}

void UEOSUserCloudSS::OnReadDataCompleted(bool bWasSuccessfull, const FUniqueNetId& UserId, const FString& FileName)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    IOnlineUserCloudPtr PlayerData = Subsystem->GetUserCloudInterface();

    PlayerData->ClearOnReadUserFileCompleteDelegate_Handle(OnReadDataHandle);
    OnReadDataHandle.Reset();

    if (bWasSuccessfull)
    {
        if (PlayerData->GetFileContents(UserId, FileName, LoadedData))// Should use a USaveGame
        {
            OnLoadData.Broadcast(true,false);
            GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("File loaded successfullty..."));
            return;
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Failed to get file (%s) content"), *FileName));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Failed to read file (%s) (callback)"), *FileName));
    }

    LoadedData.Empty();
    OnLoadData.Broadcast(false,false);
}

bool UEOSUserCloudSS::GetLoadedData(TArray<uint8>** OutLoadedData)
{
    *OutLoadedData = &LoadedData;
    return !LoadedData.IsEmpty() && OutLoadedData != nullptr;
}