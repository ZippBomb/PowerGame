#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/MeshBuildInstance.h"

#include "Core/Core.h"

#include "MachineBuildInstance.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMachines, Log, All);

class UMachineUI;

UCLASS()
class POWERGAME_API AMachineBuildInstance : public AMeshBuildInstance {

	GENERATED_BODY()
	
public:
	AMachineBuildInstance();

	virtual void Interact(AMainPlayerCharacter* character) override;

	void CloseUI() { uiOpen = false; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	bool uiOpen = false;

	UPROPERTY()
	TObjectPtr<UMachineUI> ui = nullptr;

};