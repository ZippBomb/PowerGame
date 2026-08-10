#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/MeshBuildInstance.h"

#include "Core/Core.h"

#include "CoalBoilerInstance.generated.h"

class UItemData;

class UCoalBoilerUI;

UCLASS()
class POWERGAME_API ACoalBoilerInstance : public AMeshBuildInstance {

	GENERATED_BODY()
	
public:
	ACoalBoilerInstance();

	virtual void Tick(float deltaTime) override;

	virtual void Interact(AMainPlayerCharacter* character) override;

	UFUNCTION(BlueprintCallable)
	void AddCoal();

	void CloseUI() { uiOpen = false; }

protected:
	UPROPERTY(EditAnywhere, Category = "Production")
	TObjectPtr<UItemData> input = nullptr;

	UPROPERTY(EditAnywhere, Category = "Production")
	float fuelConsumption = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Production")
	float steamProduction = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	bool uiOpen = false;

private:
	UPROPERTY()
	TObjectPtr<UCoalBoilerUI> m_ui = nullptr;

	UPROPERTY(VisibleAnywhere)
	float fuel = 0.0f;
	UPROPERTY(VisibleAnywhere)
	float steam = 0.0f;

};