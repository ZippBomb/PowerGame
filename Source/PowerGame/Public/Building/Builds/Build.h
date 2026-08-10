#pragma once

#include <CoreMinimal.h>
#include <Engine/DataAsset.h>

#include "Core/Core.h"

#include "Build.generated.h"

class ABuildInstance;
class ABuildGhost;

class UTexture2D;

UCLASS()
class POWERGAME_API UBuild : public UPrimaryDataAsset {

	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 id = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABuildInstance> buildInstanceClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABuildGhost> buildGhostClass = nullptr;

	// Visual

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector wireConnectionLocation = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool rotatable = true;

	// Build Menu

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> buildMenuIcon = nullptr;

};