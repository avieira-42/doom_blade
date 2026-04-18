#include <stdint.h>
#include <sys/time.h>
#include "game_types.h"

// https://prng.di.unimi.it/xoroshiro128plus.c

static
uint64_t	*stt_next(void)
{
	static uint64_t	state[2] = {9223372036854775783, 6364136223846793005};
	const uint64_t	s0 = state[0];
	const uint64_t	s1 = state[0] ^ state[1];

	state[0] = ((s0 << 24) | (s0 >> 40)) ^ s1 ^ (s1 << 16);
	state[1] = (s1 << 37) | (s1 >> 27);
	return (state);
}

void	ft_rng_init(void)
{
	struct timeval	now;
	uint64_t		seed;
	uint64_t		*state;

	state = stt_next();
	gettimeofday(&now, NULL);
	seed = (uint64_t)(now.tv_sec ^ now.tv_usec);
	state[0] = (seed + 0x9E3779B97f4A7C15);
	state[0] = (state[0] ^ (state[0] >> 30)) * 0xBF58476D1CE4E5B9;
	state[0] = (state[0] ^ (state[0] >> 27)) * 0x94D049BB133111EB;
	state[0] = state[0] ^ (state[0] >> 31);
	state[1] = (state[0] + 0x9E3779B97f4A7C15);
	state[1] = (state[1] ^ (state[1] >> 30)) * 0xBF58476D1CE4E5B9;
	state[1] = (state[1] ^ (state[1] >> 27)) * 0x94D049BB133111EB;
	state[1] = state[1] ^ (state[1] >> 31);
}

uint64_t	ft_rand(void)
{
	uint64_t	*state;

	state = stt_next();
	return (state[0] + state[1]);
}

// Returns a float from 0 to 1
float	ft_randf(void)
{
	t_32	result;

	result.u = (uint32_t)((ft_rand()) >> 41);
	result.u |= 0x3F800000u;			// Sets exponent to 127, so range: 1.0f ~ 2.0f
	return (result.f - 1.0f);
}
