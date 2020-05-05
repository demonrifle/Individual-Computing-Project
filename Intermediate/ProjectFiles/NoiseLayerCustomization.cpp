#include "NoiseLayerCustomization.h"
#include "PropertyEditing.h"

#define LOCTEXT_NAMESPACE "NoiseLayerDetails"

TSharedRef<IDetailCustomization> FNoiseLayerCustomization::MakeInstance()
{
	return MakeShareable(new FNoiseLayerCustomization);
}

void FNoiseLayerCustomization::CustomizeDetails(IDetailLayoutBuilder & DetailBuilder)
{
}

#undef LOCTEXT_NAMESPACE