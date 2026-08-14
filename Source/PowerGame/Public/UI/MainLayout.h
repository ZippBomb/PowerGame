#pragma once

#include <CoreMinimal.h>
#include <UI/UserWidgetBase.h>

#include "Core/Core.h"

#include "MainLayout.generated.h"

class UPauseMenu;
class UInventoryMenu;
class UBuildMenu;

class AMachineBuildInstance;
class UMachineUI;

class UNetworkVisualizer;

UCLASS(Abstract)
class POWERGAME_API UMainLayout : public UUserWidgetBase {

	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	inline UPauseMenu* GetPauseMenu() const { return pauseMenu; }
	UFUNCTION(BlueprintCallable)
	inline UInventoryMenu* GetInventoryMenu() const { return inventoryMenu; }
	UFUNCTION(BlueprintCallable)
	inline UBuildMenu* GetBuildMenu() const { return buildMenu; }

	UFUNCTION(BlueprintCallable)
	inline UNetworkVisualizer* GetNetworkVisualizer() const { return networkVisualizer; }
	
	UFUNCTION(BlueprintCallable)
	UMachineUI* GetMachineUI(TSubclassOf<UMachineUI> uiClass);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UPauseMenu> pauseMenu = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UInventoryMenu> inventoryMenu = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UBuildMenu> buildMenu = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UNetworkVisualizer> networkVisualizer = nullptr;

private:
	UPROPERTY()
	TMap<TSubclassOf<UMachineUI>, TObjectPtr<UMachineUI>> m_machineUIs;

	void RegisterMachineUIs();

};