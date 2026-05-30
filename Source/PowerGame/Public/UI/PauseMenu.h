#pragma once

#include <CoreMinimal.h>
#include "UI/UserWidgetBase.h"

#include "Core/Core.h"

#include "PauseMenu.generated.h"

class AMainPlayerController;

class UButton;

class UInputAction;
class UInputMappingContext;

UCLASS(Abstract)
class POWERGAME_API UPauseMenu : public UUserWidgetBase {

	GENERATED_BODY()

public:
	void InitializeUI(AMainPlayerController* controller);

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void Open();
	UFUNCTION(BlueprintCallable)
	void Close();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UButton> resumeButton = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UButton> saveButton = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UButton> loadButton = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UButton> quitButton = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> uiIMC = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> closeAction = nullptr;
	
	UFUNCTION()
	void Resume();
	UFUNCTION()
	void Quit();

	UFUNCTION()
	void Save();
	UFUNCTION()
	void Load();

private:
	UPROPERTY()
	TObjectPtr<AMainPlayerController> m_controller = nullptr;

};