#pragma once

#include <CoreMinimal.h>
#include "Building/Builds/Build.h"

#include "Core/Core.h"

#include "Machine.generated.h"

class UMachineUI;

UCLASS()
class POWERGAME_API UMachine : public UBuild {

	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UMachineUI> uiClass = nullptr;

};