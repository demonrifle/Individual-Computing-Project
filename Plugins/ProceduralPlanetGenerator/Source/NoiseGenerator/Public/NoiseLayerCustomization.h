// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

#pragma once

#include "PropertyEditor/Public/IDetailCustomization.h"

/**
 * 
 */
class NOISEGENERATOR_API FNoiseLayerCustomization : public IDetailCustomization
{
public:
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	static TSharedRef<IDetailCustomization> MakeInstance();
};
