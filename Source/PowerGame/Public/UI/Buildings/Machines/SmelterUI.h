#pragma once

#include <CoreMinimal.h>
#include "UI/Buildings/Machines/MachineUI.h"

#include "SmelterUI.generated.h"

struct FItemSlot;

class UItemData;
class URecipeData;

class UMachineSlot;

class UTextBlock;
class UComboBoxString;

UCLASS(Abstract)
class POWERGAME_API USmelterUI : public UMachineUI {

	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void UpdateUI(const FItemSlot& inputSlot, const FItemSlot& outputSlot, URecipeData* recipe, float progress);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UComboBoxString> recipeSelect = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UMachineSlot> inputSlot = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UMachineSlot> outputSlot = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> progressText = nullptr;

private:
	UFUNCTION()
	void OnRecipeSelected(FString recipeName, ESelectInfo::Type selectionType);

	UFUNCTION()
	void OnInputAdded(UItemData* item, uint32 quantity);
	UFUNCTION()
	void OnInputRemoved(UItemData* item, uint32 quantity);

	UFUNCTION()
	void OnOutputRemoved(UItemData* item, uint32 quantity);

};