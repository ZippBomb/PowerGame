#include "World/TerrainChunk.h"

#include <ProceduralMeshComponent.h>

ATerrainChunk::ATerrainChunk() {

	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	SetRootComponent(mesh);

}

void ATerrainChunk::BeginPlay() {

	Super::BeginPlay();

}

void ATerrainChunk::GenerateMesh(const TArray<float>& heightMap, int size, float vertexStep) {

	int vertexCount = size * size;
	int triangleCount = (size - 1) * (size - 1) * 6;

	float topLeftX = (size - 1) * vertexStep / -2.0f;
	float topLeftY = -topLeftX;

	// Prepare the arrays

	TArray<FVector> vertices;
	TArray<int32> triangles;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FLinearColor> colours;
	TArray<FProcMeshTangent> tangents;

	vertices.Reserve(vertexCount);
	triangles.Reserve(triangleCount);
	normals.Reserve(vertexCount);
	uvs.Reserve(vertexCount);
	colours.Reserve(vertexCount);
	tangents.Reserve(vertexCount);

	// Generate the mesh data

	uint32 vertex = 0;
	for (int x = 0; x < size; x++) {

		for (int y = 0; y < size; y++) {

			float meshX = x * vertexStep;
			float meshY = y * vertexStep;

			vertices.Add(FVector(topLeftX + meshX, topLeftY - meshY, heightMap[x * size + y]));
			normals.Add(FVector::UpVector);
			uvs.Add(FVector2D(x / (float)size, y / (float)size));
			colours.Add(FLinearColor::White);
			tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));

			if (x < size - 1 && y < size - 1) {

				// First triangle

				triangles.Add(vertex + size);
				triangles.Add(vertex + 1);
				triangles.Add(vertex);

				// Second triangle

				triangles.Add(vertex + size);
				triangles.Add(vertex + size + 1);
				triangles.Add(vertex + 1);

			}

			vertex++;

		}

	}

	// Send the data to the procedural mesh

	mesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uvs, colours, tangents, true);

}