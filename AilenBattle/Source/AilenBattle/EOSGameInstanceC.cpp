// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstanceC.h"
#include "MainMenuC.h"
#include "OnlineSubsystem.h"
#include "Interfaces\OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"

void UEOSGameInstanceC::Login()
{
	if (identityPtr)
	{
		FOnlineAccountCredentials onlineAccountCredentials;
		onlineAccountCredentials.Type = "accountportal";
		onlineAccountCredentials.Id = "";
		onlineAccountCredentials.Token = "";
		identityPtr->Login(0, onlineAccountCredentials);
	}
}

void UEOSGameInstanceC::CreateSession()
{
	if (sessionPtr)
	{
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bAllowInvites = true;
		sessionSettings.bIsDedicated = false;
		sessionSettings.bIsLANMatch = false;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.bUseLobbiesIfAvailable = true;
		sessionSettings.bUsesPresence = true;
		sessionSettings.bAllowJoinInProgress = true;
		sessionSettings.bAllowJoinViaPresence = true;
		sessionSettings.NumPublicConnections = true;
		sessionSettings.NumPublicConnections = 2;

		//passing an abitray data, and make is avaliable to be read on the client.
		sessionSettings.Set(FName("LobbyName"), FString("MyFunLobby"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		sessionPtr->CreateSession(0, "FunSession", sessionSettings);
	}
}

void UEOSGameInstanceC::FindSession()
{
	if (sessionPtr)
	{
		sessionSeach = MakeShareable(new FOnlineSessionSearch);

		sessionSeach->bIsLanQuery = false;
		sessionSeach->MaxSearchResults = 5;
		sessionSeach->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

		sessionPtr->FindSessions(0, sessionSeach.ToSharedRef());
	}
}

void UEOSGameInstanceC::Init()
{
	Super::Init();

	onlineSubsystem = IOnlineSubsystem::Get();
	identityPtr = onlineSubsystem->GetIdentityInterface();
	identityPtr->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstanceC::LoginCompleted);

	sessionPtr = onlineSubsystem->GetSessionInterface();
	sessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstanceC::CreateSessionCompleted);
	sessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstanceC::FindSessionCompleted);
	sessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOSGameInstanceC::JoinSessionCompleted);
}

void UEOSGameInstanceC::LoginCompleted(int numbOfPlayers, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Logged in"));
		
		MainMenuWidget->SetEnableFalseBtn(MainMenuWidget->LoginBtn);
		MainMenuWidget->SetEnableTrueBtn(MainMenuWidget->CreateSessionBtn);
		MainMenuWidget->SetEnableTrueBtn(MainMenuWidget->FindSessionBtn);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Logged failed: %s"), *Error);
	}
}

void UEOSGameInstanceC::CreateSessionCompleted(FName name, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Create Session successed"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Create Session failed"));
		MainMenuWidget->SetEnableTrueBtn(MainMenuWidget->CreateSessionBtn);
		MainMenuWidget->SetEnableTrueBtn(MainMenuWidget->FindSessionBtn);
	}

	if (GameLevel.IsValid())
	{
		GameLevel.LoadSynchronous();
	}
	const FName LevelName = FName(*FPackageName::ObjectPathToPackageName(GameLevel.ToString()));
	GetWorld()->ServerTravel(LevelName.ToString() + "?listen");
}

void UEOSGameInstanceC::FindSessionCompleted(bool bWasSuccessful)
{
	if (bWasSuccessful && sessionSeach->SearchResults.Num() > 0)
	{
		for (const FOnlineSessionSearchResult& lobbyFound : sessionSeach->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("found session with id : %s"), *lobbyFound.GetSessionIdStr());
		}

		const FOnlineSessionSearchResult& searchResult = sessionSeach->SearchResults[0];
		sessionPtr->JoinSession(0, "", searchResult);
	}
}

void UEOSGameInstanceC::JoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		FString TravelUrl; 
		sessionPtr->GetResolvedConnectString("", TravelUrl);
		GetFirstLocalPlayerController(GetWorld())->ClientTravel(TravelUrl, ETravelType::TRAVEL_Absolute);
	}
	else
	{
		MainMenuWidget->SetEnableTrueBtn(MainMenuWidget->CreateSessionBtn);
		MainMenuWidget->SetEnableTrueBtn(MainMenuWidget->FindSessionBtn);
	}
}

