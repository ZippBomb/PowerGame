#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "Interactible.generated.h"

class AMainPlayerCharacter;

UCLASS()
class POWERGAME_API AInteractible : public AActor {

	GENERATED_BODY()
	
public:
	AInteractible();

	UFUNCTION(BlueprintCallable)
	virtual void Interact(AMainPlayerCharacter* character) { }

	UFUNCTION(BLueprintCallable)
	inline bool IsInteractible() const { return interactible; }

protected:
	UPROPERTY(EditAnywhere)
	bool interactible = true;
	
};
