#pragma once

#include <CoreMinimal.h>
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

class UStaticMesh;

UCLASS()
class POWERGAME_API UItemData : public UPrimaryDataAsset {

	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText displayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> icon = nullptr;

	UPROPERTY(EditDefaultsOnly)
	uint32 stackSize = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> conveyorMesh = nullptr;

};