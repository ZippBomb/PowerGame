#pragma once

#include <CoreMinimal.h>
#include "UI/UserWidgetBase.h"
#include "UI/MainHUD.h"

#include "Core/Core.h"

#include "MachineUI.generated.h"

class AMachineBuildInstance;

class AMainPlayerController;

class UInventoryPanel;

class UInputMappingContext;
class UInputAction;

UCLASS(Abstract)
class POWERGAME_API UMachineUI : public UUserWidgetBase {

	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void Open(AMachineBuildInstance* instance);
	UFUNCTION(BlueprintCallable)
	void Close();

	UFUNCTION(BlueprintCallable)
	inline UInventoryPanel* GetInventoryPanel() const { return inventoryPanel; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UInventoryPanel> inventoryPanel = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> uiIMC = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> closeAction = nullptr;

	UPROPERTY()
	TObjectPtr<AMachineBuildInstance> instance = nullptr;

	UPROPERTY()
	TObjectPtr<AMainPlayerController> controller = nullptr;

	template<typename T> T* GetInstance() {

		T* ret = Cast<T>(instance);
		PW_ASSERT(ret != nullptr, LogUI, TEXT("Can't get typed instance."));

		return ret;

	}

};