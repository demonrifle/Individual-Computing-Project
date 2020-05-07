// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "NoiseLayer.h"

// Abstract Base Class
UNoiseLayer::UNoiseLayer()
{
	Seed = 1337;
	Frequency = 0.02f;
	AmplitudeMultiplier = 0.2f;
	ElevationReduction = 0.f;
	CentreOffset = FVector(0.f, 0.f, 0.f);
}

double UNoiseLayer::GetHeightAt3DPoint(float X, float Y, float Z)
{
	// Check initialized
	if (this == nullptr) return 1;

	// Lock CentreOffset to a step equal to the current frequency multiplied by N
	CentreOffset.X = CentreOffset.X - fmod(CentreOffset.X, Frequency);
	CentreOffset.Y = CentreOffset.Y - fmod(CentreOffset.Y, Frequency);
	CentreOffset.Z = CentreOffset.Z - fmod(CentreOffset.Z, Frequency);

	// Get Noise for offset point
	float NoiseValue = Noise.GetNoise(X + CentreOffset.X, Y + CentreOffset.Y, Z + CentreOffset.Z);

	// The returned value is normally between -1 and 1. Add 1 to make it 0-2.
	// Divide by 2 to make it 0-1.
	// Multiply it by AmplitudeMultiplier to get final result and reduce that by the elevation reduction 
	double Value = ((1 + NoiseValue) / 2 * AmplitudeMultiplier) - ElevationReduction;

	// Return the calculated value ot 0 of lower than that after elevation reduction
	Value = fmax(0.f, Value);

	return Value;
}

double UNoiseLayer::GetHeightAt3DPoint(FVector Vertex)
{
	// Check initialized
	if (this == nullptr) return 1;

	// Lock CentreOffset to a step equal to the current frequency multiplied by N
	CentreOffset.X = CentreOffset.X - fmod(CentreOffset.X, Frequency);
	CentreOffset.Y = CentreOffset.Y - fmod(CentreOffset.Y, Frequency);
	CentreOffset.Z = CentreOffset.Z - fmod(CentreOffset.Z, Frequency);

	// Get Noise for offset point
	float NoiseValue = Noise.GetNoise(Vertex.X + CentreOffset.X, Vertex.Y + CentreOffset.Y, Vertex.Z + CentreOffset.Z);

	// The returned value is normally between -1 and 1. Add 1 to make it 0-2.
	// Divide by 2 to make it 0-1.
	// Multiply it by AmplitudeMultiplier to get final result and reduce that by the elevation reduction 
	double Value = ((1 + NoiseValue) / 2 * AmplitudeMultiplier) - ElevationReduction;

	// Return the calculated value ot 0 of lower than that after elevation reduction
	Value = fmax(0.f, Value);

	return Value;
}

void UNoiseLayer::UpdateValues()
{
	Noise.SetSeed(Seed);
	Noise.SetFrequency(Frequency);
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
	Super::UpdateValues();
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
	Super::UpdateValues();
	
}

// Value Fractal Noise
UValueFractalNoise::UValueFractalNoise() 
{
	Noise.SetNoiseType(FastNoise::ValueFractal);

	UpdateValues();
}

void UValueFractalNoise::UpdateValues() {
	Super::UpdateValues();
}

// Perlin Noise
UPerlinNoise::UPerlinNoise()
{
	Noise.SetNoiseType(FastNoise::Perlin);

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
	Super::UpdateValues();

}

// Perln Fractal Noise
UPerlinFractalNoise::UPerlinFractalNoise()
{
	Noise.SetNoiseType(FastNoise::PerlinFractal);

	UpdateValues();
}

void UPerlinFractalNoise::UpdateValues()
{
	Super::UpdateValues();
}

// Simplex Noise
USimplexNoise::USimplexNoise() 
{
	Noise.SetNoiseType(FastNoise::Simplex);
	UpdateValues();
}

void USimplexNoise::UpdateValues() {
	Super::UpdateValues();
}

// Simplex Fractal Noise
USimplexFractalNoise::USimplexFractalNoise()
{
	Noise.SetNoiseType(FastNoise::SimplexFractal);
	UpdateValues();
}

void USimplexFractalNoise::UpdateValues()
{
	Super::UpdateValues();
}

// Cellular Noise
UCellularNoise::UCellularNoise()
{
	Noise.SetNoiseType(FastNoise::Cellular);
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

	Super::UpdateValues();
}
