// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuC.h"
#include "EOSGameInstanceC.h"
#include "Components\Button.h"

void UMainMenuC::NativeConstruct()
{
	Super::NativeConstruct();
	GameInst = GetGameInstance<UEOSGameInstanceC>();
	GameInst->MainMenuWidget = this;

	SetEnableTrueBtn(LoginBtn);
	SetEnableFalseBtn(CreateSessionBtn);
	SetEnableFalseBtn(FindSessionBtn);

	LoginBtn->OnClicked.AddDynamic(this, &UMainMenuC::LoginBtnClicked);
	CreateSessionBtn->OnClicked.AddDynamic(this, &UMainMenuC::CreateSessionBtnClicked);
	FindSessionBtn->OnClicked.AddDynamic(this, &UMainMenuC::FindSessionBtnClicked);
}

void UMainMenuC::SetEnableTrueBtn(UButton* Button)
{
	Button->SetIsEnabled(true);
}

void UMainMenuC::SetEnableFalseBtn(UButton* Button)
{
	Button->SetIsEnabled(false);
}

void UMainMenuC::LoginBtnClicked()
{
	if (GameInst)
	{
		GameInst->Login();
	}
}

void UMainMenuC::CreateSessionBtnClicked()
{
	if (GameInst)
	{
		SetEnableFalseBtn(CreateSessionBtn);
		SetEnableFalseBtn(FindSessionBtn);
		GameInst->CreateSession();
	}
}

void UMainMenuC::FindSessionBtnClicked()
{
	if (GameInst)
	{
		SetEnableFalseBtn(CreateSessionBtn);
		SetEnableFalseBtn(FindSessionBtn);
		GameInst->FindSession();
	}
}
