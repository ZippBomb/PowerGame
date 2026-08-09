#pragma once

#include <CoreMinimal.h>
#include "Interactible.h"

#include "OreVein.generated.h"

class UStaticMeshComponent;

class UItemData;

UCLASS()
class POWERGAME_API AOreVein : public AInteractible {

	GENERATED_BODY()
	
public:
	AOreVein();

	virtual void Tick(float deltaTime) override;

	virtual void Interact(AMainPlayerCharacter* character) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> mesh = nullptr;

	UPROPERTY(EditAnywhere, meta = (AllowedPrimaryAssetType = "ItemData"))
	TObjectPtr<UItemData> ore;
	UPROPERTY(EditAnywhere)
	uint32 yield = 1;

	UPROPERTY(EditAnywhere)
	float mineDuration = 1.0f;

	UPROPERTY()
	float mineTimer = 0.0f;
	
};