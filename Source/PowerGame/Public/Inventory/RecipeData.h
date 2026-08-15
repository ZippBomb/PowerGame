#pragma once

#include <CoreMinimal.h>
#include <Engine/DataAsset.h>

#include "RecipeData.generated.h"

class UItemData;

class UMachine;

USTRUCT()
struct POWERGAME_API FRecipeEntry {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UItemData> item = nullptr;
	UPROPERTY(EditAnywhere)
	uint32 amount = 1;

};

UCLASS()
class POWERGAME_API URecipeData : public UPrimaryDataAsset {

	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FText displayName;

	UPROPERTY(EditDefaultsOnly)
	FRecipeEntry input;
	UPROPERTY(EditDefaultsOnly)
	FRecipeEntry output;

	UPROPERTY(EditDefaultsOnly)
	float recipeLength = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMachine> machine = nullptr;

};