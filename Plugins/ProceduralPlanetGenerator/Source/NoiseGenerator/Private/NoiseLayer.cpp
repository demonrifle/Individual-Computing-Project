// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.


#include "NoiseLayer.h"
#include "DVector.h"

UNoiseLayer::UNoiseLayer()
{
	//General
	Seed = 1337;
	Frequency = 0.02f;
	Amplitude = 1.f;

	////Fractal
	//Interpolation = UNoiseInterp::Quintic;
	//Lacunarity = 2.0f;
	//Gain = 0.5f;
	//Octaves = 3;

	////Cellular
	//CellularType = UNoiseCellularType::Euclidean;
	//CellularReturnType = UNoiseCellularReturnType::CellValue;
	//Jitter = 0.45f;
	//CellularLookupNoise = nullptr;
}

double UNoiseLayer::GetHeightAt3DPoint(float X, float Y, float Z)
{
	// Check initialized
	if (this == nullptr) return 1;

	// The returned value is normally between -1 and 1. Add 1 to make it 0-2.
	// Divide by 2 to make it 0-1.
	// Multiply it by Amplitude to get final result.
	double Value = (1 + Noise.GetNoise(X, Y, Z)) / 2 * Amplitude;

	//Divide by 2 to make it 0-1. Multiply by range, then add minimumValue, to make it between min-max. I think.
	//double ret = (value / 2) * (max - min) + min;

	return Value;
}

double UNoiseLayer::GetHeightAt3DPoint(FVector Vertex)
{
	// Check initialized
	if (this == nullptr) return 1;

	// The returned value is normally between -1 and 1. Add 1 to make it 0-2.
	// Divide by 2 to make it 0-1.
	// Multiply it by Amplitude to get final result.
	double Value = (1 + Noise.GetNoise(Vertex.X, Vertex.Y, Vertex.Z)) / 2 * Amplitude;

	//Divide by 2 to make it 0-1. Multiply by range, then add minimumValue, to make it between min-max. I think.
	//double ret = (value / 2) * (max - min) + min;

	return Value;
}

void UNoiseLayer::UpdateValues()
{
	Noise.SetSeed(Seed);
	Noise.SetFrequency(Frequency);
}

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

UValueFractalNoise::UValueFractalNoise() 
{
	Noise.SetNoiseType(FastNoise::ValueFractal);

	UpdateValues();
}

void UValueFractalNoise::UpdateValues() {
	UFractalNoise::UpdateValues();
}

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
	UNoiseLayer::UpdateValues();

}

UPerlinFractalNoise::UPerlinFractalNoise()
{
	Noise.SetNoiseType(FastNoise::PerlinFractal);

	UpdateValues();
}

void UPerlinFractalNoise::UpdateValues()
{
	UFractalNoise::UpdateValues();
}

USimplexNoise::USimplexNoise() 
{
	Noise.SetNoiseType(FastNoise::Simplex);
	UpdateValues();
}

void USimplexNoise::UpdateValues() {
	UNoiseLayer::UpdateValues();
}

USimplexFractalNoise::USimplexFractalNoise()
{
	Noise.SetNoiseType(FastNoise::SimplexFractal);
	UpdateValues();
}

void USimplexFractalNoise::UpdateValues()
{
	UFractalNoise::UpdateValues();
}

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
}

UCraterNoise::UCraterNoise() {
	Noise.SetNoiseType(FastNoise::Cellular);
	UpdateValues();
}

void UCraterNoise::UpdateValues() {

	UCellularNoise::UpdateValues();
}

double UCraterNoise::GetHeightAt3DPoint(float X, float Y, float Z)
{
	// Check initialized
	if (this == nullptr) return 1;

	// The returned value is normally between -1 and 1. Add 1 to make it 0-2.
	// Divide by 2 to make it 0-1.
	// Multiply it by Amplitude to get final result.
	double Value = (1 + Noise.GetNoise(X, Y, Z)) / 2 * Amplitude;

	//Divide by 2 to make it 0-1. Multiply by range, then add minimumValue, to make it between min-max. I think.
	//double ret = (value / 2) * (max - min) + min;

	return Value;
}

double UCraterNoise::GetHeightAt3DPoint(FVector Vertex)
{
	// Check initialized
	if (this == nullptr) return 1;

	// The returned value is normally between -1 and 1. Add 1 to make it 0-2.
	// Divide by 2 to make it 0-1.
	// Multiply it by Amplitude to get final result.
	double Value = (1 + Noise.GetNoise(Vertex.X, Vertex.Y, Vertex.Z)) / 2 * Amplitude;
	double Offset = fmin(Value, 0.06) / 5 - 0.012;
	//Divide by 2 to make it 0-1. Multiply by range, then add minimumValue, to make it between min-max. I think.
	//double ret = (value / 2) * (max - min) + min;
	
	Value = Offset * Amplitude;

	return Value;
}
