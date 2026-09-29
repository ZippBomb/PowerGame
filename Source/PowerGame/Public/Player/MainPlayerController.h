#pragma once

#include <CoreMinimal.h>
#include <GameFramework/PlayerController.h>

#include "MainPlayerController.generated.h"

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

class UUserWidget;

UCLASS()
class POWERGAME_API AMainPlayerController : public APlayerController {

	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OpenUI(UUserWidget* widget);
	UFUNCTION(BlueprintCallable)
	void CloseUI();

	UFUNCTION(BlueprintCallable)
	void EnableDefaultIMC();
	UFUNCTION(BlueprintCallable)
	void DisableDefaultIMC();

	UFUNCTION(BlueprintCallable)
	void AddMappingContext(UInputMappingContext* imc, int priority = 1);
	UFUNCTION(BlueprintCallable)
	void RemoveMappingContext(UInputMappingContext* imc);

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> defaultIMC = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> uiIMC = nullptr;

private:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> m_inputSubsystem = nullptr;

};