#include "FGMagnetComponent.h"


void UFGMagnetComponent::EnableMagnetism() {
	bIsDisrupted = false;
}

void UFGMagnetComponent::NotifyPositiveField_Implementation(AActor* Instigator, float Value) {
	if (bIsDisrupted == false)
		OnStayPositiveField.Broadcast(Instigator, Value);
}

void UFGMagnetComponent::NotifyNegativeField_Implementation(AActor* Instigator, float Value) {
	if (bIsDisrupted == false)
		OnStayNegativeField.Broadcast(Instigator, Value);
}
