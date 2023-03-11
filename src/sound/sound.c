#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alhelpers.h"
#include "openal.h"
#include "sound.h"

#define TOTAL_BGM_PLAYERS 2

static StreamPlayer *players[TOTAL_BGM_PLAYERS];

int InitializeSound()
{

    int result = InitializeAl();
    if (!result)
    {
        puts("It's borked!");
    }
    for (size_t i = 0; i < TOTAL_BGM_PLAYERS; i++)
    {
        players[i] = NewPlayer();
    }
}

Bgm *NewBgm(char *filename)
{
    Bgm *bgm = malloc(sizeof(*bgm));
    bgm->bgm_name = filename;
    bgm->loops = -1;
    bgm->is_preloaded = 0;
    bgm->is_ambient = 0;
    return bgm;
}

int LoadBgm(Bgm *bgm, float volume)
{
    if (bgm->is_preloaded)
        return 1;
    return LoadBgmAl(players[bgm->is_ambient], bgm->bgm_name, &bgm->loop_begin, &bgm->loop_end, volume);
}

Sfx *NewSfx(char *filename)
{
    Sfx *sfx = malloc(sizeof(*sfx));
    sfx->sfx_name = filename;
    sfx->loaded_sfx = NULL;
    return sfx;
}

int PlayBgm(Bgm* bgm)
{
    return PlayBgmAl(players[bgm->is_ambient], bgm->loops);
}

int StopBgm(Bgm* bgm)
{
    return StopBgmAl(players[bgm->is_ambient]);
}
int PauseBgm()
{
    return PauseBgmAl(players[0]);
}
int UnPauseBgm()
{
    return UnpauseBgmAl(players[0]);
}

int PlaySfxOneShot(Sfx *sfx, float volume)
{
    if (!sfx->loaded_sfx)
    {
        sfx->loaded_sfx = LoadSfxFileAl(sfx->sfx_name);
    }
    PlaySfxAl(sfx->loaded_sfx, volume);
    return 1;
}

int LoadSfx(Sfx *sfx)
{
    if (!sfx->loaded_sfx)
    {
        sfx->loaded_sfx = LoadSfxFileAl(sfx->sfx_name);
    }
    return (sfx->loaded_sfx != NULL) ? 1 : 0;
}

int UnloadSfx(Sfx *sfx)
{
    if (sfx->loaded_sfx)
    {
        CloseSfxFileAl(sfx->loaded_sfx);
    }
    if (sfx)
    {
        free(sfx->sfx_name);
        sfx->sfx_name = NULL;
        free(sfx);
        sfx = NULL;
    }
    return (sfx == NULL) ? 1 : 0;
}

void UpdateSound()
{
    UpdateAl(players, TOTAL_BGM_PLAYERS);
}
