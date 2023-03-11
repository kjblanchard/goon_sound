#pragma once
#include <vorbis/vorbisfile.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#define BGM_NUM_BUFFERS 4
#define MAX_SFX_SOUNDS 10
#define BGM_BUFFER_SAMPLES 8192  // 8kb
#define VORBIS_REQUEST_SIZE 4096 // Max size to request from vorbis to load.

/**
 * @brief The BGM streaming player.  Probably only need one of these at any time
 *
 */
typedef struct StreamPlayer
{
    ALuint buffers[BGM_NUM_BUFFERS];
    ALuint source;
    ogg_int64_t loop_point_begin;
    ogg_int64_t loop_point_end;
    ogg_int64_t total_bytes_read_this_loop;
    OggVorbis_File vbfile;
    vorbis_info *vbinfo;
    short *membuf;
    ALenum format;
    unsigned short file_loaded;
    unsigned short is_playing;
    short loops;
} StreamPlayer;

typedef struct Sg_Loaded_Sfx
{
    ALsizei size;
    ALenum format;
    long sample_rate;
    short *sound_data;

} Sg_Loaded_Sfx;

/**
 * @brief Initialize the openAl backend
 *
 * @return 1 if successful, 0 if not.
 */
int InitializeAl();

/**
 * @brief Constructor for a BgmPlayer
 *
 * @return A initialized bgmplayer
 */
StreamPlayer *NewPlayer();
/**
 * @brief Gets the BGM source ready to play, and preloads the BGM buffers with data.
 *
 * @param player The BGM player to load.
 * @param filename The filename to open and load.
 * @param loop_begin The seconds where the loop should begin.
 * @param loop_end The seconds where the loop should end.
 * @param volume The volume that we should play, between 0 and 1.
 *
 * @return
 */
int LoadBgmAl(StreamPlayer *player, const char *filename, double *loop_begin, double *loop_end, float volume);
/**
 * @brief Play a streaming BGM.
 *
 * @param filename The file to stream from, this needs to be preloaded.
 * @param loop_begin The point the loop should begin when looping, 0 means beginning.
 * @param loop_end The point the loop should end and move to the loop)begin.  0 means end of file.
 * @param loops How many times we should loop this bgm, -1 means forever.
 *
 * @return 1 on Success, 0 on failure.
 */
int PlayBgmAl(StreamPlayer *player, short loops);
int StopBgmAl(StreamPlayer* player);
/**
 * @brief Pauses the playing bgm_player.
 *
 * @return 1 if successful, 0 if failed.
 */
int PauseBgmAl(StreamPlayer *player);
/**
 * @brief Unpauses the paused bgm_player
 *
 * @return 1 if successful, 0 if failed.
 */
int UnpauseBgmAl(StreamPlayer *player);
/**
 * @brief Loads a buffer full of the full sfx file, and returns it's information.
 *
 * @param filename The name to load
 *
 * @return A Sg_loaded_Sfx, that has the sound_data within it.
 */
Sg_Loaded_Sfx *LoadSfxFileAl(const char *filename);
/**
 * @brief Properly unloads a loaded sfx files memory.
 *
 * @param loaded_sfx The Loaded sfx that should be freed
 *
 * @return 1 if successful, 0 if failed.
 */
int CloseSfxFileAl(Sg_Loaded_Sfx *loaded_sfx);
int PlaySfxAl(Sg_Loaded_Sfx *sound_file, float volume);
/**
 * @brief Updates the openal sound system.
 */
void UpdateAl(StreamPlayer** players, int players_to_update);
/**
 * @brief Closes the AL sound system.
 *
 * @return 1 on Success, 0 on failure.
 */
int CloseAl(StreamPlayer* players, int players_to_update);
