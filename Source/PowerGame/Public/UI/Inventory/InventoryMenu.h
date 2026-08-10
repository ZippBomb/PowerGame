#pragma once

#include <CoreMinimal.h>
#include "UI/UserWidgetBase.h"

#include "InventoryMenu.generated.h"

class AMainPlayerController;

class UInventorySlot;
struct FItemSlot;

class UWrapBox;

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

	UInventorySlot* AddSlot(const FItemSlot& data);
	void RemoveSlot(int32 index);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UWrapBox> slotContainer = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlot> inventorySlotClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> uiIMC = nullptr;

private:
	UPROPERTY()
	TArray<TObjectPtr<UInventorySlot>> slots;

	UPROPERTY()
	TObjectPtr<AMainPlayerController> m_controller = nullptr;

};