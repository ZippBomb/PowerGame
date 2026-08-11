#pragma once

#include <CoreMinimal.h>
#include "UI/UserWidgetBase.h"

#include "InventoryMenu.generated.h"

class AMainPlayerController;

class UInventoryPanel;

class UInputMappingContext;

UCLASS(Abstract)
class POWERGAME_API UInventoryMenu : public UUserWidgetBase {

	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializeUI(AMainPlayerController* controller);

	UFUNCTION(BlueprintCallable)
	void Open();
	UFUNCTION(BlueprintCallable)
	void Close();

	UFUNCTION(BlueprintCallable)
	inline UInventoryPanel* GetInventoryPanel() const { return inventoryPanel; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UInventoryPanel> inventoryPanel = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> uiIMC = nullptr;

private:
	UPROPERTY()
	TObjectPtr<AMainPlayerController> m_controller = nullptr;

};