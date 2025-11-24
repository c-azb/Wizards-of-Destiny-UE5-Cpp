// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/EOS/EOSLoginSS.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

void UEOSLoginSS::Login()
{
	IOnlineIdentityPtr Identity = GetIdentityInterface();

	if (!Identity)
	{
		if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
		{
			Identity = OnlineSubsystem->GetIdentityInterface();
		}
	}

	if (Identity && !IsLoggedIn())
	{
		OnLoginHandle = Identity->AddOnLoginCompleteDelegate_Handle(0,
			FOnLoginCompleteDelegate::CreateUObject(this, &ThisClass::OnLoginCompleted)
		);

		FString AuthType;
		FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);
		bool bLoginFail = false;

		if (!AuthType.IsEmpty())
		{
			bLoginFail = !Identity->AutoLogin(0);
		}
		else
		{
			FOnlineAccountCredentials Credentials("AccountPortal", "", "");
			bLoginFail = !Identity->Login(0, Credentials);
		}
		if (bLoginFail)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Login error..."));
			Identity->ClearOnLoginCompleteDelegate_Handle(0, OnLoginHandle);
			OnLoginHandle.Reset();
			OnLogin.Broadcast(false);
		}
	}
}

void UEOSLoginSS::Logout()
{
	IOnlineIdentityPtr Identity = GetIdentityInterface();

	if (Identity && IsLoggedIn())
	{
		OnLogoutHandle = Identity->AddOnLogoutCompleteDelegate_Handle(
			0,
			FOnLogoutCompleteDelegate::CreateUObject(this, &ThisClass::OnLogoutCompleted)
		);

		if (!Identity->Logout(0))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Logout failed..."));
			Identity->ClearOnLogoutCompleteDelegate_Handle(0, OnLogoutHandle);
			OnLogoutHandle.Reset();
		}
	}
}

bool UEOSLoginSS::IsLoggedIn()
{
	if (IOnlineIdentityPtr Identity = GetIdentityInterface())
	{
		FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(0);

		if (NetId != nullptr && Identity->GetLoginStatus(0) == ELoginStatus::LoggedIn)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("User is logged in..."));
			return true;
		}
	}
	return false;
}

void UEOSLoginSS::OnLoginCompleted(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error)
{
	IOnlineIdentityPtr Identity = GetIdentityInterface();

	OnLogin.Broadcast(bWasSuccess);

	Identity->ClearOnLoginCompleteDelegate_Handle(0, OnLoginHandle);
	OnLoginHandle.Reset();

	if (bWasSuccess)
	{
		FString Nickname = Identity->GetPlayerNickname(0);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Welcome %s!"), *Nickname));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("%s"), *Error));
	}
}

void UEOSLoginSS::OnLogoutCompleted(int32 LocalUserNum, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Logout sccessfull..."));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Logout failed (callback)..."));
	}

	GetIdentityInterface()->ClearOnLogoutCompleteDelegate_Handle(0, OnLogoutHandle);
	OnLogoutHandle.Reset();
}

TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> UEOSLoginSS::GetIdentityInterface()
{
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		return OnlineSubsystem->GetIdentityInterface();
	}
	return nullptr;
}
