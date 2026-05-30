#pragma once

#include <CoreMinimal.h>
#include <Subsystems/WorldSubsystem.h>
#include <Logging/LogMacros.h>

#include "Core/Core.h"

#include "WorldGenerationSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWorldGeneration, Log, All);

USTRUCT(BlueprintType)
struct FWorldGenerationSettings {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 seed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float scale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 octaves = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float baseFrequency = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float baseAmplitude = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float lacunarity = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = "0.0", UIMax = "1.0"))
	float persistance = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float vertexStep = 100.0f;

};

UCLASS()
class POWERGAME_API UWorldGenerationSubsystem : public UWorldSubsystem {

	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

	UFUNCTION()
	void GenerateWorld(const FWorldGenerationSettings& settings);

private:
	UPROPERTY()
	FWorldGenerationSettings m_settings;

	void GenerateHeightMap(TArray<float>* out, FVector2D offset);

};