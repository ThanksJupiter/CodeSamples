#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FGCharacter.generated.h"

class UFGMagnetSystem;
class UFGMagnetComponent;
class UFGArrowSystem;

UCLASS(config = Game)
class AFGCharacter : public ACharacter {

	GENERATED_BODY()

public:
	AFGCharacter(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Magnetism)
	UFGMagnetComponent* MagnetComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Magnetism)
	UFGMagnetSystem* MagnetSystem = nullptr;

	UPROPERTY(VisibleAnywhere, Category = LookDirection)
	UFGArrowSystem* ArrowSystem = nullptr;
};
