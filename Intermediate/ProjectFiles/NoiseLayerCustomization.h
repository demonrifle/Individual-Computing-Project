#pragma once

#include "DetailCustomizations.h"
#include "IDetailCustomization.h"

class FNoiseLayerCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

};