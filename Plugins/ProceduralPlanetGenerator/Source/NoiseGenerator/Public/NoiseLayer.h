// Project belongs to Nikolay Nikolov for the purposes of a final-year university project/dissertation. 2020 All rights reserved.

// This class serves as an Unreal Engine wrapper of the FastNoise class by Jordan Peck / Auburns.
// Most but not all of the FastNoise variables are redeclared here and set on an actual FastNoise object during updates.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FastNoise.h"
#include "DVector.h"
#include "Math/RandomStream.h"
#include "Engine.h"

#include "NoiseLayer.generated.h"


// Enum wrapping/redeclarations
// Internal set enum for generated Noise Type. 
// Cannot be externally set
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
	WhiteNoise
};

// Interpolation method. Controls out noise curve generation.
UENUM()
enum class UNoiseInterp : uint8 
{
	Linear,
	Hermite,
	Quintic
};

// Fractal generation types
UENUM()
enum class UNoiseFractalType : uint8 
{
	FBM,
	Billow,
	RigidMulti
};

// Cellular generation types
UENUM()
enum class UNoiseCellularType : uint8 
{
	Euclidean,
	Manhattan,
	Natural
};

// Type of calculation done on noise before returning values.
UENUM()
enum class UNoiseCellularReturnType : uint8 
{
	CellValue,
	Distance,
	Distance2,
	Distance2Add,
	Distance2Sub,
	Distance2Mul,
	Distance2Div,
};


// Abstract Noise Class to be inheritted from

UCLASS(Abstract, DefaultToInstanced)
class NOISEGENERATOR_API UNoiseLayer : public UObject
{
	GENERATED_BODY()

protected:
	// FastNoise generator
		FastNoise Noise;
	// Type of noise
		UNoiseType NoiseType;

public:
	// Toggles the visibiltiy of the layer 
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 1))
		bool LayerVisibility;
	// Seed value for recreating pseudo-random noise
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 1, ClampMin = "0"))
		int Seed;
	// Frequency is the base spatial value for noise. 
	// Determines how far apart noise is sampled. 
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayAfter = "Seed", ClampMin = "0.01", ClampMax = "10"))
		float Frequency;
	// The Amplitude sets the height of the noise.
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayAfter = "Frequency", ClampMin = "0"))
		float Amplitude;
	// The Elevation Reduction reduces the elevation from the noise output by a flat amount
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayAfter = "Amplitude", ClampMin = "0"))
		float ElevationReduction;
	// The Centre Offset shifts the sampling of points, effectively moving features in a desired direction
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayAfter = "ElevationReduction"))
		FVector CentreOffset;
	// Inverts noise values setting highs to lows and vice versa
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayAfter = "CentreOffset", DisplayName = "Invert Noise"))
		bool IsInverted;

public:
	// Default constructor 
	UNoiseLayer();

	// Returns noise for a given 3D point.
	// Set as double for better precision
	virtual double GetHeightAt3DPoint(float X, float Y, float Z);
	virtual double GetHeightAt3DPoint(DVector Vertex);

	// Updates the values of the FastNoise object by setting its properties according to the ones from this class
	virtual void UpdateValues();

	// Gets a randomized noise layer. Type depends on input.
	// 0 - Value 
	// 1 - Value Fractal
	// 2 - Perlin
	// 3 - Perlin Fractal
	// 4 - Simplex
	// 5 - Simplex Fractal
	// 6 - Cellular
	// 7 - White
	static UNoiseLayer* GetRandomNoiseLayer(FRandomStream* Seed);

};


//Abstract Fractal noise class to be inherited from
UCLASS(Abstract)
class UFractalNoise : public UNoiseLayer
{
	GENERATED_BODY()
public:
	// Lacunarity determines how much detail is added or removed at each octave
	// Higher values get small features sticking out of the smooth general pattern
	// Lower values smooth out the pattern.
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings",  meta = (DisplayPriority = 2, ClampMin = "0", ClampMax = "10"))
		float Lacunarity;
	// Gain (Persistance) affect how much octaves contribute to the overall shape.
	// 1 means full affect is applied.
	// 0 means simpler image and octaves don't matter.
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings",  meta = (DisplayPriority = 2, ClampMin = "0", ClampMax = "1"))
		float Gain;
	// Octaves determine the number of levels of detail you want you perlin noise to have.
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings",  meta = (DisplayPriority = 2, ClampMin = "1", ClampMax = "10"))
		int Octaves;
	// FractalType determines the fractal type of generation
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseFractalType FractalType;

	UFractalNoise();
	virtual void UpdateValues();
};

//Value Noise
UCLASS(EditInlineNew)
class UValueNoise : public UNoiseLayer 
{
	GENERATED_BODY()
public:
	// The type of applied Interpolation. 
	// Hermite and Quintic are smoother than Linear
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseInterp Interpolation;

	UValueNoise();
	void UpdateValues();

	// Gets a randomized Value noise layer.
	static UValueNoise* GetRandomLayer(FRandomStream* Seed);

};

//Value Fractal Noise
UCLASS(EditInlineNew)
class UValueFractalNoise : public UFractalNoise 
{
	GENERATED_BODY()
public:
	// The type of applied Interpolation. 
	// Hermite and Quintic are smoother than Linear
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseInterp Interpolation;

	UValueFractalNoise();
	void UpdateValues();


	// Gets a randomized Value Fractal noise layer.
	static UValueFractalNoise* GetRandomLayer(FRandomStream* Seed);
};

//Perlin Noise
UCLASS(EditInlineNew)
class UPerlinNoise : public UNoiseLayer
{
	GENERATED_BODY()
public:
	// The type of applied Interpolation. 
	// Hermite and Quintic are smoother than Linear
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseInterp Interpolation;

	UPerlinNoise();
	void UpdateValues();


	// Gets a randomized Perlin noise layer.
	static UPerlinNoise* GetRandomLayer(FRandomStream* Seed);
};

//Perlin Fractal Noise
UCLASS(EditInlineNew)
class UPerlinFractalNoise : public UFractalNoise {
	GENERATED_BODY()
public:	
	// The type of applied Interpolation. 
	// Hermite and Quintic are smoother than Linear
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseInterp Interpolation;

	UPerlinFractalNoise();
	void UpdateValues();

	// Gets a randomized Perlin Fractal noise layer.
	static UPerlinFractalNoise* GetRandomLayer(FRandomStream* Seed);
};


//Simplex Noise
UCLASS(EditInlineNew)
class USimplexNoise : public UNoiseLayer {
	GENERATED_BODY()
public:
	USimplexNoise();
	void UpdateValues();

	// Gets a randomized Simplex noise layer.
	static USimplexNoise* GetRandomLayer(FRandomStream* Seed);

};

//Simplex Fractal Noise
UCLASS(EditInlineNew)
class USimplexFractalNoise : public UFractalNoise {
	GENERATED_BODY()
public:
	USimplexFractalNoise();
	void UpdateValues();

	// Gets a randomized Simplex Fractal noise layer.
	static USimplexFractalNoise* GetRandomLayer(FRandomStream* Seed);

};

//Cellular Noise
UCLASS(EditInlineNew)
class UCellularNoise : public UNoiseLayer {
	GENERATED_BODY()
public:
	UCellularNoise();
	void UpdateValues();

	// Jitter the maximum distance a cellular point can move from its grid position
	// Setting this high will make artifacts more common
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2, ClampMin = "0"))
		float Jitter;
	// Sets distance function used in cellular noise calculations
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseCellularType CellularType;
	// Sets return type from cellular noise calculations
	UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
		UNoiseCellularReturnType CellularReturnType;
	//UPROPERTY(EditAnywhere, Category = "Noise Layer Settings", meta = (DisplayPriority = 2))
	//	UNoiseLayer* CellularLookupNoise;


	// Gets a randomized Cellular noise layer.
	static UCellularNoise* GetRandomLayer(FRandomStream* Seed);
};

UCLASS()
class UWhiteNoise : public UNoiseLayer
{
	GENERATED_BODY()
public:
	UWhiteNoise();
	void UpdateValues();

	// Gets a randomized Value noise layer.
	static UWhiteNoise* GetRandomLayer(FRandomStream* Seed);
};