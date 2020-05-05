// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

// This class serves as an Unreal Engine wrapper of the FastNoise class by Jordan Peck / Auburns.
// Most but not all of the FastNoise variables are redeclared here and set on an actual FastNoise object during updates.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FastNoise.h"

#include "NoiseLayer.generated.h"


//Enum wrapping/redeclarations
UENUM()
enum class UNoiseType : uint8
{ 
	Value, 
	ValueFractal, 
	Perlin, 
	PerlinFractal, 
	Simplex, 
	SimplexFractal, 
	Cellular, 
	WhiteNoise,
};

UENUM()
enum class UNoiseInterp : uint8 
{
	Linear,
	Hermite,
	Quintic
};

UENUM()
enum class UNoiseFractalType : uint8 
{
	FBM,
	Billow,
	RigidMulti
};

UENUM()
enum class UNoiseCellularType : uint8 
{
	Euclidean,
	Manhattan,
	Natural
};


UENUM()
enum class UNoiseCellularReturnType : uint8 
{
	CellValue,
	NoiseLookup,
	Distance,
	Distance2,
	Distance2Add,
	Distance2Sub,
	Distance2Mul,
	Distance2Div,
};

/**
 * The class is declared as UCLASS to be seen and edited inside the Editor
 * It has no destructors as UObjects have automatic garbage collection

 Abstract Noise Class to be inheritted from
 */
UCLASS(Abstract)
class NOISEGENERATOR_API UNoiseLayer : public UObject
{
	GENERATED_BODY()

protected:
	//Noise generator
	FastNoise Noise;

	//Main noise settings
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 1))
		UNoiseType NoiseType;
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayAfter = "NoiseType"))
		int Seed;
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayAfter = "Seed"))
		float Frequency;
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayAfter = "Frequency"))
		float Amplitude;

public:
	// Default constructor 
	UNoiseLayer();

	// Returns noise for a given 3D point.
	// Set as double for better precision
	virtual double GetHeightAt3DPoint(float X, float Y, float Z);
	virtual double GetHeightAt3DPoint(FVector Vertex);

	// Updates the values of the FastNoise object by setting its properties according to the ones from this class
	virtual void UpdateValues();

};


//Abstract Fractal noise class to be inherited from
UCLASS(Abstract)
class UFractalNoise : public UNoiseLayer
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings",  meta = (DisplayPriority = 2))
	float Lacunarity;
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings",  meta = (DisplayPriority = 2))
		float Gain;
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings",  meta = (DisplayPriority = 2))
		int Octaves;
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseFractalType FractalType;

	UFractalNoise();
	void UpdateValues() override;
};

//Value Noise
UCLASS(EditInlineNew)
class UValueNoise : public UNoiseLayer 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseInterp Interpolation;

	UValueNoise();
	void UpdateValues() override;

};

//Value Fractal Noise
UCLASS(EditInlineNew)
class UValueFractalNoise : public UFractalNoise 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseInterp Interpolation;

	UValueFractalNoise();
	void UpdateValues() override;
};

//Perlin Noise
UCLASS(EditInlineNew)
class UPerlinNoise : public UNoiseLayer
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseInterp Interpolation;

	UPerlinNoise();
	void UpdateValues() override;
};

//Perlin Fractal Noise
UCLASS(EditInlineNew)
class UPerlinFractalNoise : public UFractalNoise {
	GENERATED_BODY()
public:
	UPerlinFractalNoise();
	void UpdateValues() override;
};


//Simplex Noise
UCLASS(EditInlineNew)
class USimplexNoise : public UNoiseLayer {
	GENERATED_BODY()
public:
	USimplexNoise();
	void UpdateValues() override;

};

//Simplex Fractal Noise
UCLASS(EditInlineNew)
class USimplexFractalNoise : public UFractalNoise {
	GENERATED_BODY()
public:
	USimplexFractalNoise();
	void UpdateValues() override;

};

//Cellular Noise
UCLASS(EditInlineNew)
class UCellularNoise : public UNoiseLayer {
	GENERATED_BODY()
public:
	UCellularNoise();
	void UpdateValues() override;

	//Cellular specific settings
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		float Jitter;
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseCellularType CellularType;
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseCellularReturnType CellularReturnType;
	//UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
	//	UNoiseLayer* CellularLookupNoise;
};

//Crater Noise
UCLASS(EditInlineNew)
class UCraterNoise : public UCellularNoise {
	GENERATED_BODY()
public:
	UCraterNoise();
	void UpdateValues() override;

	double GetHeightAt3DPoint(float X, float Y, float Z) override;
	double GetHeightAt3DPoint(FVector Vertex) override;
};