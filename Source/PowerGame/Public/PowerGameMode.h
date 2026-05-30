#pragma once

#include <CoreMinimal.h>
#include <GameFramework/GameModeBase.h>
#include <Logging/LogMacros.h>

#include "Core/Core.h"

#include "World/WorldGenerationSubsystem.h"

#include "PowerGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPowerGame, Log, All)

UCLASS()
class POWERGAME_API APowerGameMode : public AGameModeBase {

	GENERATED_BODY()

public:
	APowerGameMode() : Super() {}

private:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "World generation")
	FWorldGenerationSettings worldGenerationSettings;

};
