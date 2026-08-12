#pragma once

#include <CoreMinimal.h>
#include "Inventory/InventoryComponent.h"

#include "PlayerInventoryComponent.generated.h"

class UInventoryMenu;

class UInputAction;

UCLASS()
class POWERGAME_API UPlayerInventoryComponent : public UInventoryComponent {

	GENERATED_BODY()
	
public:
	UPlayerInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void Open();
	UFUNCTION(BlueprintCallable)
	void Close();

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> closeAction = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UInventoryMenu> m_ui = nullptr;

	virtual void BeginPlay() override;

};