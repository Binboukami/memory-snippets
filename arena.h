#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef ARENA_MX_SZ_BYTES
	#define ARENA_MX_SZ_BYTES 1024 // ~1KB
#endif

typedef struct
{
	void* data;
	size_t offset;
	size_t capacity;

} Arena;

Arena* CreateArena(size_t size)
{
	if(size > ARENA_MX_SZ_BYTES)
	{
		fprintf(
				stderr,
				"Maximum arena size exceeded: Failed to allocate with size '%lu'(bytes)\n",
				size);

		return NULL;
	}

	// alloc enough space for the arena struct and its body
	void* mem = malloc(sizeof(Arena) + size); 

	memset(mem, 0, sizeof(Arena) + size);

	void* data_offset = mem + sizeof(Arena);

	Arena* arena = mem; // Set the arena struct memory block as the header
	
	arena->offset = 0;
	arena->capacity = size;
	arena->data = data_offset; // Set the remaining block as the data

	return arena;
}

void* ArenaAlloc(Arena* arena, size_t size)
{
	bool overflows = (arena->offset + size) > (arena->capacity);

	if(overflows)
	{
		fprintf(stderr,
				"Maximum arena size exceeded: Failed to allocate with size '%lu'(bytes)\n",
				size);

		return NULL;
	}

	arena->offset += size;

	return arena->data + arena->offset;
}

size_t ArenaRemainingSize(Arena* arena)
{
	return arena->capacity - arena->offset;
}

size_t ArenaClear(Arena* arena)
{
	arena->offset = 0;
	memset(arena->data, 0, arena->capacity);
	return arena->capacity;
}
