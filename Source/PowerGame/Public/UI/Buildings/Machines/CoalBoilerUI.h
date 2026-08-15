#pragma once

#include <CoreMinimal.h>
#include "UI/Buildings/Machines/MachineUI.h"

#include "Core/Core.h"

#include "CoalBoilerUI.generated.h"

struct FItemSlot;

class UMachineSlot;

class UTextBlock;

UCLASS(Abstract)
class POWERGAME_API UCoalBoilerUI : public UMachineUI {

	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void UpdateUI(const FItemSlot& input, float fuelProgress, float steam);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UMachineSlot> inputSlot = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> coalText = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> steamText = nullptr;

private:
	UFUNCTION()
	void OnInputAdded(UItemData* item, uint32 quantity);
	UFUNCTION()
	void OnInputRemoved(UItemData* item, uint32 quantity);

};