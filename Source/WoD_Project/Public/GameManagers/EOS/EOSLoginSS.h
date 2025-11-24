// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EOSLoginSS.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLogin, bool, bWasSuccess);

UCLASS()
class WOD_PROJECT_API UEOSLoginSS : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void Login();
	FOnLogin OnLogin;

	void Logout();
	bool IsLoggedIn();

private:

	void OnLoginCompleted(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error);
	FDelegateHandle OnLoginHandle;

	void OnLogoutCompleted(int32 LocalUserNum, bool bWasSuccessful);
	FDelegateHandle OnLogoutHandle;

	TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> GetIdentityInterface();
};
