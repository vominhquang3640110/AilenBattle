// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuC.generated.h"

/**
 *
 */
UCLASS()
class AILENBATTLE_API UMainMenuC : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	class UEOSGameInstanceC* GameInst;

public:
	void SetEnableTrueBtn(class UButton* Button);
	void SetEnableFalseBtn(class UButton* Button);

	UPROPERTY(meta = (BindWidget))
	class UButton* LoginBtn;
	UPROPERTY(meta = (BindWidget))
	class UButton* CreateSessionBtn;
	UPROPERTY(meta = (BindWidget))
	class UButton* FindSessionBtn;

private:
	UFUNCTION()
	void LoginBtnClicked();
	UFUNCTION()
	void CreateSessionBtnClicked();
	UFUNCTION()
	void FindSessionBtnClicked();
};
