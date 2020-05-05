// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "NoiseLayerCustomization.h"
#include "NoiseLayer.h"
#include "PropertyEditing.h"

#define LOCTEXT_NAMESPACE "NoiseGeneratorModule"

void FNoiseLayerCustomization::CustomizeDetails(IDetailLayoutBuilder & DetailBuilder)
{
	TSharedRef<IPropertyHandle> NoiseType = DetailBuilder.GetProperty("NoiseType");
	if (NoiseType->IsValidHandle())
	{
		FString Result;
		NoiseType->GetValueAsFormattedString(Result);
		UE_LOG(LogTemp, Warning, TEXT("Result is %s"), *Result);
	}

	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);

}

TSharedRef<IDetailCustomization> FNoiseLayerCustomization::MakeInstance()
{
	return MakeShareable(new FNoiseLayerCustomization);
}

#undef LOCTEXT_NAMESPACE