#include <sys/mman.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define OK 0
#define ERR_IO 1
#define ERR_PARSE 2
#define ERR_PERM 3
#define ERR_INVALID 4

enum
{
  BUFFER_SIZE_SMALL = 128,
  BUFFER_SIZE_DEFAULT = 256,
  BUFFER_SIZE_LARGE = 512,
  PATH_MAX_LEN = 4096
};

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef i16 b16;
typedef i32 b32;

static inline u64
KiB(u64 n)
{
  return n << 10;
}
static inline u64
MiB(u64 n)
{
  return n << 20;
}
static inline u64
GiB(u64 n)
{
  return n << 30;
}

typedef struct mem_arena mem_arena;
mem_arena *
arena_create(u64 capacity);
void
arena_destroy(mem_arena *arena);
void
arena_clear(mem_arena *arena);

/**
 * Arena Helper macro's
 * */

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ALIGN_UP_POW2(n, p) (((u64)(n) + ((u64)(p) - 1)) & (~((u64)(p) - 1)))

/*
 * Represents a disk partition with major/minor device numbers and block count.
 */

/**
 * replacing malloc/free with arena allocaters
 *
 * */

#define ARENA_BASE_POS (sizeof(mem_arena))
// void * for the size of a pointer on the machine, 64/32bit comp
#define ARENA_ALIGN (sizeof(void *))

struct mem_arena
{
  u64 capacity;
  u64 pos;
};

// arena prototypes
mem_arena *
arena_create(u64 capacity);
// make it a void pointer to allow implicit conversion
void
arena_destroy(mem_arena *arena);

void *
arena_push(mem_arena *arena, u64 size, b32 non_zero);

void
arena_pop(mem_arena *arena, u64 size);

void
arena_pop_to(mem_arena *arena, u64 pos);

void
arena_clear(mem_arena *arena);

mem_arena *
arena_create(u64 capacity)
{
  mem_arena *arena = mmap(0, capacity, PROT_READ | PROT_WRITE | PROT_EXEC,
    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (arena == MAP_FAILED)
  {
    assert(0);
  }

  arena->capacity = capacity;
  arena->pos = ARENA_BASE_POS;

  return arena;
}

// make it a void pointer to allow implicit conversion
void
arena_destroy(mem_arena *arena)
{
  munmap(arena, arena->capacity);
}

void *
arena_push(mem_arena *arena, u64 size, b32 non_zero)
{
  u64 pos_aligned = ALIGN_UP_POW2(arena->pos, ARENA_ALIGN);
  u64 new_pos = pos_aligned + size;

  if (new_pos > arena->capacity)
  {
    assert(0);
    return NULL;
  }

  arena->pos = new_pos;
  // cast to u8 to be able to do pointer arithemtic
  u8 *out = (u8 *)arena + pos_aligned;

  if (!non_zero)
  {
    memset(out, 0, size);
  }
  return out;
}
void
arena_pop(mem_arena *arena, u64 size)
{
  size = MIN(size, arena->pos - ARENA_BASE_POS);
  arena->pos -= size;
}

void
arena_pop_to(mem_arena *arena, u64 pos)
{
  u64 size = pos < arena->pos ? arena->pos - pos : 0;
  arena_pop(arena, size);
}

void
arena_clear(mem_arena *arena)
{
  arena_pop_to(arena, ARENA_BASE_POS);
}

#define PUSH_STRUCT(arena, T) (T *)arena_push((arena), sizeof(T), 0)
#define PUSH_STRUCT_NZ(arena, T) (T *)arena_push((arena), sizeof(T), 1)
#define PUSH_ARRAY(arena, T, n) (T *)arena_push((arena), sizeof(T) * (n), 0)
#define PUSH_ARRAY_NZ(arena, T, n) (T *)arena_push((arena), sizeof(T) * (n), 1)
