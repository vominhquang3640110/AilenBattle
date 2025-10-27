// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Interfaces\OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "EOSGameInstanceC.generated.h"

/**
 *
 */
UCLASS()
class AILENBATTLE_API UEOSGameInstanceC : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Login();
	UFUNCTION(BlueprintCallable)
	void CreateSession();
	UFUNCTION()
	void FindSession();

	UPROPERTY()
	class UMainMenuC* MainMenuWidget;

protected:
	virtual void Init() override;

private:
	IOnlineSubsystem* onlineSubsystem;
	TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> identityPtr;
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> sessionPtr;

	void LoginCompleted(int numbOfPlayers, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void CreateSessionCompleted(FName name, bool bWasSuccessful);
	void FindSessionCompleted(bool bWasSuccessful);
	void JoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> GameLevel;

	TSharedPtr<class FOnlineSessionSearch> sessionSeach;
};