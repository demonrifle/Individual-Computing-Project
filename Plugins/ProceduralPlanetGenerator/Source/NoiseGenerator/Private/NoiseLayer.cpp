// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "NoiseLayer.h"

// Abstract Base Class
UNoiseLayer::UNoiseLayer()
{
	LayerVisibility = true;
	Seed = 1337;
	Frequency = 0.02f;
	Amplitude = 200.0f;
	ElevationReduction = 0.f;
	CentreOffset = FVector(0.f, 0.f, 0.f);
	IsInverted = false;
}

double UNoiseLayer::GetHeightAt3DPoint(float X, float Y, float Z)
{
	// Check initialized
	if (this == nullptr) return 0;

	// Check if layer should be visible
	if (!LayerVisibility) return 0;

	DVector Vertex = DVector(FVector(X, Y, Z));

	// Lock CentreOffset to a step for precision
	CentreOffset.X = CentreOffset.X - fmod(CentreOffset.X, 0.01);
	CentreOffset.Y = CentreOffset.Y - fmod(CentreOffset.Y, 0.01);
	CentreOffset.Z = CentreOffset.Z - fmod(CentreOffset.Z, 0.01);

	// Get Noise for offset point
	double NoiseValue = Noise.GetNoise(Vertex.x + CentreOffset.X, Vertex.y + CentreOffset.Y, Vertex.z + CentreOffset.Z);

	// The returned value is normally between -1 and 1. Add 1 to make it 0-2.
	// Divide by 2 to make it 0-1.
	// Multiply it by Amplitude to get final result and reduce that by the elevation reduction 
	double Value = ((1 + NoiseValue) / 2 * Amplitude) - ElevationReduction;

	// Return the calculated value ot 0 of lower than that after elevation reduction
	Value = fmax(0.f, Value);

	return Value;
}

double UNoiseLayer::GetHeightAt3DPoint(DVector Vertex)
{
	// Check initialized
	if (this == nullptr) return 1;

	// Check if layer should be visible
	if (!LayerVisibility) return 0;

	// Lock CentreOffset to a step for precision
	CentreOffset.X = CentreOffset.X - fmod(CentreOffset.X, 0.01);
	CentreOffset.Y = CentreOffset.Y - fmod(CentreOffset.Y, 0.01);
	CentreOffset.Z = CentreOffset.Z - fmod(CentreOffset.Z, 0.01);

	// Get Noise for offset point
	double NoiseValue = Noise.GetNoise(Vertex.x + CentreOffset.X, Vertex.y + CentreOffset.Y, Vertex.z + CentreOffset.Z);

	//Sanitize

	// Invert if set
	IsInverted ? NoiseValue *= -1 : NoiseValue;

	// The returned value is normally between -1 and 1. Add 1 to make it 0-2.
	// Divide by 2 to make it 0-1.
	NoiseValue = (1 + NoiseValue) / 2;

	// Multiply it by Amplitude to get final result and reduce that by the elevation reduction 
	double Value = ( NoiseValue * Amplitude) - ElevationReduction;

	// Return the calculated value of 0 or higher after elevation reduction
	Value = fmax(0.f, Value);

	return Value;
}

void UNoiseLayer::UpdateValues()
{
	Noise.SetSeed(Seed);
	Noise.SetFrequency(Frequency);
}

UNoiseLayer * UNoiseLayer::GetRandomNoiseLayer(FRandomStream* Seed)
{
	int LayerType = Seed->RandRange(0, 6);
	switch (LayerType)
	{
		case 0 :	return UValueNoise::GetRandomLayer(Seed);		
		case 1 :	return UValueFractalNoise::GetRandomLayer(Seed);
		case 2 :	return UPerlinNoise::GetRandomLayer(Seed);
		case 3 :	return UPerlinFractalNoise::GetRandomLayer(Seed);
		case 4 :	return USimplexNoise::GetRandomLayer(Seed);
		case 5 :	return USimplexFractalNoise::GetRandomLayer(Seed);
		case 6 :	return UCellularNoise::GetRandomLayer(Seed);
			// Out of generation scope due to generated artefacts.
		case 7 :	return UWhiteNoise::GetRandomLayer(Seed);
	}
	return nullptr;
}

// Abstract Fractal Base Class
UFractalNoise::UFractalNoise()
{
	Lacunarity = 2.0f;
	Gain = 0.5f;
	Octaves = 3;
	FractalType = UNoiseFractalType::FBM;

	UpdateValues();
}

void UFractalNoise::UpdateValues()
{
	switch (FractalType)
	{
	case UNoiseFractalType::FBM :
	{
		Noise.SetFractalType(FastNoise::FractalType::FBM);
		break;
	}
	case UNoiseFractalType::Billow:
	{
		Noise.SetFractalType(FastNoise::FractalType::Billow);
		break;
	}
	case UNoiseFractalType::RigidMulti:
	{
		Noise.SetFractalType(FastNoise::FractalType::RigidMulti);
		break;
	}
	}

	Noise.SetFractalLacunarity(Lacunarity);
	Noise.SetFractalGain(Gain);
	Noise.SetFractalOctaves(Octaves);
	UNoiseLayer::UpdateValues();
}

// Value Noise
UValueNoise::UValueNoise()
{
	Noise.SetNoiseType(FastNoise::NoiseType::Value);
	Interpolation = UNoiseInterp::Quintic;
	UpdateValues();
}

void UValueNoise::UpdateValues()
{
	switch (Interpolation)
	{
	case UNoiseInterp::Linear:
	{
		Noise.SetInterp(FastNoise::Interp::Linear);
		break;
	}
	case UNoiseInterp::Hermite:
	{
		Noise.SetInterp(FastNoise::Interp::Hermite);
		break;
	}
	case UNoiseInterp::Quintic:
	{
		Noise.SetInterp(FastNoise::Interp::Quintic);
		break;
	}
	}
	UNoiseLayer::UpdateValues();
	
}

UValueNoise* UValueNoise::GetRandomLayer(FRandomStream* Seed)
{
	UValueNoise* Layer = NewObject<UValueNoise>();
	Layer->Seed = Seed->RandRange(0, 20000);
	Layer->Frequency = Seed->FRandRange(0.02f, 4.0f);
	Layer->Amplitude = Seed->FRandRange(20.0f, 200.0f);
	Layer->ElevationReduction = Seed->FRandRange(0.0f, Layer->Amplitude - 20.0f);
	
	int LayerInterpolation = Seed->RandRange(0, 2);
	switch (LayerInterpolation)
	{
		case 0 :
		{
			Layer->Interpolation = UNoiseInterp::Linear;
		}
		case 1 :
		{
			Layer->Interpolation = UNoiseInterp::Hermite;
		}
		case 2 :
		{
			Layer->Interpolation = UNoiseInterp::Quintic;
		}
	}

	return Layer;
}




// Value Fractal Noise
UValueFractalNoise::UValueFractalNoise() 
{
	Noise.SetNoiseType(FastNoise::ValueFractal);
	Interpolation = UNoiseInterp::Quintic;

	UpdateValues();
}

void UValueFractalNoise::UpdateValues() 
{
	switch (Interpolation)
	{
	case UNoiseInterp::Linear:
	{
		Noise.SetInterp(FastNoise::Interp::Linear);
		break;
	}
	case UNoiseInterp::Hermite:
	{
		Noise.SetInterp(FastNoise::Interp::Hermite);
		break;
	}
	case UNoiseInterp::Quintic:
	{
		Noise.SetInterp(FastNoise::Interp::Quintic);
		break;
	}
	}
	UFractalNoise::UpdateValues();
}

UValueFractalNoise * UValueFractalNoise::GetRandomLayer(FRandomStream * Seed)
{
	UValueFractalNoise* Layer = NewObject<UValueFractalNoise>();
	Layer->Seed = Seed->RandRange(0, 20000);
	Layer->Frequency = Seed->FRandRange(0.02f, 4.0f);
	Layer->Amplitude = Seed->FRandRange(20.0f, 200.0f);
	Layer->ElevationReduction = Seed->FRandRange(0.0f, Layer->Amplitude - 20.0f);
	Layer->Lacunarity = Seed->FRandRange(0.0f, 4.0f);
	Layer->Gain = Seed->FRandRange(0.0f, 1.0f);
	Layer->Octaves = Seed->RandRange(1, 9);

	int FractalType = Seed->RandRange(0, 2);
	switch (FractalType)
	{
		case 0:
		{
			Layer->FractalType = UNoiseFractalType::FBM;
		}
		case 1:
		{
			Layer->FractalType = UNoiseFractalType::Billow;
		}
		case 2:
		{
			Layer->FractalType = UNoiseFractalType::RigidMulti;
		}
	}

	int LayerInterpolation = Seed->RandRange(0, 2);
	switch (LayerInterpolation)
	{
		case 0:
		{
			Layer->Interpolation = UNoiseInterp::Linear;
		}
		case 1:
		{
			Layer->Interpolation = UNoiseInterp::Hermite;
		}
		case 2:
		{
			Layer->Interpolation = UNoiseInterp::Quintic;
		}
	}

	return Layer;
}



// Perlin Noise
UPerlinNoise::UPerlinNoise()
{
	Noise.SetNoiseType(FastNoise::Perlin);
	Interpolation = UNoiseInterp::Quintic;
	UpdateValues();
}

void UPerlinNoise::UpdateValues()
{
	switch (Interpolation)
	{
	case UNoiseInterp::Linear:
	{
		Noise.SetInterp(FastNoise::Interp::Linear);
		break;
	}
	case UNoiseInterp::Hermite:
	{
		Noise.SetInterp(FastNoise::Interp::Hermite);
		break;
	}
	case UNoiseInterp::Quintic:
	{
		Noise.SetInterp(FastNoise::Interp::Quintic);
		break;
	}
	}
	UNoiseLayer::UpdateValues();

}

UPerlinNoise * UPerlinNoise::GetRandomLayer(FRandomStream * Seed)
{
	UPerlinNoise* Layer = NewObject<UPerlinNoise>();
	Layer->Seed = Seed->RandRange(0, 20000);
	Layer->Frequency = Seed->FRandRange(0.02f, 4.0f);
	Layer->Amplitude = Seed->FRandRange(20.0f, 200.0f);
	Layer->ElevationReduction = Seed->FRandRange(0.0f, Layer->Amplitude - 20.0f);

	int LayerInterpolation = Seed->RandRange(0, 2);
	switch (LayerInterpolation)
	{
	case 0:
	{
		Layer->Interpolation = UNoiseInterp::Linear;
	}
	case 1:
	{
		Layer->Interpolation = UNoiseInterp::Hermite;
	}
	case 2:
	{
		Layer->Interpolation = UNoiseInterp::Quintic;
	}
	}

	return Layer;
}




// Perln Fractal Noise
UPerlinFractalNoise::UPerlinFractalNoise()
{
	Noise.SetNoiseType(FastNoise::PerlinFractal);
	Interpolation = UNoiseInterp::Quintic;

	UpdateValues();
}

void UPerlinFractalNoise::UpdateValues()
{
	switch (Interpolation)
	{
	case UNoiseInterp::Linear:
	{
		Noise.SetInterp(FastNoise::Interp::Linear);
		break;
	}
	case UNoiseInterp::Hermite:
	{
		Noise.SetInterp(FastNoise::Interp::Hermite);
		break;
	}
	case UNoiseInterp::Quintic:
	{
		Noise.SetInterp(FastNoise::Interp::Quintic);
		break;
	}
	}
	UFractalNoise::UpdateValues();
}

UPerlinFractalNoise * UPerlinFractalNoise::GetRandomLayer(FRandomStream * Seed)
{
	UPerlinFractalNoise* Layer = NewObject<UPerlinFractalNoise>();
	Layer->Seed = Seed->RandRange(0, 20000);
	Layer->Frequency = Seed->FRandRange(0.02f, 4.0f);
	Layer->Amplitude = Seed->FRandRange(20.0f, 200.0f);
	Layer->ElevationReduction = Seed->FRandRange(0.0f, Layer->Amplitude - 20.0f);
	Layer->Lacunarity = Seed->FRandRange(0.0f, 4.0f);
	Layer->Gain = Seed->FRandRange(0.0f, 1.0f);
	Layer->Octaves = Seed->RandRange(1, 9);

	int FractalType = Seed->RandRange(0, 2);
	switch (FractalType)
	{
	case 0:
	{
		Layer->FractalType = UNoiseFractalType::FBM;
	}
	case 1:
	{
		Layer->FractalType = UNoiseFractalType::Billow;
	}
	case 2:
	{
		Layer->FractalType = UNoiseFractalType::RigidMulti;
	}
	}

	int LayerInterpolation = Seed->RandRange(0, 2);
	switch (LayerInterpolation)
	{
	case 0:
	{
		Layer->Interpolation = UNoiseInterp::Linear;
	}
	case 1:
	{
		Layer->Interpolation = UNoiseInterp::Hermite;
	}
	case 2:
	{
		Layer->Interpolation = UNoiseInterp::Quintic;
	}
	}

	return Layer;
}

// Simplex Noise
USimplexNoise::USimplexNoise() 
{
	Noise.SetNoiseType(FastNoise::Simplex);
	UpdateValues();
}

void USimplexNoise::UpdateValues() {
	UNoiseLayer::UpdateValues();
}

USimplexNoise * USimplexNoise::GetRandomLayer(FRandomStream * Seed)
{
	USimplexNoise* Layer = NewObject<USimplexNoise>();
	Layer->Seed = Seed->RandRange(0, 20000);
	Layer->Frequency = Seed->FRandRange(0.02f, 4.0f);
	Layer->Amplitude = Seed->FRandRange(20.0f, 200.0f);
	Layer->ElevationReduction = Seed->FRandRange(0.0f, Layer->Amplitude - 20.0f);

	return Layer;
}


// Simplex Fractal Noise
USimplexFractalNoise::USimplexFractalNoise()
{
	Noise.SetNoiseType(FastNoise::SimplexFractal);
	UpdateValues();
}

void USimplexFractalNoise::UpdateValues()
{
	UFractalNoise::UpdateValues();
}

USimplexFractalNoise * USimplexFractalNoise::GetRandomLayer(FRandomStream * Seed)
{
	USimplexFractalNoise* Layer = NewObject<USimplexFractalNoise>();
	Layer->Seed = Seed->RandRange(0, 20000);
	Layer->Frequency = Seed->FRandRange(0.02f, 4.0f);
	Layer->Amplitude = Seed->FRandRange(20.0f, 200.0f);
	Layer->ElevationReduction = Seed->FRandRange(0.0f, Layer->Amplitude - 20.0f);
	Layer->Lacunarity = Seed->FRandRange(0.0f, 4.0f);
	Layer->Gain = Seed->FRandRange(0.0f, 1.0f);
	Layer->Octaves = Seed->RandRange(1, 9);

	int FractalType = Seed->RandRange(0, 2);
	switch (FractalType)
	{
	case 0:
	{
		Layer->FractalType = UNoiseFractalType::FBM;
	}
	case 1:
	{
		Layer->FractalType = UNoiseFractalType::Billow;
	}
	case 2:
	{
		Layer->FractalType = UNoiseFractalType::RigidMulti;
	}
	}

	return Layer;
}

// Cellular Noise
UCellularNoise::UCellularNoise()
{
	Noise.SetNoiseType(FastNoise::Cellular);
	Jitter = 0.45f;;
	CellularType = UNoiseCellularType::Euclidean;
	CellularReturnType = UNoiseCellularReturnType::CellValue;
	UpdateValues();
}

void UCellularNoise::UpdateValues()
{
	Noise.SetCellularJitter(Jitter);

	//Set enum variables by switches
	switch (CellularType)
	{
	case UNoiseCellularType::Euclidean:
	{
		Noise.SetCellularDistanceFunction(FastNoise::CellularDistanceFunction::Euclidean);
		break;
	}
	case UNoiseCellularType::Manhattan:
	{
		Noise.SetCellularDistanceFunction(FastNoise::CellularDistanceFunction::Manhattan);
		break;
	}
	case UNoiseCellularType::Natural:
	{
		Noise.SetCellularDistanceFunction(FastNoise::CellularDistanceFunction::Natural);
		break;
	}
	}

	switch (CellularReturnType)
	{
	case UNoiseCellularReturnType::CellValue:
	{
		Noise.SetCellularReturnType(FastNoise::CellularReturnType::CellValue);
		break;
	}
	case UNoiseCellularReturnType::Distance:
	{
		Noise.SetCellularReturnType(FastNoise::CellularReturnType::Distance);
		break;
	}
	case UNoiseCellularReturnType::Distance2:
	{
		Noise.SetCellularReturnType(FastNoise::CellularReturnType::Distance2);
		break;
	}
	case UNoiseCellularReturnType::Distance2Add:
	{
		Noise.SetCellularReturnType(FastNoise::CellularReturnType::Distance2Add);
		break;
	}
	case UNoiseCellularReturnType::Distance2Div:
	{
		Noise.SetCellularReturnType(FastNoise::CellularReturnType::Distance2Div);
		break;
	}
	case UNoiseCellularReturnType::Distance2Mul:
	{
		Noise.SetCellularReturnType(FastNoise::CellularReturnType::Distance2Mul);
		break;
	}
	case UNoiseCellularReturnType::Distance2Sub:
	{
		Noise.SetCellularReturnType(FastNoise::CellularReturnType::Distance2Sub);
		break;
	}
	}

	UNoiseLayer::UpdateValues();
}

UCellularNoise * UCellularNoise::GetRandomLayer(FRandomStream * Seed)
{
	UCellularNoise* Layer = NewObject<UCellularNoise>();
	Layer->Seed = Seed->RandRange(0, 20000);
	Layer->Frequency = Seed->FRandRange(0.02f, 4.0f);
	Layer->Amplitude = Seed->FRandRange(20.0f, 200.0f);
	Layer->Jitter = Seed->FRandRange(0.0f, 1.0f);
	
	int CellularType = Seed->RandRange(0, 2);
	switch (CellularType)
	{
	case 0:
	{
		Layer->CellularType = UNoiseCellularType::Euclidean;
	}
	case 1:
	{
		Layer->CellularType = UNoiseCellularType::Manhattan;
	}
	case 2:
	{
		Layer->CellularType = UNoiseCellularType::Natural;
	}
	}

	int CellularReturnType = Seed->RandRange(0, 6);
	switch (CellularReturnType)
	{
	case 0:
	{
		Layer->CellularReturnType = UNoiseCellularReturnType::CellValue;
	}
	case 1:
	{
		Layer->CellularReturnType = UNoiseCellularReturnType::Distance;
	}
	case 2:
	{
		Layer->CellularReturnType = UNoiseCellularReturnType::Distance2;
	}
	case 3:
	{
		Layer->CellularReturnType = UNoiseCellularReturnType::Distance2Add;
	}
	case 4:
	{
		Layer->CellularReturnType = UNoiseCellularReturnType::Distance2Div;
	}
	case 5:
	{
		Layer->CellularReturnType = UNoiseCellularReturnType::Distance2Mul;
	}
	case 6:
	{
		Layer->CellularReturnType = UNoiseCellularReturnType::Distance2Sub;
	}
	}

	return Layer;
}

// White Noise
UWhiteNoise::UWhiteNoise()
{
	Noise.SetNoiseType(FastNoise::NoiseType::WhiteNoise);
}

void UWhiteNoise::UpdateValues()
{
	UNoiseLayer::UpdateValues();
}

UWhiteNoise * UWhiteNoise::GetRandomLayer(FRandomStream * Seed)
{
	UWhiteNoise* Layer = NewObject<UWhiteNoise>();
	Layer->Seed = Seed->RandRange(0, 20000);
	Layer->Frequency = Seed->FRandRange(0.02f, 4.0f);
	Layer->Amplitude = Seed->FRandRange(20.0f, 200.0f);

	return Layer;
}


