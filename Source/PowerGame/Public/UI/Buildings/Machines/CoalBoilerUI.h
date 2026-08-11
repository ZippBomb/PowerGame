#pragma once

#include <CoreMinimal.h>
#include "UI/UserWidgetBase.h"

#include "Core/Core.h"

#include "CoalBoilerUI.generated.h"

class AMainPlayerController;

class ACoalBoilerInstance;

class UButton;
class UTextBlock;

class UInputAction;
class UInputMappingContext;

UCLASS(Abstract)
class POWERGAME_API UCoalBoilerUI : public UUserWidgetBase {

	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void Open(ACoalBoilerInstance* instance);
	UFUNCTION(BlueprintCallable)
	void Close();

	UFUNCTION(BlueprintCallable)
	void UpdateUI(float fuel, float steam);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UButton> button = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> coalText = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> steamText = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> uiIMC = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> closeAction = nullptr;

private:
	UPROPERTY()
	TObjectPtr<ACoalBoilerInstance> m_instance = nullptr;

	UPROPERTY()
	TObjectPtr<AMainPlayerController> m_controller = nullptr;

	UFUNCTION()
	void AddCoal();

};