#pragma once

#include <CoreMinimal.h>
#include "GameFramework/Actor.h"

#include "TerrainChunk.generated.h"

class UProceduralMeshComponent;

UCLASS()
class POWERGAME_API ATerrainChunk : public AActor {

	GENERATED_BODY()
	
public:
	ATerrainChunk();

	UFUNCTION(BlueprintCallable)
	void GenerateMesh(const TArray<float>& heightMap, int size, float vertexStep);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UProceduralMeshComponent> mesh = nullptr;
	
};