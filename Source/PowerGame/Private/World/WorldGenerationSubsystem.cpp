#include "World/WorldGenerationSubsystem.h"
#include "World/TerrainChunk.h"

DEFINE_LOG_CATEGORY(LogWorldGeneration);

void UWorldGenerationSubsystem::Initialize(FSubsystemCollectionBase& collection){

	Super::Initialize(collection);

}

void UWorldGenerationSubsystem::Deinitialize() {

	Super::Deinitialize();

}

void UWorldGenerationSubsystem::GenerateWorld(const FWorldGenerationSettings& settings) {

	m_settings = settings;

	// Generate height map

	TArray<float> heightMap;
	heightMap.Reserve(m_settings.size * m_settings.size);

	GenerateHeightMap(&heightMap, FVector2D::ZeroVector);

	// Generate procedural mesh

	ATerrainChunk* chunk = GetWorld()->SpawnActor<ATerrainChunk>();
	PW_ASSERT(chunk != nullptr, LogWorldGeneration, TEXT("Could not spawn ATerrainChunk actor."));

	chunk->GenerateMesh(heightMap, m_settings.size, m_settings.vertexStep);

}

void UWorldGenerationSubsystem::GenerateHeightMap(TArray<float>* out, FVector2D offset) {

	FMath::SRandInit(m_settings.seed);

	TArray<FVector2D> octaveOffsets;
	for (int i = 0; i < m_settings.octaves; i++) {

		float offsetX = FMath::SRand() * 20000.0f - 10000.0f + offset.X;
		float offsetY = FMath::SRand() * 20000.0f - 10000.0f + offset.Y;
		octaveOffsets.Add(FVector2D(offsetX, offsetY));

	}

	for (int x = 0; x < m_settings.size; x++) {

		for (int y = 0; y < m_settings.size; y++) {

			float amplitude = m_settings.baseAmplitude;
			float frequency = m_settings.baseFrequency;
			float height = 0.0f;

			for (int i = 0; i < m_settings.octaves; i++) {

				float sampleX = x / m_settings.scale * frequency + octaveOffsets[i].X;
				float sampleY = y / m_settings.scale * frequency + octaveOffsets[i].Y;

				height += FMath::PerlinNoise2D(FVector2D(sampleX, sampleY)) * amplitude;

				amplitude *= m_settings.persistance;
				frequency *= m_settings.lacunarity;

			}

			out->Add(height);

		}

	}

}