#include <stdio.h>
#include <assert.h>

#include "portaudio.h"
#include "fft_block.h"
#include <unistd.h>


#ifdef _WIN32
#include <conio.h>
#define GETCH _getch
#else
#define GETCH getchar
#endif


#define SAMPLE_RATE 48000
#define FFT_LENGTH  32768
#define PA_CHECKERROR(x) assert( (x) == paNoError);


static int callback(const void* input,
                    void* output,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo* time_info,
                    PaStreamCallbackFlags flags,
                    void* userData)
{
    float* in = (float*)input;
    float* out = (float*)output;
    int compteur = 0;

    /* Perform FFT process */
    fft_block_process(in
                      ,out
                      ,framesPerBuffer
                      ,userData
                      );

    return 0;
}

int main(int argc, const char * argv[])
{
    int fft_err;
    PaStream *stream;
    PaError err;

    /* Initialize fft block */
    fft_err = fft_block_init(SAMPLE_RATE, FFT_LENGTH);

    /* Init Portaudio */
    err = Pa_Initialize();
    PA_CHECKERROR(err);

    /* Use default audio device with one input and output */
    err = Pa_OpenDefaultStream(&stream
                               ,1
                               ,1
                               ,paFloat32
                               ,SAMPLE_RATE
                               ,256
                               ,callback
                               ,NULL);
    PA_CHECKERROR(err);

    /* Let Portaudio start */
    printf("Starting stream... press 'enter' to exit\n");
    err = Pa_StartStream(stream);
    PA_CHECKERROR(err);

    sleep(10);

    /* Stop Portaudio */
    printf("\nDone!\n");
    FILE *f = NULL;
    f = fopen("log_c.txt", "a");
    if (!f)
        return -1;
    fprintf(f, "-----debut nouvel enregistrement------\n");
    fclose(f);
    err = Pa_StopStream(stream);
    PA_CHECKERROR(err);


    /* free the fft block */
    fft_block_close();

    /* Clean up Portaudio */
    err = Pa_CloseStream(stream);
    PA_CHECKERROR(err);

    err = Pa_Terminate();
    PA_CHECKERROR(err);

    return 0;
}
