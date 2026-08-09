#pragma once

#include <CoreMinimal.h>
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

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

};