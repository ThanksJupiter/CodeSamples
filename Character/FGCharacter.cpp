#include "SexMagnetCharacter.h"
#include "Components/InputComponent.h"
#include "Components/SMMagnetComponent.h"
#include "Systems/SMMagnetSystem.h"

AFGCharacter::AFGCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

	MagnetComponent = CreateDefaultSubobject<UFGMagnetComponent>(TEXT("MagnetComponent"));

	MagnetSystem = CreateDefaultSubobject<UFGMagnetSystem>(TEXT("MagnetSystem"));
}

void AFGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {

	PlayerInputComponent->BindAxis("RightTriggerAxis", MagnetSystem, &USMMagnetSystem::EngagePositiveField);
	PlayerInputComponent->BindAxis("LeftTriggerAxis", MagnetSystem, &USMMagnetSystem::EngageNegativeField);

	PlayerInputComponent->BindAction("RightTriggerButton", IE_Pressed, MagnetSystem, &USMMagnetSystem::BeginOverlapMagneticObjects);
	PlayerInputComponent->BindAction("LeftTriggerButton", IE_Pressed, MagnetSystem, &USMMagnetSystem::BeginOverlapMagneticObjects);

	PlayerInputComponent->BindAction("RightTriggerButton", IE_Released, MagnetSystem, &USMMagnetSystem::StopOverlap);
	PlayerInputComponent->BindAction("LeftTriggerButton", IE_Released, MagnetSystem, &USMMagnetSystem::StopOverlap);
}

void AFGCharacter::BeginPlay() {
	Super::BeginPlay();

	MagnetSystem->Initialize(this);
}
